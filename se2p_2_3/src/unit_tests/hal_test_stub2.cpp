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
 * @file    hal_test_stub2.cpp
 * @version 0.1
 *
 * Unit tests der HAL
 **/

#include "hal_test_stub2.hpp"

#include "lib/util/logging.hpp"
#include <iostream>

using namespace std;
using namespace se2::hal;
using namespace se2::unit_tests;

hal_test_stub2::hal_test_stub2() : m_hal(NULL), m_error(0) {
  // nop
}

hal_test_stub2::~hal_test_stub2() {
  // nop
}

int hal_test_stub2::beforeClass() {
  m_hal = hwaccess::get_instance();
  m_hal->change_stub(new iostub(0b00000000, 0b00010100, 0b01010000));
  return 0;
}

int hal_test_stub2::before() {
  m_error = 0;
  return 0;
}

int hal_test_stub2::init() {
  m_test_functions.push_back(&hal_test_stub2::light_barrier_interrupted_test);
  m_test_functions.push_back(&hal_test_stub2::valid_height_test);
  m_test_functions.push_back(&hal_test_stub2::metal_detected_test);
  m_test_functions.push_back(&hal_test_stub2::buttons_pressed_test);
  return 0;
}

int hal_test_stub2::after() {
  return 0;
}

int hal_test_stub2::afterClass() {
  return 0;
}

int hal_test_stub2::light_barrier_interrupted_test() {
  if (!m_hal->obj_in_light_barrier(entrace_sensor)) {
    m_error++;
    LOG_ERROR("obj is not entrace_sensor")
  }
  if (!m_hal->obj_in_light_barrier(height_sensor)) {
    m_error++;
    LOG_ERROR("obj is not in height_sensor")
  }
  if (!m_hal->obj_in_light_barrier(switch_sensor)) {
    m_error++;
    LOG_ERROR("obj is not in switch_sensor")
  }
  if (!m_hal->obj_in_light_barrier(slight_sensor)) {
    m_error++;
    LOG_ERROR("obj is not inslight_sensor")
  }
  if (!m_hal->obj_in_light_barrier(exit_sensor)) {
    m_error++;
    LOG_ERROR("obj is not in exit_sensor")
  }
  return m_error;
}

int hal_test_stub2::valid_height_test() {
  if (!m_hal->obj_has_valid_height()) {
    m_error++;
    LOG_ERROR("not in toleratable height")
  }
  return m_error;
}

int hal_test_stub2::metal_detected_test() {
  if (!m_hal->obj_has_metal()) {
    m_error++;
    LOG_ERROR("metal not detected")
  }
  return m_error;
}

int hal_test_stub2::buttons_pressed_test() {
  if (!m_hal->is_button_pressed(button_start)) {
    m_error++;
    LOG_ERROR("button_start is not pressed")
  }
  if (!m_hal->is_button_pressed(button_stop)) {
    m_error++;
    LOG_ERROR("button_stop is not pressed")
  }
  if (!m_hal->is_button_pressed(button_reset)) {
    m_error++;
    LOG_ERROR("button_reset is not pressed")
  }
  if (!m_hal->is_button_pressed(button_estop)) {
    m_error++;
    LOG_ERROR("button_estop is not pressed")
  }
  return m_error;
}
