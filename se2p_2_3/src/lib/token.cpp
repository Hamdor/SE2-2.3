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
 * @file    token.cpp
 * @version 0.1
 *
 * Kontext Klasse fuer Puck/Token
 **/

#include "lib/token.hpp"
#include "lib/constants.hpp"
#include "lib/util/singleton_mgr.hpp"

#include <iostream>

using namespace se2;
using namespace se2::fsm;
using namespace se2::util;
using namespace se2::timer;
using namespace se2::dispatch;

int token::m_id_counter = 0;

void token::set_vals_from_telegram(const serial_bus::telegram& tel) {
  m_id      = tel.m_id;
  m_height1 = tel.m_height1;
  m_height2 = tel.m_height2;
}

int token::get_id() const {
  return m_id;
}

int token::get_height1() const {
  return m_height1;
}

void token::set_height1(int height1) {
  m_height1 = height1;
}

int token::get_height2() const {
  return m_height2;
}

void token::set_height2(int height2) {
  m_height2 = height2;
}

bool token::is_upside_down() const {
  return m_is_upside_down;
}

void token::set_is_upside_down(bool value) {
  m_is_upside_down = value;
}

bool token::is_metal() const {
  return m_is_metal;
}

void token::set_is_metal(bool metal) {
  m_is_metal = metal;
}

void token::reset() {
  m_id             = ++m_id_counter;
  m_height1        = 0;
  m_height2        = 0;
  m_is_metal       = false;
  m_is_upside_down = false;
  m_seg2_ok        = false;
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  for (size_t i = 0; i < m_timer_ids.size(); ++i) {
    hdl->delete_timer(m_timer_ids[i]);
  }
  m_timer_ids.clear();

  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->remove_from_all(this);
}

void token::pretty_print() const {
  std::cout << "[(token) m_id: "      << m_id
            << ", m_height1: "        << m_height1
            << ", m_height2: "        << m_height2
            << ", m_is_metal: "       << (m_is_metal ? "true" : "false")
            << ", m_is_upside_down: " << (m_is_upside_down ? "true" : "false")
            << " ]"                   << std::endl;
}

void token::add_timer_id(int idx) {
  m_timer_ids.push_back(idx);
}

void token::dispatched_event_button_start() {
  m_state->dispatched_event_button_start();
}

void token::dispatched_event_button_stop() {
  m_state->dispatched_event_button_stop();
}

void token::dispatched_event_button_reset() {
  m_state->dispatched_event_button_reset();
}

void token::dispatched_event_button_e_stop() {
  m_state->dispatched_event_button_e_stop();
}

void token::dispatched_event_sensor_entrance() {
  m_state->dispatched_event_sensor_entrance();
}

void token::dispatched_event_sensor_entrance_rising() {
  m_state->dispatched_event_sensor_entrance_rising();
}

void token::dispatched_event_sensor_height() {
  m_state->dispatched_event_sensor_height();
}

void token::dispatched_event_sensor_height_rising() {
  m_state->dispatched_event_sensor_height_rising();
}

void token::dispatched_event_sensor_switch() {
  m_state->dispatched_event_sensor_switch();
}

void token::dispatched_event_sensor_switch_rising() {
  m_state->dispatched_event_sensor_switch_rising();
}

void token::dispatched_event_sensor_slide() {
  m_state->dispatched_event_sensor_slide();
}

void token::dispatched_event_sensor_slide_rising() {
  m_state->dispatched_event_sensor_slide_rising();
}

void token::dispatched_event_sensor_exit() {
  m_state->dispatched_event_sensor_exit();
}

void token::dispatched_event_sensor_exit_rising() {
  m_state->dispatched_event_sensor_exit_rising();
}

void token::dispatched_event_serial_data() {
  m_state->dispatched_event_serial_data();
}

void token::dispatched_event_serial_msg() {
  m_state->dispatched_event_serial_msg();
}

void token::dispatched_event_serial_err() {
  m_state->dispatched_event_serial_err();
}

void token::dispatched_event_serial_next_ok() {
  m_state->dispatched_event_serial_next_ok();
}

void token::dispatched_event_serial_transfer_fin() {
  m_state->dispatched_event_serial_transfer_fin();
}

void token::dispatched_event_serial_e_stopp() {
  m_state->dispatched_event_serial_e_stopp();
}

void token::dispatched_event_serial_unk() {
  m_state->dispatched_event_serial_unk();
}

void token::dispatched_event_seg1_has_to_expire() {
  m_state->dispatched_event_seg1_has_to_expire();
}

void token::dispatched_event_seg2_has_to_expire() {
  m_state->dispatched_event_seg2_has_to_expire();
}

void token::dispatched_event_seg3_has_to_expire() {
  m_state->dispatched_event_seg3_has_to_expire();
}

void token::dispatched_event_seg1_too_late() {
  m_state->dispatched_event_seg1_too_late();
}

void token::dispatched_event_seg2_too_late() {
  m_state->dispatched_event_seg2_too_late();
}

void token::dispatched_event_seg3_too_late() {
  m_state->dispatched_event_seg3_too_late();
}

void token::dispatched_event_slide_full_timeout() {
  m_state->dispatched_event_slide_full_timeout();
}

void token::dispatched_event_turn_token() {
  m_state->dispatched_event_turn_token();
}

void token::dispatched_event_remove_token() {
  m_state->dispatched_event_remove_token();
}
