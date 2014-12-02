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
 * @file    state.cpp
 * @version 0.1
 *
 * Klasse fuer Zustandsautomaten
 **/

#include "config.h"

#include "lib/fsm/state.hpp"
#include "lib/constants.hpp"
#include "lib/util/logging.hpp"

#include <unistd.h>

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::serial_bus;

void state::dispatched_event_button_e_stop() {
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->reregister_e_stop();
  mgr->enter_safe_state();
}

void state::dispatched_event_button_e_stop_rising() {
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->reregister_e_stop_rising();
  mgr->exit_safe_state();
}

anonymous_token::anonymous_token(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->reset();
#if defined(IS_CONVEYOR_1)
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
#elif defined(IS_CONVEYOR_2)
  new (this) b2_receive_data(m_token);
#endif
}

void anonymous_token::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
#if defined(IS_CONVEYOR_1)
  new (this) b1_realized_object(m_token);
#endif
}

/******************************************************************************
 *                                BAND 1 FSM                                  *
 ******************************************************************************/
#ifdef IS_CONVEYOR_1
b1_realized_object::b1_realized_object(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(
      DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT);
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->notify_existence();
}

void b1_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_slow_motor();
  new (this) b1_height_measurement(m_token);
}

b1_height_measurement::b1_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int height = hal->get_height_value();
  m_token->set_height1(height);
  if (TOO_SMALL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
      && height <= TOO_SMALL_HI + HEIGHT_SENSOR_TOLERANCE_MAX) {
    new (this) b1_token_too_small(m_token);
    return;
  } else if ((HOLE_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
     || (METAL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    m_token->set_is_upside_down(false);
  } else if ((HOLE_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
          || (METAL_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    m_token->set_is_upside_down(true);
  }
  new (this) b1_valid_height(m_token);
}

b1_token_too_small::b1_token_too_small(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE);
}

void b1_token_too_small::dispatched_event_sensor_slide() {
  LOG_TRACE("")
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->notify_death();
  new (this) anonymous_token(m_token);
}

void b1_token_too_small::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
}

b1_valid_height::b1_valid_height(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

void b1_valid_height::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->request_fast_motor();
}

void b1_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->open_switch();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
}

void b1_valid_height::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->close_switch();
}

void b1_valid_height::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  new (this) b1_exit(m_token);
}

b1_exit::b1_exit(token* t) : state::state(t) {
  LOG_TRACE("")
  if (m_token->is_upside_down()) {
    new (this) b1_token_upside_down(m_token);
  } else {
    new (this) b1_token_ready_for_b2(m_token);
  }
}

b1_token_upside_down::b1_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void b1_token_upside_down::dispatched_event_button_start() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  new (this) b1_token_ready_for_b2(m_token);
}

b1_token_ready_for_b2::b1_token_ready_for_b2(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->pretty_print();
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  if (!mgr->check_conveyor2_ready()) {
    mgr->request_stop_motor();
    dispatcher* disp =
        TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(m_token, EVENT_SERIAL_NEXT_OK);
  } else {
    telegram tg(m_token);
    TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tg);
    mgr->notify_token_trasition();
    sleep(1); // FIXME
    mgr->notify_death();
    new (this) anonymous_token(m_token);
  }
}

void b1_token_ready_for_b2::dispatched_event_serial_next_ok() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
  telegram tg(m_token);
  TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tg);
}

void b1_token_ready_for_b2::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
  sleep(1); // FIXME: Bei manchen maschinen ist der Sensor nicht ganz am Ende...
            // Am besten mit einem Timer loesen...
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  mgr->notify_token_trasition();
  new (this) anonymous_token(m_token);
}

#endif

/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/
#ifdef IS_CONVEYOR_2

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
  // FIXME: Hier sollte eigentlich ueber den `token_mgr` gegangen werden,
  // problem ist, dass der Motor aktiviert werden muss bevor die
  // Eingagnslichtschranke durchbrochen werden konnte...
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
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int height = hal->get_height_value();
  m_token->set_height2(height);
  if (TOO_SMALL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
      && height <= TOO_SMALL_HI + HEIGHT_SENSOR_TOLERANCE_MAX) {
    // darf nicht passieren
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
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
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
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

void b2_valid_height::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
}

void b2_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->open_switch();
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
    TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->close_switch();
  }
}

b2_is_wrong_order::b2_is_wrong_order(token* t) : state::state(t) {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_left_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
}

void b2_is_wrong_order::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
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
  new (this) anonymous_token(m_token);
}

b2_is_correct_order::b2_is_correct_order(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
}

void b2_is_correct_order::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->close_switch();
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
#endif
