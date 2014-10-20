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

using namespace se2;
using namespace se2::util;
using namespace se2::hal;

hwaccess* hwaccess::instance = NULL;
mutex     hwaccess::s_lock;

/**
 * Singleton nach DCLP - Double Checked Locking Pattern
 **/
hwaccess* hwaccess::get_instance() {
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
   * TODO ADD STUBS HERE
   * */
#else
  m_io = new iowrapper();
#endif
  /**
   * TODO Init Control Register (IOCTLADDR)
   * */
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
    m_io->out1(PORTA, static_cast<uint8_t>(motor_slow), false);
    return;
  }
  const uint8_t bit = static_cast<uint8_t>(mode);
  m_io->out1(PORTA, bit, true);
}

void hwaccess::open_switch() {
  m_io->out1(PORTA, SWITCH_BIT, true);
}

void hwaccess::close_switch() {
  m_io->out1(PORTA, SWITCH_BIT, false);
}

void hwaccess::set_light(enum light_colors light, bool on) {
  m_io->out1(PORTA, static_cast<uint8_t>(light), on);
}

bool hwaccess::obj_in_light_barrier(enum light_barriers barrier) const {
  return !m_io->in1(PORTB, static_cast<uint8_t>(barrier));
}

bool hwaccess::obj_has_valid_height() const {
  return m_io->in1(PORTB, HEIGHT_BIT);
}

uint16_t hwaccess::get_height_value() {
  /**
   * TODO: Ansteuerung des Höhensensors
   * 1. Steuersignal setzen
   * 2. Wert auslesen
   * 3. Wert zusammensetzen?
   * "Analogeingabeport" => Adresse?
   * */
  return 0;
}

bool hwaccess::obj_has_metal() const {
  return m_io->in1(PORTB, METAL_BIT);
}

bool hwaccess::is_switch_open() const {
  return m_io->in1(PORTB, SWITCH_OPEN_BIT);
}

void hwaccess::set_led_state(enum button_leds led, bool on) {
  m_io->out1(PORTC, static_cast<uint8_t>(led), on);
}

bool hwaccess::is_button_pressed(enum buttons key) const {
  bool value = m_io->in1(PORTC, static_cast<uint8_t>(key));
  if (key == button_stop || key == button_estop) {
    // Invertierte logik (low aktiv)
    return !value;
  } else {
    // high aktiv
    return value;
  }
}
