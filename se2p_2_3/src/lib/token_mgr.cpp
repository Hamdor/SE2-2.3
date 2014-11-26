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

using namespace se2;
using namespace se2::fsm;

token_mgr* token_mgr::instance = 0;

token_mgr::token_mgr() {
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
