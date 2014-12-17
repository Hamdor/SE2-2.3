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
 * @file    error_handler.cpp
 * @version 0.2
 *
 * Klasse fuer Fehler Zustandsautomaten
 **/

#include "lib/fsm/error_states.hpp"
#include "lib/util/singleton_mgr.hpp"

using namespace se2;
using namespace se2::hal;
using namespace se2::fsm;
using namespace se2::util;
using namespace se2::dispatch;

err_slide_full::err_slide_full(token* t) : state::state(t) {
  LOG_TRACE("")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET,1);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
  // TODO:
  // - Alleine Weg gehen registieren
}

void err_slide_full::dispatched_event_button_reset() {
  LOG_TRACE("")
  new (this) err_slide_full_quitted(m_token);
}

err_slide_full_quitted::err_slide_full_quitted(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_led_state(LED_RESET,0);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  hal->set_led_state(LED_START,1);
  // TODO:
  // - Register start button
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_slide_full_quitted::dispatched_event_button_start() {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_led_state(LED_START,0);
  if (hal->obj_in_light_barrier(SENSOR_SLIDE)) {
    // rutsche wurde nicht geleert...
    new (this) err_slide_full(m_token);
    return;
  }
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

err_token_not_removed_from_end::err_token_not_removed_from_end(token* t)
    : state::state(t) {
  LOG_TRACE("")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  // TODO:
  // - Sensitiv auf Steigende Flanke Exit => Error Gone
  // - Register exit sensor
}

err_token_not_removed_from_end_token_removed::
err_token_not_removed_from_end_token_removed(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_GONE);
  // TODO:
  // - Quittierung
}

err_token_not_removed_from_end_quitted::
err_token_not_removed_from_end_quitted(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START,1);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

err_runtime_too_long::err_runtime_too_long(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->delete_timers();
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
  // TODO: Evtl sensitiv auf alle Schranken?
}

void err_runtime_too_long::dispatched_event_button_reset() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, false);
  new (this) err_runtime_too_long_quitted(m_token);
}

err_runtime_too_long_quitted
::err_runtime_too_long_quitted(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_runtime_too_long_quitted::dispatched_event_button_start() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, false);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

err_runtime_too_short::err_runtime_too_short(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->delete_timers();
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
}

void err_runtime_too_short::dispatched_event_button_reset() {
  LOG_TRACE("")
    TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, false);
  new (this) err_runtime_too_short_quitted(m_token);
}

err_runtime_too_short_quitted::err_runtime_too_short_quitted(token* t)
    : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_runtime_too_short_quitted::dispatched_event_button_start() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, false);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

// TODO AN ALLEN SCHRANKEN ALS PREFFERED TOKEN ANMELDEN
err_unexpected_token::err_unexpected_token(token* t) : state::state(t) {
  LOG_TRACE("")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, true);
  lmgr->set_state(ERROR_NOT_RESOLVED);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
  // TODO:
  // - Register entfernen
  // - Register quittieren
}

void err_unexpected_token::dispatched_event_button_reset() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, false);
  new (this) err_unexpected_token_quitted(m_token);
}

// TODO VON ALLEN SCHRANKEN ALS PREFFERED TOKEN ABMELDEN
err_unexpected_token_quitted::err_unexpected_token_quitted(token* t)
    : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_unexpected_token_quitted::dispatched_event_button_start() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, false);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  new (this) anonymous_token(m_token);
}

/**
 * Werkstueck wurde nicht im Zeitfenster vom Band entnommen
 * (Zum drehen)
 * TOOD: - Evtl vom Start button unregistrieren?
 **/
err_token_not_removed_turnover::err_token_not_removed_turnover(token* t)
    : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, false);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
}

/**
 * Werkstueck wurde entnommen
 **/
void err_token_not_removed_turnover::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
}

/**
 * Werkstueck wurde zurueck gelegt
 **/
void err_token_not_removed_turnover::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  new (this) err_token_not_removed_turnover_fixed(m_token);
}

/**
 * Werkstueck wurde entnommen
 **/
err_token_not_removed_turnover_fixed
::err_token_not_removed_turnover_fixed(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_GONE);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
}

/**
 * Werkstueck wurde auf Band 1 gelegt und es wurde der Reset button betaetigt
 **/
void err_token_not_removed_turnover_fixed::dispatched_event_button_reset() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, false);
  new (this) err_token_not_removed_turnover_quitted(m_token);
}

/**
 * Error wurde quittiert
 **/
err_token_not_removed_turnover_quitted
::err_token_not_removed_turnover_quitted(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, true);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
}

/**
 * Start Taster wurde wieder betaetigt, Band ist wieder bereit
 **/
void err_token_not_removed_turnover_quitted::dispatched_event_button_start() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, false);
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  new (this) b1_token_ready_for_b2(m_token);
}
