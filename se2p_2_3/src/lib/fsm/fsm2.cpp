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
 * @file    fsm2.cpp
 * @version 0.2
 *
 * Klasse fuer Zustandsautomaten (FSM2)
 **/

#include "config.h"

#include "lib/fsm/state.hpp"
#include "lib/constants.hpp"
#include "lib/util/logging.hpp"
#include "lib/util/light_mgr.hpp"
#include "lib/hal/HWaccess.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::serial_bus;
using namespace se2::timer;

b2_receive_data::b2_receive_data(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(
      singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SERIAL_DATA);
#ifdef CONVEYOR_2_SINGLEMOD
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
#endif
}

#ifdef CONVEYOR_2_SINGLEMOD
void b2_receive_data::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_existence();
  new (this) b2_realized_object(m_token);
}
#endif

void b2_receive_data::dispatched_event_serial_data() {
  LOG_TRACE("")
  serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  m_token->set_vals_from_telegram(serial->get_telegram());
  new (this) b2_received_object(m_token);
}

b2_received_object::b2_received_object(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_RIGHT);
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
}

void b2_received_object::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_existence();
  new (this) b2_realized_object(m_token);
}

b2_realized_object::b2_realized_object(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT);
}

void b2_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_slow_motor();
  new (this) b2_height_measurement(m_token);
}

b2_height_measurement::b2_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int height = hal->get_height_value();
  m_token->set_height2(height);
  if (TOO_SMALL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
      && height <= TOO_SMALL_HI + HEIGHT_SENSOR_TOLERANCE_MAX) {
    // darf nicht passieren
    // TODO: In Fehlerbehandlung?
  } else if ((HOLE_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
     || (METAL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    m_token->set_is_upside_down(false);
  } else if ((HOLE_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
          || (METAL_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    new (this) b2_token_upside_down(m_token);
    return;
  }
  new (this) b2_valid_height(m_token);
}

b2_token_upside_down::b2_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE);
}

void b2_token_upside_down::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
}

void b2_token_upside_down::dispatched_event_sensor_slide() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  // TODO: Wenn zu lange unterbrochen, dann in error springen,
  // da die rutsche voll.
  new (this) anonymous_token(m_token);
}

b2_valid_height::b2_valid_height(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

void b2_valid_height::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
}

void b2_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  new (this) b2_metal_detection(m_token);
}

b2_metal_detection::b2_metal_detection(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_token->set_is_metal(hal->obj_has_metal());
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  if (mgr->check_order(m_token->is_metal())) {
    new (this) b2_is_correct_order(m_token);
  } else {
    new (this) b2_is_wrong_order(m_token);
  }
}

b2_is_wrong_order::b2_is_wrong_order(token* t) : state::state(t) {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_left_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(REMOVE_TOKEN);
}

void b2_is_wrong_order::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE_R);
}

void b2_is_wrong_order::dispatched_event_sensor_entrance_rising() {
  // Token wurde vom Band entfernt
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_left_motor(false);
  mgr->unrequest_stop_motor(false);
  mgr->notify_death();
  // TODO: Muss jetzt erst erneut die start Taste gedrueckt werden?
  // wie steht das in den Requirements?
  // Wenn ja, dann sollte send_free() auch erst dort aufgerufen werden...
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  new (this) anonymous_token(m_token);
}

b2_is_correct_order::b2_is_correct_order(token* t) : state::state(t) {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_open_switch();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
}

void b2_is_correct_order::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_open_switch();
}

void b2_is_correct_order::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  m_token->pretty_print();
}

void b2_is_correct_order::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor(false);
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}