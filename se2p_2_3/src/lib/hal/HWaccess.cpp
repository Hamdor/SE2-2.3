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

#include <iostream>

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
      std::cerr << "hwaccess::get_instance() : ThreadCtl() failed!"
                << std::endl;
  }
  if (!instance) {
    lock_guard lock(s_lock);
    if (!instance) {
      instance = new hwaccess();
    }
  }
  return instance;
}

hwaccess::hwaccess() {
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
}

hwaccess::~hwaccess() {
  delete m_io;
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
  // Port C inhalt speichern?
  // PORTC aka IOANALOG
  //m_io->outbyte(PORTC, 0x10); // startet messung
  // Der Sensor braucht einen moment bis er die Daten in
  // das Register geschrieben hat. Ich weiß nicht genau
  // wie lange, deshalb erstmal 50 loops...
  //uint8_t mask = 0x80;
  //for (int i = 0; i < 50 && !(m_io->inbyte(PORTC) & mask); ++i) {
    // nop
  //}
  // Wert sollte nun im Register stehen
  //uint16_t height = m_io->inshort(PORTC);
  //std::cout << "hwaccess::get_height_value(): " << height << std::endl;
  //return height;
  return 0;
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
