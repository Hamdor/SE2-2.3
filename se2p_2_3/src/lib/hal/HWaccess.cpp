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
#else
  m_inout = new iowrapper();
#endif
}

void hwaccess::set_motor(enum motor_modes mode, bool slow) {

}

void hwaccess::open_switch() {

}

void hwaccess::close_switch() {

}

void hwaccess::set_light(enum button_leds light, bool on) {

}

bool hwaccess::obj_in_light_barrier(enum light_barriers barrier) const {
  return false;
}

bool hwaccess::obj_has_valid_height() const {
  return false;
}

uint16_t hwaccess::get_height_value() {
  return 0;
}

bool hwaccess::obj_has_metal() const {
  return false;
}

bool hwaccess::is_switch_open() const {
  return false;
}

void hwaccess::set_led_state(enum button_leds led, bool on) {

}

bool hwaccess::is_button_pressed(enum buttons key) const {
  return false;
}
