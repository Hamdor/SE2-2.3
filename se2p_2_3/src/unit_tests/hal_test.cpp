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
 * @file    hal_test.cpp
 * @version 0.1
 *
 * Unit tests der HAL
 **/

#include "hal_test.hpp"
#include "lib/hal/iostub.hpp"
#include "lib/util/logging.hpp"
#include <iostream>

using namespace std;
using namespace se2::hal;

hal_test::hal_test() {
  // nop
}

hal_test::~hal_test() {
  // nop
}

int hal_test::start_test() {
  int error_count = 0;
  hwaccess* hal = hwaccess::get_instance();
  hal->change_stub(new iostub(0b00000000, 0b11001011, 0b10100000));
  /**
   * motor tests
   **/
  hal->set_motor(motor_right);
  hal->set_motor(motor_slow);
  hal->set_motor(motor_left);
  hal->set_motor(motor_fast);
  hal->set_motor(motor_stop);
  /**
   * switch tests
   **/
  hal->open_switch();
  if(!hal->is_switch_open()) {
    error_count++;
    LOG_ERROR("switch is not open")
  }
  hal->close_switch();
  if(hal->is_switch_open()) {
    error_count++;
    LOG_ERROR("switch is not closed")
  }
  /**
   * light tests
   **/
  hal->set_light(green, true);
  hal->set_light(yellow, true);
  hal->set_light(red, true);
  hal->set_light(green, false);
  hal->set_light(yellow, false);
  hal->set_light(red, false);
  /**
   * button led tests
   **/
  hal->set_led_state(start_button, true);
  hal->set_led_state(start_button, false);
  hal->set_led_state(reset_button, true);
  hal->set_led_state(reset_button, false);
  hal->set_led_state(q1_button, true);
  hal->set_led_state(q1_button, false);
  hal->set_led_state(q2_button, true);
  hal->set_led_state(q2_button, false);
  /**
   * light barrier tests für nicht durchbrochen
   **/
  if (hal->obj_in_light_barrier(entrace_sensor)) {
    error_count++;
    LOG_ERROR("obj in entrace_sensor")
  }
  if (hal->obj_in_light_barrier(height_sensor)) {
    error_count++;
    LOG_ERROR("obj in height_sensor")
  }
  if (hal->obj_in_light_barrier(switch_sensor)) {
    error_count++;
    LOG_ERROR("obj in switch_sensor")
  }
  if (hal->obj_in_light_barrier(slight_sensor)) {
    error_count++;
    LOG_ERROR("obj in slight_sensor")
  }
  if (hal->obj_in_light_barrier(exit_sensor)) {
    error_count++;
    LOG_ERROR("obj in exit_sensor")
  }
  /**
   * height außer toleranzbereich und kein metall tests
   **/
  if (hal->obj_has_valid_height()) {
    error_count++;
    LOG_ERROR("something is in height sensor")
  }
  if (hal->obj_has_metal()) {
    error_count++;
    LOG_ERROR("metal detected")
  }
  /**
   * Buttons nicht gedrückt
   **/
  if (hal->is_button_pressed(button_start)) {
    error_count++;
    LOG_ERROR("button_start is pressed")
  }
  if (hal->is_button_pressed(button_stop)) {
    error_count++;
    LOG_ERROR("button_stop is pressed")
  }
  if (hal->is_button_pressed(button_reset)) {
    error_count++;
    LOG_ERROR("button_reset is pressed")
  }
  if (hal->is_button_pressed(button_estop)) {
    error_count++;
    LOG_ERROR("button_estop is pressed")
  }
  /**
   * light barrier tests für durchbrochen
   **/
  hal->change_stub(new iostub(0b00000000, 0b00010100, 0b01010000));
  if (!hal->obj_in_light_barrier(entrace_sensor)) {
    error_count++;
    LOG_ERROR("obj is not entrace_sensor")
  }
  if (!hal->obj_in_light_barrier(height_sensor)) {
    error_count++;
    LOG_ERROR("obj is not in height_sensor")
  }
  if (!hal->obj_in_light_barrier(switch_sensor)) {
    error_count++;
    LOG_ERROR("obj is not in switch_sensor")
  }
  if (!hal->obj_in_light_barrier(slight_sensor)) {
    error_count++;
    LOG_ERROR("obj is not inslight_sensor")
  }
  if (!hal->obj_in_light_barrier(exit_sensor)) {
    error_count++;
    LOG_ERROR("obj is not in exit_sensor")
  }
  /**
   * height in toleranzbereich und metall erkannt tests
   **/
  if (!hal->obj_has_valid_height()) {
    error_count++;
    LOG_ERROR("not in toleratable height")
  }
  if (!hal->obj_has_metal()) {
    error_count++;
    LOG_ERROR("metal not detected")
  }
  /**
   * Buttons gedrückt
   **/
  if (!hal->is_button_pressed(button_start)) {
    error_count++;
    LOG_ERROR("button_start is not pressed")
  }
  if (!hal->is_button_pressed(button_stop)) {
    error_count++;
    LOG_ERROR("button_stop is not pressed")
  }
  if (!hal->is_button_pressed(button_reset)) {
    error_count++;
    LOG_ERROR("button_reset is not pressed")
  }
  if (!hal->is_button_pressed(button_estop)) {
    error_count++;
    LOG_ERROR("button_estop is not pressed")
  }
  return error_count;
}

