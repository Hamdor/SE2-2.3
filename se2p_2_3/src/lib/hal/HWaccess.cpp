/******************************************************************************
 *                   ____    ____       ___    ____                           *
 *                  /\  _`\ /\  _`\   /'___`\ /\  _`\                         *
 *                  \ \,\L\_\ \ \L\_\/\_\ /\ \\ \ \L\ \                       *
 *                   \/_\__ \\ \  _\L\/_/// /__\ \ ,__/                       *
 *                     /\ \L\ \ \ \L\ \ // /_\ \\ \ \/                        *
 *                     \ `\____\ \____//\______/ \ \_\                        *
 *                      \/_____/\/___/ \/_____/   \/_/                        *
 *                                                                            *
 * Copyright (C) 2014                                                         *
 * Katja Kirstein    <katja.kirstein    (at) haw-hamburg.de>                  *
 * Anne-Lena Kowalka <anne-lena.kowalka (at) haw-hamburg.de>                  *
 * Marian Triebe     <marian.triebe     (at) haw-hamburg.de>                  *
 * Eugen Winter      <eugen.winter      (at) haw-hamburg.de>                  *
 *                                                                            *
 * Gruppe 2.3                                                                 *
 ******************************************************************************/
/**
 * @file    HWaccess.hpp
 * @version 0.1
 *
 * Implementierung der HAL
 * Allgemeiner Zugriff auf Hardware, sowie Ansteuerung der Sensorik und Aktorik
 **/

#include "lib/hal/HWaccess.hpp"

#include "lib/hal/iowrapper.hpp"
#include "lib/util/lock_guard.hpp"
#include "lib/util/logging.hpp"

#ifdef USE_STUBS
  #include "lib/hal/iostub.hpp"
#endif

#include <sstream>
#include <iostream>

#include <sys/siginfo.h>

using namespace se2;
using namespace se2::util;
using namespace se2::hal;

hwaccess* hwaccess::instance = NULL;
mutex     hwaccess::s_lock;

/**
 * Singleton nach DCLP - Double Checked Locking Pattern
 **/
hwaccess* hwaccess::get_instance() {
  /**
   * Dieser Thread möchte gern auf HW zugreifen,
   * ThreadCtl geben.
   **/
  if (ThreadCtl(_NTO_TCTL_IO_PRIV, NULL) == -1) {
    LOG_ERROR("ThreadCtl() failed!")
  }
  if (!instance) {
    lock_guard lock(s_lock);
    if (!instance) {
      instance = new hwaccess();
    }
  }
  return instance;
}

hwaccess::hwaccess() : m_isr(new isr_control) {
  LOG_TRACE("")
#ifdef USE_STUBS
  /**
   * Stub IO
   * USE_STUBS ist definiert in HWaccess.hpp
   **/
  m_io = new iostub(0b00000000, 0b11001011, 0b10100000);
#else
  /**
   * Hardware IO
   **/
  m_io = new iowrapper();
#endif
  m_io->init_input_output();
  init_isr();
}

hwaccess::~hwaccess() {
  LOG_TRACE("")
  delete m_io;
  delete m_isr;
  hwaccess::instance = NULL;
}

#ifdef USE_STUBS
void hwaccess::change_stub(iostub* ptr) {
  if (!ptr) {
    LOG_ERROR("invalid stub");
    return;
  }
  if (m_io) {
    delete m_io;
  }
  m_io = reinterpret_cast<abstract_inout*>(ptr);
}
#endif

/**
 * Für die anzusteuernden Bits kann das übergebene
 * enum einfach zu einem Int gecastet werden. Sonderfall hier
 * ist `motor_fast`, hierbei muss Bit 2 entfernt werden.
 **/
void hwaccess::set_motor(enum motor_modes mode) {
  if (mode == motor_fast) {
    // Motor soll wieder schnell laufen
    // setze Bit 2 auf 0
    m_io->outbit(PORTA, static_cast<uint8_t>(motor_slow), false);
    return;
  }
  const uint8_t bit = static_cast<uint8_t>(mode);
  m_io->outbit(PORTA, bit, true);
}

void hwaccess::open_switch() {
  m_io->outbit(PORTA, SWITCH_BIT, true);
}

void hwaccess::close_switch() {
  m_io->outbit(PORTA, SWITCH_BIT, false);
}

void hwaccess::set_light(enum light_colors light, bool on) {
  m_io->outbit(PORTA, static_cast<uint8_t>(light), on);
}

bool hwaccess::obj_in_light_barrier(enum light_barriers barrier) const {
  return !m_io->inbit(PORTB, static_cast<uint8_t>(barrier));
}

bool hwaccess::obj_has_valid_height() const {
  return m_io->inbit(PORTB, HEIGHT_BIT);
}

uint16_t hwaccess::get_height_value() {
  LOG_TRACE("");
  // Sensor sagen, er soll Wert in Register schreiben
  m_io->outbyte(ANALOG_PORT_A, START_HEIGHT_MEASURE);
  uint8_t  mask   = 0x01 << 7;
  size_t   loop   = 0;
  while(loop++ < 100 && (m_io->inbyte(ANALOG_BASE) & mask) == 0) {
    // nop
  }
  // Wert sollte nun im Register stehen
  uint16_t height = m_io->inshort(ANALOG_PORT_A);
  std::stringstream ss;
  ss << "height: " << std::hex << height;
  LOG_DEBUG(ss.str().c_str())
  return height;
}

bool hwaccess::obj_has_metal() const {
  return m_io->inbit(PORTB, METAL_BIT);
}

bool hwaccess::is_switch_open() const {
  return !m_io->inbit(PORTB, SWITCH_OPEN_BIT);
}

void hwaccess::set_led_state(enum button_leds led, bool on) {
  m_io->outbit(PORTC, static_cast<uint8_t>(led), on);
}

bool hwaccess::is_button_pressed(enum buttons key) const {
  bool value = m_io->inbit(PORTC, static_cast<uint8_t>(key));
  if (key == button_stop || key == button_estop) {
    // Invertierte logik (low aktiv)
    return !value;
  } else {
    // high aktiv
    return value;
  }
}

const struct sigevent* isr(void* arg, int id) {
  struct sigevent* event = static_cast<struct sigevent*>(arg);
  // Hier muss leider die HAL übergangen werden um von einem
  // Register zu lesen/schreiben.
  // Grund: Wir haben keine HAL Instanz, da die isr Funktion
  // nicht von einem `hwaccess` Objekt ist. Die Funktion
  // `hwaccess::get_instance()` lockt unter umständen jedoch
  // ein Mutex, wenn dieses Mutex vergriffen ist würde
  // dies ein Deadlock produzieren.
  // Es ist eigentlich unwahrscheinlich dass das passiert,
  // da wenn wir uns in der `isr` befinden, bereits eine
  // Instanz von `hwaccess` exsistiert. Dieses Problem
  // könnte also nur auftreten während das Singleton gelöscht
  // wird, welches beim `shutdown()` passiert.
  // TODO: Text anpassen da `shutdown()` noch nicht implementiert.
  int irq_val = in8(static_cast<uint16_t>(IRQ_CLEAR_REG));
  event->sigev_value.sival_int = 0;
  event->sigev_notify = SIGEV_PULSE;
  if (irq_val == PORTB_INTERRUPT || irq_val == PORTC_INTERRUPT) {
    // IRQ wurde von Port B oder Port C ausgelöst
    uint8_t portb = in8(static_cast<uint16_t>(PORTB));
    uint8_t portc = in8(static_cast<uint16_t>(PORTC));
    // Werte von Port B um 8 Bit verschieben, damit die Werte
    // von Port C angehängt werden können.
    // TODO: Wenn der Dispatcher implementiert wurde
    // diesen Code noch einmal überdenken!
    event->sigev_value.sival_int = (portb << 8) | portc;
  }
  // Interrupt zurücksetzen
  out8(IRQ_CLEAR_REG, 0);
  return event;
}

void hwaccess::init_isr() {
  LOG_TRACE("")
  // Channel erstellen
  m_isr->m_chid = ChannelCreate(0);
  if (m_isr->m_chid == -1) {
    LOG_ERROR("ChannelCreate() failed!")
    return;
  }
  // Channel öffnen
  m_isr->m_coid = ConnectAttach(0, 0, m_isr->m_chid, _NTO_SIDE_CHANNEL, 0);
  if (m_isr->m_coid == -1) {
    LOG_ERROR("ConnectAttach() failed!")
    return;
  }
  // Initialisiert event struktur auf isr pulse message
  SIGEV_PULSE_INIT(&m_isr->m_event, m_isr->m_coid, SIGEV_PULSE_PRIO_INHERIT,
                   0 /* Dieser wert definiert von wo der puls kam */, 0);
  // IRQ für Port B und Port C aktivieren
  m_io->outbyte(IRQ_ENABLE_REG, IRQ_ENABLE_MASK);
  // Interrupts zurücksetzen
  m_io->outbyte(IRQ_CLEAR_REG, 0);
  // Interrupt an isr (Funktion) binden
  m_isr->m_interruptid = InterruptAttach(IO_IRQ, isr, &m_isr->m_event,
                                         sizeof(m_isr->m_event), 0);
  if (m_isr->m_interruptid == -1) {
    LOG_ERROR("InterruptAttach() failed!")
    return;
  }
}
