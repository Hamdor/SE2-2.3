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

#include "lib/fsm/events.hpp"
#include "lib/util/logging.hpp"
#include "lib/dispatcher/dispatcher.hpp"

#include <cstring>

using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;

dispatcher* dispatcher::instance = 0;

void map_insert(std::map<event_values, dispatcher_events>& map,
                event_values key, dispatcher_events value) {
  map.insert(std::make_pair(key, value));
}

dispatcher::dispatcher() {
  std::memset(m_prior_listners, 0,
      sizeof(m_prior_listners[0]) * DISPATCHED_EVENT_MAX);
  m_functions[0]  = &fsm::events::dispatched_event_button_start;
  m_functions[1]  = &fsm::events::dispatched_event_button_stop;
  m_functions[2]  = &fsm::events::dispatched_event_button_reset;
  m_functions[3]  = &fsm::events::dispatched_event_button_e_stop;
  m_functions[4]  = &fsm::events::dispatched_event_button_e_stop_rising;
  m_functions[5]  = &fsm::events::dispatched_event_sensor_entrance;
  m_functions[6]  = &fsm::events::dispatched_event_sensor_entrance_rising;
  m_functions[7]  = &fsm::events::dispatched_event_sensor_height;
  m_functions[8]  = &fsm::events::dispatched_event_sensor_height_rising;
  m_functions[9]  = &fsm::events::dispatched_event_sensor_switch;
  m_functions[10] = &fsm::events::dispatched_event_sensor_switch_rising;
  m_functions[11] = &fsm::events::dispatched_event_sensor_slide;
  m_functions[12] = &fsm::events::dispatched_event_sensor_slide_rising;
  m_functions[13] = &fsm::events::dispatched_event_sensor_exit;
  m_functions[14] = &fsm::events::dispatched_event_sensor_exit_rising;
  m_functions[15] = &fsm::events::dispatched_event_serial_data;
  m_functions[16] = &fsm::events::dispatched_event_serial_msg;
  m_functions[17] = &fsm::events::dispatched_event_serial_err;
  m_functions[18] = &fsm::events::dispatched_event_serial_next_ok;
  m_functions[19] = &fsm::events::dispatched_event_serial_transfer_fin;
  m_functions[20] = &fsm::events::dispatched_event_serial_e_stopp;
  m_functions[21] = &fsm::events::dispatched_event_serial_e_stopp_gone;
  m_functions[22] = &fsm::events::dispatched_event_serial_unk;
  m_functions[23] = &fsm::events::dispatched_event_seg1_has_to_expire;
  m_functions[24] = &fsm::events::dispatched_event_seg2_has_to_expire;
  m_functions[25] = &fsm::events::dispatched_event_seg3_has_to_expire;
  m_functions[26] = &fsm::events::dispatched_event_seg1_too_late;
  m_functions[27] = &fsm::events::dispatched_event_seg2_too_late;
  m_functions[28] = &fsm::events::dispatched_event_seg3_too_late;
  m_functions[29] = &fsm::events::dispatched_event_slide_full_timeout;
  m_functions[30] = &fsm::events::dispatched_event_turn_token;
  m_functions[31] = &fsm::events::dispatched_event_remove_token;
  // Map fuer mapping von event_values => dispatcher_events fuellen
  map_insert(m_mapping, EVENT_ZERO, DISPATCHED_EVENT_MAX);
  map_insert(m_mapping, EVENT_BUTTON_START, DISPATCHED_EVENT_BUTTON_START);
  map_insert(m_mapping, EVENT_BUTTON_STOP, DISPATCHED_EVENT_BUTTON_STOP);
  map_insert(m_mapping, EVENT_BUTTON_RESET, DISPATCHED_EVENT_BUTTON_RESET);
  map_insert(m_mapping, EVENT_BUTTON_E_STOP, DISPATCHED_EVENT_BUTTON_E_STOP);
  map_insert(m_mapping, EVENT_BUTTON_E_STOP_R,
             DISPATCHED_EVENT_BUTTON_E_STOP_R);
  map_insert(m_mapping, EVENT_SENSOR_ENTRANCE,
             DISPATCHED_EVENT_SENSOR_ENTRANCE);
  map_insert(m_mapping, EVENT_SENSOR_ENTRANCE_R,
             DISPATCHED_EVENT_SENSOR_ENTRANCE_R);
  map_insert(m_mapping, EVENT_SENSOR_HEIGHT, DISPATCHED_EVENT_SENSOR_HEIGHT);
  map_insert(m_mapping, EVENT_SENSOR_HEIGHT_R,
             DISPATCHED_EVENT_SENSOR_HEIGHT_R);
  map_insert(m_mapping, EVENT_SENSOR_SWITCH, DISPATCHED_EVENT_SENSOR_SWITCH);
  map_insert(m_mapping, EVENT_SENSOR_SWITCH_R,
             DISPATCHED_EVENT_SENSOR_SWITCH_R);
  map_insert(m_mapping, EVENT_SENSOR_SLIDE, DISPATCHED_EVENT_SENSOR_SLIDE);
  map_insert(m_mapping, EVENT_SENSOR_SLIDE_R, DISPATCHED_EVENT_SENSOR_SLIDE_R);
  map_insert(m_mapping, EVENT_SENSOR_EXIT, DISPATCHED_EVENT_SENSOR_EXIT);
  map_insert(m_mapping, EVENT_SENSOR_EXIT_R, DISPATCHED_EVENT_SENSOR_EXIT_R);
  map_insert(m_mapping, EVENT_SERIAL_DATA, DISPATCHED_EVENT_SERIAL_DATA);
  map_insert(m_mapping, EVENT_SERIAL_MSG, DISPATCHED_EVENT_SERIAL_MSG);
  map_insert(m_mapping, EVENT_SERIAL_ERR, DISPATCHED_EVENT_SERIAL_ERR);
  map_insert(m_mapping, EVENT_SERIAL_NEXT_OK, DISPATCHED_EVENT_SERIAL_NEXT_OK);
  map_insert(m_mapping, EVENT_SERIAL_TRANSFER_FIN,
      DISPATCHED_EVENT_SERIAL_TRANSFER_FIN);
  map_insert(m_mapping, EVENT_SERIAL_E_STOPP, DISPATCHED_EVENT_SERIAL_E_STOPP);
  map_insert(m_mapping, EVENT_SERIAL_E_STOPP_GONE,
      DISPATCHED_EVENT_SERIAL_E_STOPP_GONE);
  map_insert(m_mapping, EVENT_SERIAL_UNK, DISPATCHED_EVENT_SERIAL_UNK);
  map_insert(m_mapping, EVENT_SEG1_HAS_TO_EXPIRE,
      DISPATCHED_EVENT_SEG1_HAS_TO_EXPIRE);
  map_insert(m_mapping, EVENT_SEG2_HAS_TO_EXPIRE,
      DISPATCHED_EVENT_SEG2_HAS_TO_EXPIRE);
  map_insert(m_mapping, EVENT_SEG3_HAS_TO_EXPIRE,
      DISPATCHED_EVENT_SEG3_HAS_TO_EXPIRE);
  map_insert(m_mapping, EVENT_SEG1_TOO_LATE, DISPATCHED_EVENT_SEG1_TOO_LATE);
  map_insert(m_mapping, EVENT_SEG2_TOO_LATE, DISPATCHED_EVENT_SEG2_TOO_LATE);
  map_insert(m_mapping, EVENT_SEG3_TOO_LATE, DISPATCHED_EVENT_SEG3_TOO_LATE);
  map_insert(m_mapping, EVENT_SLIDE_FULL_TIMEOUT,
      DISPATCHED_EVENT_SLIDE_FULL_TIMEOUT);
  map_insert(m_mapping, EVENT_TURN_TOKEN_TIMEOUT,
      DISPATCHED_EVENT_TURN_TOKEN_TIMEOUT);
  map_insert(m_mapping, EVENT_REMOVE_TOKEN_TIMEOUT,
      DISPATCHED_EVENT_REMOVE_TOKEN_TIMEOUT);
}

dispatcher::~dispatcher() {
  instance = 0;
}

bool dispatcher::register_listener(fsm::events* listener,
                                   hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(m_mapping,
      event);
  if (devent == DISPATCHED_EVENT_MAX) {
    return false;
  }
  m_listeners[devent].push(listener);
  return true;
}

bool dispatcher::register_prior_listener(fsm::events* listener,
                                         hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(m_mapping,
      event);
  if (devent == DISPATCHED_EVENT_MAX || m_prior_listners[devent] != NULL) {
    return false;
  }
  m_prior_listners[devent] = listener;
  return true;
}

bool dispatcher::unregister_prior_listener(fsm::events* listener,
                                           hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(m_mapping,
      event);
  if (devent == DISPATCHED_EVENT_MAX || m_prior_listners[devent] != listener) {
    return false;
  }
  m_prior_listners[devent] = NULL;
  return true;
}

void dispatcher::direct_call_event(hal::event_values event) {
  dispatcher_events devent = dispatcher::map_from_event_values(m_mapping,
      event);
  if (devent == DISPATCHED_EVENT_MAX) {
    return;
  }
  if (m_prior_listners[devent] != NULL) {
    (m_prior_listners[devent]->*m_functions[devent])();
    return;
  }
  if (m_listeners[devent].empty()) {
    return;
  }
  (m_listeners[devent].front()->*m_functions[devent])();
  m_listeners[devent].pop();
}

dispatcher_events dispatcher::map_from_event_values(
    const std::map<hal::event_values, dispatcher_events>& map,
    event_values val) {
  std::map<event_values, dispatcher_events>::const_iterator it = map.find(val);
  if (it != map.end()) {
    return it->second;
  }
  return DISPATCHED_EVENT_MAX;
}

void dispatcher::initialize() {
  start(NULL);
}

void dispatcher::destroy() {
  shutdown();
}

#define ISR_USED_BITS 16
#define ISR_CONCURRENT_HANDLE_START_MASK 0x01
void dispatcher::special_case_handling(const _pulse& buffer) {
  // Fuer eventuelle Spezialfaelle in den Signalen
  switch(buffer.code) {
  case INTERRUPT:
    switch(buffer.value.sival_int) {
    case EVENT_ZERO:
      break;
    case EVENT_BUTTON_START:
      break;
    case EVENT_BUTTON_STOP:
      break;
    case EVENT_BUTTON_RESET:
      break;
    case EVENT_BUTTON_E_STOP: {
      token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
      mgr->enter_safe_state();
    } break;
    case EVENT_BUTTON_E_STOP_R: {
      token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
      mgr->exit_safe_state();
    } break;
    case EVENT_SENSOR_ENTRANCE:
      break;
    case EVENT_SENSOR_ENTRANCE_R:
      break;
    case EVENT_SENSOR_HEIGHT:
      break;
    case EVENT_SENSOR_HEIGHT_R:
      break;
    case EVENT_SENSOR_SWITCH:
      break;
    case EVENT_SENSOR_SWITCH_R:
      break;
    case EVENT_SENSOR_SLIDE:
      break;
    case EVENT_SENSOR_SLIDE_R:
      break;
    case EVENT_SENSOR_EXIT:
      break;
    case EVENT_SENSOR_EXIT_R:
      break;
    case EVENT_UNKOWN1:
    case EVENT_UNKOWN2:
    case EVENT_UNKOWN3:
      break;
    default: {
      LOG_WARNING("Unkown Interrupt Value")
      // Unkown Event, diese Funktion schiebt einzele
      // Masken um zu schauen welche Events zeitgleich
      // ausgeloest wurden und fuehrt diese aus...
      int event = 0;
      int mask  = ISR_CONCURRENT_HANDLE_START_MASK;
      for (int i = 0; i < ISR_USED_BITS; ++i) {
        event = mask & buffer.value.sival_int;
        if (event) {
          direct_call_event(event);
        }
        mask <<= i;
      }
    } break;
    }
    break;
  case SERIAL:
    switch(buffer.value.sival_int) {
    case EVENT_SERIAL_DATA:
      break;
    case EVENT_SERIAL_MSG:
      break;
    case EVENT_SERIAL_ERR:
      break;
    case EVENT_SERIAL_NEXT_OK: {
      token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
      mgr->notify_ready_for_next();
    } break;
    case EVENT_SERIAL_TRANSFER_FIN:
      break;
    case EVENT_SERIAL_E_STOPP: {
      token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
      mgr->enter_safe_state(false);
    } break;
    case EVENT_SERIAL_E_STOPP_GONE: {
      token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
      mgr->exit_safe_state();
    } break;
    case EVENT_SERIAL_UNK:
      break;
    default:
      LOG_WARNING("Unkown Serial Interface Value")
      break;
    }
    break;
  case TIMER:
    switch(buffer.value.sival_int) {
    case EVENT_SEG1_HAS_TO_EXPIRE:
      break;
    case EVENT_SEG2_HAS_TO_EXPIRE:
      break;
    case EVENT_SEG3_HAS_TO_EXPIRE:
      break;
    case EVENT_SEG1_TOO_LATE:
      break;
    case EVENT_SEG2_TOO_LATE:
      break;
    case EVENT_SEG3_TOO_LATE:
      break;
    case EVENT_SLIDE_FULL_TIMEOUT:
      break;
    case EVENT_TURN_TOKEN_TIMEOUT:
      break;
    case EVENT_REMOVE_TOKEN_TIMEOUT:
      break;
    default:
      LOG_WARNING("Unkown Timer Value")
      break;
    }
    break;
  default:
    LOG_WARNING("Unkown Pulse Code")
    break;
  }
}

#define SHUTDOWN_CODE -1
#define SHUTDOWN_VAL  -1
void dispatcher::execute(void*) {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int chid = hal->get_isr_channel();
  struct _pulse buffer;
  std::memset(&buffer, 0, sizeof(buffer));
  while(!isStopped()) {
    MsgReceivePulse(chid, &buffer, sizeof(_pulse), NULL);
    if (buffer.code == SHUTDOWN_CODE
        && buffer.value.sival_int == SHUTDOWN_VAL) {
      break;
    }
    special_case_handling(buffer);
    direct_call_event(static_cast<event_values>(buffer.value.sival_int));
  }
}

void dispatcher::shutdown() {
  // Shutdown message in den ISR Pulse message channel schreiben
  // das sollte den Dispatcher entblockieren.
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int chid = hal->get_isr_channel();
  int coid = ConnectAttach(0, 0, chid, 0, 0);
  int rc = MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT,
                        SHUTDOWN_CODE, SHUTDOWN_VAL);
  if (rc == -1) {
    perror("dispatcher::shutdown()");
  }
#ifndef SIMULATION
  ConnectDetach(coid);
#endif
}
