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
 * @file    dispatcher.cpp
 * @version 0.1
 *
 * Dispatcher
 **/

#include "lib/dispatcher/dispatcher.hpp"
#include "lib/util/logging.hpp"

using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;

dispatcher* dispatcher::instance = 0;

dispatcher::dispatcher() {
  /**
   * TODO:
   * m_functions initialisieren
   **/
}

dispatcher::~dispatcher() {
  instance = 0;
}

bool dispatcher::register_listener(transition* listener,
                                   hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(event);
  if (devent == DISPATCHED_EVENT_MAX) {
    return false;
  }
  for (size_t i = 0; i < m_max_listeners; ++i) {
    if (m_listeners[devent][i] == NULL) {
      m_listeners[devent][i] = listener;
      return true;
    }
  }
  return false;
}

bool dispatcher::unregister_listener(transition* listener,
                                     hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(event);
  if (devent == DISPATCHED_EVENT_MAX) {
    return false;
  }
  for (size_t i = 0; i < m_max_listeners; ++i) {
    if (m_listeners[devent][i] == listener) {
      m_listeners[devent][i] = NULL;
      return true;
    }
  }
  return false;
}

void dispatcher::unregister_from_all(transition* listener) {

}

void dispatcher::direct_call_event(hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(event);
  for (size_t i = 0; i < m_max_listeners; ++i) {
    if (m_listeners[devent][i] != NULL) {
      (m_listeners[devent][i]->*m_functions[devent])();
    }
  }
}

dispatcher_events dispatcher::map_from_event_values(
    event_values val) {
  switch(val) {
  case EVENT_BUTTON_START:
    return DISPATCHED_EVENT_BUTTON_START;
  case EVENT_BUTTON_STOP:
    return DISPATCHED_EVENT_BUTTON_STOP;
  case EVENT_BUTTON_RESET:
    return DISPATCHED_EVENT_BUTTON_RESET;
  case EVENT_BUTTON_E_STOP:
    return DISPATCHED_EVENT_BUTTON_E_STOP;
  case EVENT_SENSOR_ENTRANCE:
    return DISPATCHED_EVENT_SENSOR_ENTRANCE;
  case EVENT_SENSOR_HEIGHT:
    return DISPATCHED_EVENT_SENSOR_HEIGHT;
  case EVENT_SENSOR_SWITCH:
    return DISPATCHED_EVENT_SENSOR_SWITCH;
  case EVENT_SENSOR_SLIDE:
    return DISPATCHED_EVENT_SENSOR_SLIDE;
  case EVENT_SENSOR_EXIT:
    return DISPATCHED_EVENT_SENSOR_EXIT;
  case EVENT_SERIAL_DATA:
    return DISPATCHED_EVENT_SERIAL_DATA;
  case EVENT_SERIAL_MSG:
    return DISPATCHED_EVENT_SERIAL_MSG;
  case EVENT_SERIAL_ERR:
    return DISPATCHED_EVENT_SERIAL_ERR;
  case EVENT_SERIAL_UNK:
    return DISPATCHED_EVENT_SERIAL_UNK;
  case EVENT_SEG1_EXCEEDED:
    return DISPATCHED_EVENT_SEG1_EXCEEDED;
  case EVENT_SEG2_EXCEEDED:
    return DISPATCHED_EVENT_SEG2_EXCEEDED;
  case EVENT_SEG3_EXCEEDED:
    return DISPATCHED_EVENT_SEG3_EXCEEDED;
  case EVENT_SLIDE_FULL:
    return DISPATCHED_EVENT_SLIDE_FULL;
  case EVENT_OPEN_SWITCH:
    return DISPATCHED_EVENT_OPEN_SWITCH;
  case EVENT_TURN_TOKEN:
    return DISPATCHED_EVENT_TURN_TOKEN;
  case EVENT_REMOVE_TOKEN:
    return DISPATCHED_EVENT_REMOVE_TOKEN;
  case EVENT_TOKEN_FINISHED:
    return DISPATCHED_EVENT_TOKEN_FINISHED;
  default:
    LOG_ERROR("map_from_event_values() : unkown value")
    break;
  }
  return DISPATCHED_EVENT_MAX;
}

void dispatcher::initialize() {

}

void dispatcher::destroy() {

}
