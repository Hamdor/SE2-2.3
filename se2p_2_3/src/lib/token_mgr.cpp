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
 * @file    token_mgr.hpp
 * @version 0.1
 *
 * Manager fuer Token
 **/

#include "lib/token_mgr.hpp"
#include "lib/fsm/state.hpp"

#include "lib/util/singleton_mgr.hpp"
#include "lib/dispatcher/dispatcher.hpp"
#include "lib/hal/HWaccess.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;

token_mgr* token_mgr::instance = 0;

token_mgr::token_mgr() : e_stop_listener(new state)
                       , m_alife(0), m_motor_slow(0)
                       , m_motor_stop(false)
                       , m_motor_left(false)
                       , m_expected_token(ALL) {
  dispatcher* disp = TO_DISPATCHER
      (singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(e_stop_listener, EVENT_BUTTON_E_STOP);
  disp->register_listener(e_stop_listener, EVENT_BUTTON_E_STOP_R);
  // Initialisiere alle Token mit anonymous token
  for (int i = 0; i < NUM_OF_TOKENS; ++i) {
    m_tokens[i].set_state(new anonymous_token(&m_tokens[i]));
  }
}

token_mgr::~token_mgr() {
  delete e_stop_listener;
  token_mgr::instance = 0;
}

void token_mgr::initialize() {
  // nop
}

void token_mgr::destroy() {
  // nop
}

void token_mgr::update() {
  hwaccess* hal = TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN));
  if (m_alife > 0) {
    hal->set_motor(MOTOR_RIGHT);
  }
  if (m_motor_slow > 0) {
    hal->set_motor(MOTOR_SLOW);
  } else {
    hal->set_motor(MOTOR_FAST);
  }
  if (m_motor_left) {
    hal->set_motor(MOTOR_LEFT);
  } else {
    hal->set_motor(MOTOR_RIGHT);
  }
  if (m_motor_stop) {
    hal->set_motor(MOTOR_STOP);
  } else {
    hal->set_motor(MOTOR_RESUME);
  }
  if (m_alife == 0) {
    hal->set_motor(MOTOR_STOP);
  }
  if (m_alife < 0) {
    LOG_ERROR("m_alife is under 0")
  }
#if defined (IS_CONVEYOR_2)
  if (m_alife == 0) {
    // TODO: Hier muss (per Serieller Schnittstelle) eine Nachricht an Band 1
    // gesendet werden, dass auf dem Band 2 wieder Platz fuer einen Token ist.
  }
#endif
}

void token_mgr::notify_existence(bool update) {
  ++m_alife;
  if (update) {
    this->update();
  }
}

void token_mgr::notify_death(bool update) {
  --m_alife;
  if (update) {
    this->update();
  }
}

void token_mgr::request_fast_motor(bool update) {
  --m_motor_slow;
  if (update) {
    this->update();
  }
}

void token_mgr::request_slow_motor(bool update) {
  ++m_motor_slow;
  if (update) {
    this->update();
  }
}

void token_mgr::request_left_motor(bool update) {
  m_motor_left = true;
  if (update) {
    this->update();
  }
}

void token_mgr::unrequest_left_motor(bool update) {
  m_motor_left = false;
  if (update) {
    this->update();
  }
}

void token_mgr::request_stop_motor(bool update) {
  m_motor_stop = true;
  if (update) {
    this->update();
  }
}

void token_mgr::unrequest_stop_motor(bool update) {
  m_motor_stop = false;
  if (update) {
    this->update();
  }
}

void token_mgr::reregister_e_stop() {
  dispatch::dispatcher* disp = TO_DISPATCHER
      (singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(e_stop_listener, EVENT_BUTTON_E_STOP);
}

void token_mgr::reregister_e_stop_rising() {
  dispatch::dispatcher* disp = TO_DISPATCHER
      (singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(e_stop_listener, EVENT_BUTTON_E_STOP_R);
}

void token_mgr::enter_safe_state() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_safe.m_switch_open   = hal->is_switch_open();
  m_safe.m_motor_running = hal->is_motor_running();
  hal->close_switch();
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN))->pause_all();
}

void token_mgr::exit_safe_state() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  if (m_safe.m_switch_open) {
    hal->open_switch();
  } else {
    hal->close_switch();
  }
  if (m_safe.m_motor_running) {
    token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
    mgr->unrequest_stop_motor();
  }
  TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN))->continue_all();
}

bool token_mgr::check_order(bool metal) {
  if (m_expected_token == ALL) {
    m_expected_token = (metal ? PLASTIC : METAL);
    return true;
  } else if (m_expected_token == METAL) {
    if (metal) {
      m_expected_token = PLASTIC;
      return true;
    } else {
      return false;
    }
  } else if (m_expected_token == PLASTIC) {
    if (metal) {
      return false;
    } else {
      m_expected_token = METAL;
      return true;
    }
  } else {
    // Kann gar nicht passieren
    LOG_ERROR("nicht beruecksichtigte Kombination")
    return false;
  }
}
