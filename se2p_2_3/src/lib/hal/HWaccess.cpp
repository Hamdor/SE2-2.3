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
#include "lib/hal/isr.h"

#ifdef UNIT_TESTS_STUB
  #include "lib/hal/iostub.hpp"
#endif

#include <sstream>
#include <iostream>

using namespace se2;
using namespace se2::util;
using namespace se2::hal;

hwaccess* hwaccess::instance = NULL;

hwaccess::hwaccess() : m_isr(new isr_control) {
  LOG_TRACE("")
#ifdef UNIT_TESTS_STUB
  /**
   * Stub IO
   **/
  m_io = new iostub(0b00000000, 0b11001011, 0b10100000);
#else
  /**
   * Hardware IO
   **/
  m_io = new iowrapper();
#endif
}

hwaccess::~hwaccess() {
  delete m_io;
  delete m_isr;
  hwaccess::instance = NULL;
}

// Diese Funktion ist eine NOP funktion
// solange `UNIT_TESTS_STUB` nicht definiert ist
void hwaccess::change_stub(iostub* ptr) {
#ifdef UNIT_TESTS_STUB
  if (!ptr) {
    LOG_ERROR("invalid stub");
    return;
  }
  if (m_io) {
    delete m_io;
  }
  m_io = reinterpret_cast<abstract_inout*>(ptr);

#else
  // nop
#endif
}

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
  return m_io->inshort(ANALOG_PORT_A);
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

int hwaccess::get_isr_channel() const {
  return m_isr->m_chid;
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
  m_isr->m_coid = ConnectAttach(0, 0, m_isr->m_chid, 0, 0);
  if (m_isr->m_coid == -1) {
    LOG_ERROR("ConnectAttach() failed!")
    return;
  }
  isr_coid = m_isr->m_coid; // globale variable setzen...
                            // die isr kennt die hal nicht
  // Interrupts zurücksetzen
  m_io->outbyte(IRQ_CLEAR_REG, 0);
  // IRQ für Port B und Port C aktivieren
  m_io->outbyte(IRQ_ENABLE_REG, IRQ_ENABLE_MASK);
  // Initialisiert event struktur auf isr pulse message
  SIGEV_PULSE_INIT(&m_isr->m_event, m_isr->m_coid, SIGEV_PULSE_PRIO_INHERIT,
                   0 /* Dieser wert definiert von wo der puls kam */, 0);
  // Interrupt an isr (Funktion) binden
  m_isr->m_interruptid = InterruptAttach(IO_IRQ, isr, &m_isr->m_event,
                                         sizeof(m_isr->m_event), 0);
  if (m_isr->m_interruptid == -1) {
    LOG_ERROR("InterruptAttach() failed!")
    return;
  }
  // Globale Variable der ISR initialisieren
  port_old = (in8(static_cast<uint16_t>(PORTB)) << 8) |
              in8(static_cast<uint16_t>(PORTC));
}

void hwaccess::stop_isr() {
  int rc = 0;
  rc = InterruptDetach(m_isr->m_interruptid);
  if (rc) {
    LOG_ERROR("InterruptDetach() failed")
  }
  rc = ConnectDetach(m_isr->m_coid);
  if (rc) {
    LOG_ERROR("ConnectDetach() failed!")
  }
  rc = ChannelDestroy(m_isr->m_chid);
  if (rc) {
    LOG_ERROR("ChannelDestroy() failed!")
  }
}

void hwaccess::initialize() {
  m_io->init_input_output();
  init_isr();
}

void hwaccess::destroy() {
  LOG_TRACE("")
  stop_isr();
}
