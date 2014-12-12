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
  LOG_TRACE("err_slide_full::err_slide_full")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
  // TODO:
  // - Alleine Weg gehen registieren
}

void err_slide_full::dispatched_event_button_reset() {
  LOG_TRACE("err_slide_full::dispatched_event_button_reset")
  new (this) err_slide_full_quitted(m_token);
}

err_slide_full_quitted::err_slide_full_quitted(token* t) : state::state(t) {
  LOG_TRACE("err_slide_full_quitted::err_slide_full_quitted")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  // TODO:
  // - Register start button
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_slide_full_quitted::dispatched_event_button_start() {
  LOG_TRACE("err_slide_full_quitted::dispatched_event_button_start")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  new (this) anonymous_token(m_token);
}

err_token_not_removed_from_end::err_token_not_removed_from_end(token* t)
    : state::state(t) {
  LOG_TRACE("err_token_not_removed_from_end::err_token_not_removed_from_end")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  // TODO:
  // - Sensitiv auf Steigende Flanke Exit => Error Gone
  // - Register exit sensor
}

err_token_not_removed_from_end_token_removed::
err_token_not_removed_from_end_token_removed(token* t) : state::state(t) {
  LOG_TRACE("err_token_not_removed_from_end_token_removed::\
    err_token_not_removed_from_end_token_removed")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_GONE);
  // TODO:
  // - Quittierung
}

err_token_not_removed_from_end_quitted::
err_token_not_removed_from_end_quitted(token* t) : state::state(t) {
  LOG_TRACE("err_token_not_removed_from_end_quitted::err_token_not_removed_from_end_quitted")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

err_runtime_too_long::err_runtime_too_long(token* t) : state::state(t) {
  LOG_TRACE("err_runtime_too_long::err_runtime_too_long")
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
  LOG_TRACE("err_runtime_too_long::dispatched_event_button_reset")
  new (this) err_runtime_too_long_quitted(m_token);
}

err_runtime_too_long_quitted
::err_runtime_too_long_quitted(token* t) : state::state(t) {
  LOG_TRACE("err_runtime_too_long_quitted::err_runtime_too_long_quitted")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_runtime_too_long_quitted::dispatched_event_button_start() {
  LOG_TRACE("err_runtime_too_long_quitted::dispatched_event_button_start")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

err_runtime_too_short::err_runtime_too_short(token* t) : state::state(t) {
  LOG_TRACE("err_runtime_too_short::err_runtime_too_short")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_NOT_RESOLVED);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_RESET);
  // TODO:
  // - Register entfernen
}

void err_runtime_too_short::dispatched_event_button_reset() {
  LOG_TRACE("err_runtime_too_short::dispatched_event_button_reset")
  new (this) err_runtime_too_short_quitted(m_token);
}

err_runtime_too_short_quitted::err_runtime_too_short_quitted(token* t)
    : state::state(t) {
  LOG_TRACE("err_runtime_too_short_quitted::err_runtime_too_short_quitted")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_runtime_too_short_quitted::dispatched_event_button_start() {
  LOG_TRACE("err_runtime_too_short_quitted::dispatched_event_button_start")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  new (this) anonymous_token(m_token);
}

// TODO AN ALLEN SCHRANKEN ALS PREFFERED TOKEN ANMELDEN
err_unexpected_token::err_unexpected_token(token* t) : state::state(t) {
  LOG_TRACE("err_unexpected_token::err_unexpected_token")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
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
  LOG_TRACE("err_unexpected_token::dispatched_event_button_reset")
  new (this) err_unexpected_token_quitted(m_token);
}

// TODO VON ALLEN SCHRANKEN ALS PREFFERED TOKEN ABMELDEN
err_unexpected_token_quitted::err_unexpected_token_quitted(token* t) : state::state(t) {
  LOG_TRACE("err_unexpected_token_quitted::err_unexpected_token_quitted")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(ERROR_RESOLVED);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
}

void err_unexpected_token_quitted::dispatched_event_button_start() {
  LOG_TRACE("err_unexpected_token_quitted::dispatched_event_button_start")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  new (this) anonymous_token(m_token);
}
