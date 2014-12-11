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
#include "lib/fsm/error_states.hpp"

#include "lib/util/singleton_mgr.hpp"
#include "lib/dispatcher/dispatcher.hpp"
#include "lib/hal/HWaccess.hpp"
#include "lib/serial_bus/serial_channel.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::serial_bus;

token_mgr* token_mgr::instance = 0;

token_mgr::token_mgr() : m_pseudo_token()
                       , m_alife(0)
                       , m_motor_slow(0)
                       , m_switch_open(0)
                       , m_wait_turnover(0)
                       , m_motor_stop(0)
                       , m_motor_left(false)
                       , m_expected_token(ALL)
                       , m_is_b2_ready(true)
                       , m_safe_state(false) {
  // Initialisiere alle Token mit anonymous token
  for (int i = 0; i < NUM_OF_TOKENS; ++i) {
    m_tokens[i].set_state(new anonymous_token(&m_tokens[i]));
  }
}

token_mgr::~token_mgr() {
  token_mgr::instance = 0;
}

void token_mgr::initialize() {
  // nop
}

void token_mgr::destroy() {
  // nop
}

void token_mgr::update() {
  if (m_safe_state) {
    return;
  }
  hwaccess* hal = TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN));
  if (m_alife > 0) {
    hal->set_motor(MOTOR_RIGHT);
  }
  if (m_motor_slow > 0) {
    hal->set_motor(MOTOR_SLOW);
  } else {
    hal->set_motor(MOTOR_FAST);
  }
  if (m_switch_open > 0) {
    hal->open_switch();
  } else {
    hal->close_switch();
  }
  if (m_motor_left) {
    hal->set_motor(MOTOR_LEFT);
  } else {
    hal->set_motor(MOTOR_RIGHT);
  }
  if (m_motor_stop > 0) {
    hal->set_motor(MOTOR_STOP);
    TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN))->pause_all();
  } else {
    if (m_wait_turnover == 0) {
      hal->set_motor(MOTOR_RESUME);
      TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN))->continue_all();
    }
  }
  if (m_alife == 0) {
    hal->set_motor(MOTOR_STOP);
#if defined(IS_CONVEYOR_2)
    send_free();
#endif
  }
  if (m_alife < 0) {
    LOG_ERROR("m_alife is under 0")
  }
}

void token_mgr::send_free() {
  serial_channel* serial =
      TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  telegram tel(B2_FREE);
  serial->send_telegram(&tel);
}

void token_mgr::notify_existence(bool update) {
  ++m_alife;
  if (update) {
    this->update();
  }
#ifdef IS_CONVEYOR_2
  // Band 1 signalisieren das uebergabe komplett
  serial_channel* serial =
      TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  telegram tel(B2_TRANS_FIN);
  serial->send_telegram(&tel);
#endif
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
  ++m_motor_stop;
  if (update) {
    this->update();
  }
}

void token_mgr::unrequest_stop_motor(bool update) {
  --m_motor_stop;
  if (update) {
    this->update();
  }
}

void token_mgr::request_open_switch(bool update) {
  ++m_switch_open;
  if (update) {
    this->update();
  }
}

void token_mgr::unrequest_open_switch(bool update) {
  --m_switch_open;
  if (update) {
    this->update();
  }
}

void token_mgr::request_turnover(bool update) {
  ++m_wait_turnover;
  if (update) {
    this->update();
  }
}

void token_mgr::unrequest_turnover(bool update) {
  --m_wait_turnover;
  if (update) {
    this->update();
  }
}

void token_mgr::enter_safe_state(bool send_serial) {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_safe.m_switch_open   = hal->is_switch_open();
  m_safe.m_motor_running = hal->is_motor_running();
  hal->close_switch();
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  m_safe_state = true;
  TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN))->pause_all();
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(NO_LIGHTS);
  if (send_serial) {
    telegram tel(E_STOP);
    TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tel);
  }
  singleton_mgr::shutdown();
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

bool token_mgr::check_conveyor2_ready() const {
  return m_is_b2_ready;
}

void token_mgr::notify_token_trasition() {
  m_is_b2_ready = false;
}

void token_mgr::notify_ready_for_next() {
  m_is_b2_ready = true;
}

void token_mgr::notify_pseudo_token_not_known_event() {
  m_pseudo_token.set_state(new err_unexpected_token(&m_pseudo_token));
}
