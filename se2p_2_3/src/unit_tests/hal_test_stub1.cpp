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
 * @file    hal_test_stub1.cpp
 * @version 0.1
 *
 * Unit tests der HAL
 **/

#include "unit_tests/hal_test_stub1.hpp"

#include "lib/util/logging.hpp"
#include "lib/util/singleton_mgr.hpp"

using namespace std;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::unit_tests;

hal_test_stub1::hal_test_stub1() : m_hal(NULL), m_error(0) {
  // nop
}

hal_test_stub1::~hal_test_stub1() {
  // nop
}

int hal_test_stub1::before_class() {
  m_hal = TO_HAL(singleton_mgr::get_instance(HAL));
  m_hal->change_stub(new iostub(0b00000000, 0b11001011, 0b10100000));
  return 0;
}

int hal_test_stub1::before() {
  m_error = 0;
  return 0;
}

int hal_test_stub1::init() {
  m_test_functions.push_back(&hal_test_stub1::motor_test);
  m_test_functions.push_back(&hal_test_stub1::switch_test);
  m_test_functions.push_back(&hal_test_stub1::button_led_test);
  m_test_functions.push_back(&hal_test_stub1::light_test);
  m_test_functions.push_back(&hal_test_stub1::light_barrier_test);
  m_test_functions.push_back(&hal_test_stub1::buttons_not_pressed_test);
  m_test_functions.push_back(&hal_test_stub1::invalid_heigt_test);
  m_test_functions.push_back(&hal_test_stub1::no_metal_test);
  return 0;
}

int hal_test_stub1::after() {
  return 0;
}

int hal_test_stub1::after_class() {
  return 0;
}

int hal_test_stub1::motor_test() {
  m_hal->set_motor(motor_right);
  m_hal->set_motor(motor_slow);
  m_hal->set_motor(motor_left);
  m_hal->set_motor(motor_fast);
  m_hal->set_motor(motor_stop);
  return 0;
}

int hal_test_stub1::switch_test() {
  /**
   * switch tests
   **/
  m_hal->open_switch();
  if(!m_hal->is_switch_open()) {
    m_error++;
    LOG_ERROR("switch is not open")
  }
  m_hal->close_switch();
  if(m_hal->is_switch_open()) {
    m_error++;
    LOG_ERROR("switch is not closed")
  }
  return m_error;
}

int hal_test_stub1::button_led_test() {
  m_hal->set_led_state(start_button, true);
  m_hal->set_led_state(start_button, false);
  m_hal->set_led_state(reset_button, true);
  m_hal->set_led_state(reset_button, false);
  m_hal->set_led_state(q1_button, true);
  m_hal->set_led_state(q1_button, false);
  m_hal->set_led_state(q2_button, true);
  m_hal->set_led_state(q2_button, false);
  return 0;
}

int hal_test_stub1::light_test() {
  m_hal->set_light(green, true);
  m_hal->set_light(yellow, true);
  m_hal->set_light(red, true);
  m_hal->set_light(green, false);
  m_hal->set_light(yellow, false);
  m_hal->set_light(red, false);
  return 0;
}

int hal_test_stub1::light_barrier_test() {
  if (m_hal->obj_in_light_barrier(entrace_sensor)) {
    m_error++;
    LOG_ERROR("obj in entrace_sensor")
  }
  if (m_hal->obj_in_light_barrier(height_sensor)) {
    m_error++;
    LOG_ERROR("obj in height_sensor")
  }
  if (m_hal->obj_in_light_barrier(switch_sensor)) {
    m_error++;
    LOG_ERROR("obj in switch_sensor")
  }
  if (m_hal->obj_in_light_barrier(slight_sensor)) {
    m_error++;
    LOG_ERROR("obj in slight_sensor")
  }
  if (m_hal->obj_in_light_barrier(exit_sensor)) {
    m_error++;
    LOG_ERROR("obj in exit_sensor")
  }
  return m_error;
}

int hal_test_stub1::buttons_not_pressed_test() {
  if (m_hal->is_button_pressed(button_start)) {
    m_error++;
    LOG_ERROR("button_start is pressed")
  }
  if (m_hal->is_button_pressed(button_stop)) {
    m_error++;
    LOG_ERROR("button_stop is pressed")
  }
  if (m_hal->is_button_pressed(button_reset)) {
    m_error++;
    LOG_ERROR("button_reset is pressed")
  }
  if (m_hal->is_button_pressed(button_estop)) {
    m_error++;
    LOG_ERROR("button_estop is pressed")
  }
  return m_error;
}

int hal_test_stub1::invalid_heigt_test() {
  if (m_hal->obj_has_valid_height()) {
    m_error++;
    LOG_ERROR("something is in height sensor")
  }
 return m_error;
}

int hal_test_stub1::no_metal_test() {
  if (m_hal->obj_has_metal()) {
    m_error++;
    LOG_ERROR("metal detected")
  }
  return m_error;
}
