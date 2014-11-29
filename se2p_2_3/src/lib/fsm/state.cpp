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
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(se2::util::TOKEN_PLUGIN));
  mgr->reregister_e_stop_rising();
  mgr->exit_safe_state();
}

anonymous_token::anonymous_token(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->reset();
#ifdef IS_CONVEYOR_1
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
#endif
#ifdef IS_CONVEYOR_2
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->request_stop_motor();
  new (this) b2_receive_data(m_token);
#endif
}

void anonymous_token::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
#ifdef IS_CONVEYOR_1
  new (this) b1_realized_object(this->m_token);
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
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->notify_exsistens();
}

void b1_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("")
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->request_slow_motor();
  // TODO: Pruefen, ob Timer fuer die langsame Strecke notwendig ist
  new (this) b1_height_measurement(this->m_token);
}

b1_height_measurement::b1_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int height = hal->get_height_value();
  m_token->set_height1(height);
  if (TOO_SMALL_LOW <= height && height <= TOO_SMALL_HI) {
    new (this) b1_token_too_small(m_token);
  } else if ((HOLE_LOW <= height && height <= HOLE_HI)
          || (METAL_LOW <= height && height <= METAL_HI)) {
    m_token->set_is_upside_down(false);
  } else if ((HOLE_BOTTOM_UP_LOW <= height && height <= HOLE_BOTTOM_UP_HI)
          || (METAL_BOTTOM_UP_LOW <= height && height <= METAL_BOTTOM_UP_HI)) {
    m_token->set_is_upside_down(true);
  }
  new (this) b1_valid_height(m_token);
}

b1_token_too_small::b1_token_too_small(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE);
}

void b1_token_too_small::dispatched_event_sensor_slide() {
  LOG_TRACE("")
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->notify_death();
  new (this) anonymous_token(m_token);
}

void b1_token_too_small::dispatched_event_sensor_height_rising() {
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->request_fast_motor();
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
  new (this) b1_metal_detection(m_token);
}

b1_metal_detection::b1_metal_detection(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_token->set_is_metal(hal->obj_has_metal());
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
}

void b1_metal_detection::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->close_switch();
}

void b1_metal_detection::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  new (this) b1_exit(m_token);
}

b1_exit::b1_exit(token* t) : state::state(t) {
  LOG_TRACE("")
  if (m_token->get_is_upside_down()) {
    new (this) b1_token_upside_down(m_token);
  } else {
    new (this) b1_token_ready_for_b2(m_token);
  }
}

b1_token_upside_down::b1_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("")
  // TODO: Hier in Fehlerbehandlung springen
  std::cout << "MUSS GEDREHT WERDEN!" << std::endl;
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void b1_token_upside_down::dispatched_event_button_start() {
  LOG_TRACE("")
  // TODO: Wenn Wendevorgang erfolgreich -> b1_token_ready_for_b2
}

b1_token_ready_for_b2::b1_token_ready_for_b2(token* t) : state::state(t) {
  // TODO: Pruefen ob Band 2 frei
  LOG_TRACE("")
  m_token->pretty_print();
  telegram tg(m_token);
  TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tg);
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN))->notify_death();
  new (this) anonymous_token(m_token);
}

#endif

/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/
#ifdef IS_CONVEYOR_2
// b2_receive_data
b2_receive_data::b2_receive_data(token* t) : state::state(t) {
  LOG_TRACE("")
  // Beginne mit Lauschen auf serielle Schnittstelle
  TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SERIAL_DATA);
}

void b2_receive_data::dispatched_event_serial_data() {
  LOG_TRACE("")
  serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  // Werte aus Telegramm holen
  telegram tg = serial->get_telegram();
  m_token->set_id(tg.m_id);
  m_token->set_height1(tg.m_height1);
  new (this) b2_received_object(m_token);
}

// b2_received_object
b2_received_object::b2_received_object(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
}

void b2_received_object::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_RIGHT);
  new (this) b2_realized_object(this->m_token);
}

// b2_realized_object
b2_realized_object::b2_realized_object(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_HEIGHT);
}

void b2_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_SLOW);
  // TODO: Pruefen, ob Timer fuer die langsame Strecke notwendig ist
  // 1. Define fuer Band 2 abfragen
  // m_token->set_is_upside_down(hal->get_is_upside_down()); // FIXME
  new (this) b2_height_measurement(this->m_token);
}

// b2_height_measurement
b2_height_measurement::b2_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("")
  // TODO: Define fuer Band 2 abfragen
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_FAST);

  if (0 /* hal->obj_is_upside_down() */) {
    hal->close_switch();
    new (this) b2_token_upside_down(m_token);
  } else {
    m_token->set_height2(hal->get_height_value());
    new (this) b2_valid_height(m_token);
  }
}

// b2_token_upside_down
b2_token_upside_down::b2_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_SLIDE);
}

void b2_token_upside_down::dispatched_event_sensor_slide() {
  LOG_TRACE("")
  new (this) anonymous_token(m_token);
}

// b2_valid_height
b2_valid_height::b2_valid_height(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_SWITCH);
}

void b2_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_token->set_is_metal(hal->obj_has_metal());
  new (this) b2_metal_detection(this->m_token);
}

// b2_metal_detection
b2_metal_detection::b2_metal_detection(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  if (0 /* wrong_order */) { // FIXME
    hal->set_motor(MOTOR_LEFT);
    new (this) b2_is_wrong_order(this->m_token);
  } else {
    hal->open_switch();
    new (this) b2_is_correct_order(this->m_token);
  }
}

// b2_is_wrong_order
b2_is_wrong_order::b2_is_wrong_order(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
}

void b2_is_wrong_order::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  // TODO 1. Define von Band 2 abfragen
  hal->set_motor(MOTOR_STOP);// 2. Motor von Band 2 anhalten
  // TODO: Entfernungs-Vorgang einleiten und dort Puck zu anonymous_token machen
  // new (this) err_remove_token(this->m_token);
}

// b2_is_correct_order
b2_is_correct_order::b2_is_correct_order(token* t) : state::state(t) {
  LOG_TRACE("")
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_EXIT);
}


void b2_is_correct_order::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  // TODO: 1. Define fuer Band 2 abfragen
  hal->set_motor(MOTOR_STOP);
  hal->close_switch();
  new (this) anonymous_token(m_token);
}
#endif

/******************************************************************************
 *                                ERROR FSM                                   *
 ******************************************************************************/

// class err_b1_...
