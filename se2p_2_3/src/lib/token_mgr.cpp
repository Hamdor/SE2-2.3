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
#include "lib/hal/HWaccess.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;

token_mgr* token_mgr::instance = 0;

token_mgr::token_mgr() : m_alife(0), m_motor_slow(0)
                       , m_motor_stop(false) {
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
  if (m_alife > 0) {
    TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_RIGHT);
  }
  if (m_motor_slow > 0) {
    TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_SLOW);
  } else {
    TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_FAST);
  }
  /*if (m_motor_stop) {
    TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_STOP);
  } else {
    TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_RIGHT);
  }*/
  if (m_alife == 0) {
    TO_HAL(util::singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_STOP);
  }
  if (m_alife < 0) {
    LOG_ERROR("m_alife is under 0")
  }
}

void token_mgr::notify_exsistens() {
  ++m_alife;
  update();
}

void token_mgr::notify_death() {
  --m_alife;
  update();
}

void token_mgr::request_fast_motor() {
  --m_motor_slow;
  update();
}

void token_mgr::request_slow_motor() {
  ++m_motor_slow;
  update();
}

void token_mgr::request_stop_motor() {
  m_motor_stop = true;
  update();
}

void token_mgr::unrequest_stop_motor() {
  m_motor_stop = false;
  update();
}
