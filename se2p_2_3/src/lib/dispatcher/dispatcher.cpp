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
#include "lib/fsm/events.hpp"

using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;

dispatcher* dispatcher::instance = 0;

dispatcher::dispatcher() {
  m_functions[0]  = &fsm::events::dispatched_event_button_start;
  m_functions[1]  = &fsm::events::dispatched_event_button_stop;
  m_functions[2]  = &fsm::events::dispatched_event_button_reset;
  m_functions[3]  = &fsm::events::dispatched_event_button_e_stop;
  m_functions[4]  = &fsm::events::dispatched_event_sensor_entrance;
  m_functions[5]  = &fsm::events::dispatched_event_sensor_height;
  m_functions[6]  = &fsm::events::dispatched_event_sensor_switch;
  m_functions[7]  = &fsm::events::dispatched_event_sensor_slide;
  m_functions[8]  = &fsm::events::dispatched_event_sensor_exit;
  m_functions[9]  = &fsm::events::dispatched_event_serial_data;
  m_functions[10] = &fsm::events::dispatched_event_serial_msg;
  m_functions[11] = &fsm::events::dispatched_event_serial_err;
  m_functions[12] = &fsm::events::dispatched_event_serial_unk;
  m_functions[13] = &fsm::events::dispatched_event_seg1_exceeded;
  m_functions[14] = &fsm::events::dispatched_event_seg2_exceeded;
  m_functions[15] = &fsm::events::dispatched_event_seg3_exceeded;
  m_functions[16] = &fsm::events::dispatched_event_slide_full;
  m_functions[17] = &fsm::events::dispatched_event_open_switch;
  m_functions[18] = &fsm::events::dispatched_event_turn_token;
  m_functions[19] = &fsm::events::dispatched_event_remove_token;
  m_functions[20] = &fsm::events::dispatched_event_token_finished;
}

dispatcher::~dispatcher() {
  instance = 0;
}

bool dispatcher::register_listener(fsm::events* listener,
                                   hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(event);
  if (devent == DISPATCHED_EVENT_MAX) {
    return false;
  }
  m_listeners[devent].push(listener);
  return true;
}

void dispatcher::direct_call_event(hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(event);
  if (devent == DISPATCHED_EVENT_MAX) {
    return;
  }
  if (m_listeners[devent].empty()) {
    return;
  }
  (m_listeners[devent].front()->*m_functions[devent])();
  m_listeners[devent].pop();
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
  start(NULL);
}

void dispatcher::destroy() {
  shutdown();
}

void dispatcher::execute(void*) {
  int chid = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->get_isr_channel();
  struct _pulse buffer;
  while(!isStopped()) {
    MsgReceivePulse(chid, &buffer, sizeof(_pulse), NULL);
  }
}

void dispatcher::shutdown() {
  // Shutdown message in den ISR Pulse message channel schreiben
  // das sollte den Dispatcher entblockieren.
  int chid = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->get_isr_channel();
  int coid = ConnectAttach(0, 0, chid, 0, 0);
  int rc = MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
  if (rc == -1) {
    perror("dispatcher::shutdown()");
  }
  ConnectDetach(coid);
}
