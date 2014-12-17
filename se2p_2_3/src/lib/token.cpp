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
#include "lib/timer/time_utils.hpp"

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
  delete_timers();
  dispatcher* disp =
    TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->remove_from_all(this);
  reset_internal_times();
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

void token::delete_timers() {
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  for (size_t i = 0; i < m_timer_ids.size(); ++i) {
    hdl->delete_timer(m_timer_ids[i]);
  }
  m_timer_ids.clear();
}

void token::init_internal_times() {
  const timespec add_seg1 = { (time_t)SEGMENT1__SEC, SEGMENT1_NSEC };
  const timespec add_seg2 = { (time_t)SEGMENT2__SEC, SEGMENT2_NSEC };
  const timespec add_seg3 = { (time_t)SEGMENT3__SEC, SEGMENT3_NSEC };
  clock_gettime(CLOCK_REALTIME, &m_timespec_seg1);
  m_timespec_seg1 = time_utils::add(m_timespec_seg1, add_seg1);
  m_timespec_seg2 = m_timespec_seg1;
  m_timespec_seg2 = time_utils::add(m_timespec_seg2, add_seg2);
  m_timespec_seg3 = m_timespec_seg2;
  m_timespec_seg3 = time_utils::add(m_timespec_seg3, add_seg3);
}

void token::reset_internal_times() {
  std::memset(&m_timespec_seg1, 0, sizeof(timespec));
  std::memset(&m_timespec_seg2, 0, sizeof(timespec));
  std::memset(&m_timespec_seg3, 0, sizeof(timespec));
  std::memset(&m_timespec_stop, 0, sizeof(timespec));
}

void token::add_internal_times(int sec, long nsec) {
  timespec add_spec = { (time_t)sec, nsec };
  // TODO Pruefen ob kurz vor erwartetem durchgang, dann nicht addieren
  m_timespec_seg1 = time_utils::add(m_timespec_seg1, add_spec);
  m_timespec_seg2 = time_utils::add(m_timespec_seg2, add_spec);
  m_timespec_seg3 = time_utils::add(m_timespec_seg3, add_spec);
}

void token::stop_internal_times() {
  clock_gettime(CLOCK_REALTIME, &m_timespec_stop);
}

void token::start_internal_times() {
  timespec curspec;
  clock_gettime(CLOCK_REALTIME, &curspec);
  timespec tempspec = time_utils::diff(m_timespec_stop, curspec);
  if (m_timespec_seg1.tv_nsec != 0 && m_timespec_seg2.tv_nsec != 0
      && m_timespec_seg3.tv_nsec != 0) {
    m_timespec_seg1 = time_utils::add(m_timespec_seg1, tempspec);
    m_timespec_seg2 = time_utils::add(m_timespec_seg2, tempspec);
    m_timespec_seg3 = time_utils::add(m_timespec_seg3, tempspec);
  }
}

bool token::check_internal_times(int section) {
  timespec curspec;
  clock_gettime(CLOCK_REALTIME, &curspec);
  std::stringstream ss;
  if (section == SEGMENT_1) {
    const timespec max_offset = { (time_t)OFFSET_CHECK_TIMES_SEG1__SEC,
                                  OFFSET_CHECK_TIMES_SEG1_NSEC };
    const timespec diffspec = time_utils::diff(m_timespec_seg1, curspec);
    ss << "Segment 1 - Sec: " << diffspec.tv_sec
       << " Nsec: "           << diffspec.tv_nsec;
    LOG_TRACE(ss.str().c_str())
    return time_utils::smaller_then(diffspec, max_offset);
  } else if (section == SEGMENT_2) {
    const timespec max_offset = { (time_t)OFFSET_CHECK_TIMES_SEG2__SEC,
                                  OFFSET_CHECK_TIMES_SEG2_MSEC };
    const timespec diffspec = time_utils::diff(m_timespec_seg2, curspec);
    ss << "Segment 2 - Sec: " << diffspec.tv_sec
       << " Nsec: "           << diffspec.tv_nsec;
    LOG_TRACE(ss.str().c_str())
    return time_utils::smaller_then(diffspec, max_offset);
  } else if (section == SEGMENT_3) {
    const timespec max_offset = { (time_t)OFFSET_CHECK_TIMES_SEG3__SEC,
                                   OFFSET_CHECK_TIMES_SEG3_MSEC };
    const timespec diffspec = time_utils::diff(m_timespec_seg3, curspec);
    ss << "Segment 3 - Sec: " << diffspec.tv_sec
       << " Nsec: "           << diffspec.tv_nsec;
    LOG_TRACE(ss.str().c_str())
    return time_utils::smaller_then(diffspec, max_offset);
  } else {
    return false;
  }
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

void token::dispatched_event_turn_token_timeout() {
  m_state->dispatched_event_turn_token_timeout();
}

void token::dispatched_event_remove_token_timeout() {
  m_state->dispatched_event_remove_token_timeout();
}

void token::dispatched_event_close_switch_time() {
  m_state->dispatched_event_close_switch_time();
}
