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

#ifndef SE2_TOKEN_MGR_HPP
#define SE2_TOKEN_MGR_HPP

#include "config.h"

#include "lib/token.hpp"
#include "lib/util/abstract_singleton.hpp"

namespace se2 {
namespace util {
class singleton_mgr;
}

class token_mgr : public util::abstract_singleton {
  friend token;
  friend util::singleton_mgr;

  token_mgr();
  virtual ~token_mgr();

  virtual void initialize();
  virtual void destroy();

  static token_mgr* instance;
  token m_tokens[NUM_OF_TOKENS];
};

}

#endif // SE2_TOKEN_MGR_HPP
