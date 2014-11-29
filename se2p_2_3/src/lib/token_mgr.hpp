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
namespace fsm {
class state;
}
namespace util {
class singleton_mgr;
}
class token_mgr : public util::abstract_singleton {
  friend token;
  friend util::singleton_mgr;

  /**
   * `safe_state` Speichert alle wichtigen Zustaende
   * der Maschine zum zeitpunkt des drucks auf den E-Stop.
   **/
  struct safe_state {
    bool m_switch_open;
    bool m_motor_running;
    bool m_motor_slow;
    bool m_motor_left;
  };

  token_mgr();
  virtual ~token_mgr();

  virtual void initialize();
  virtual void destroy();

  void update();

 public:
  /**
   * Mit dieser Funktion meldet sich der neue `token`
   * einmal bei dem `token_mgr` an.
   **/
  void notify_exsistens();

  /**
   * Mit dieser Funktion meldet sich der `token`
   * vom `token_mgr` ab.
   **/
  void notify_death();

  void request_fast_motor();
  void request_slow_motor();

  void request_stop_motor();
  void unrequest_stop_motor();

  void reregister_e_stop();
  void reregister_e_stop_rising();

  void enter_safe_state();
  void exit_safe_state();

 private:
  static token_mgr* instance;
  token m_tokens[NUM_OF_TOKENS];
  /**
   * Dieser Zustand ist permanent und wartet nur auf den E-Stop
   **/
  fsm::state* e_stop_listener;

  int  m_alife;
  int  m_motor_slow;
  bool m_motor_stop;
  safe_state m_safe;
};

}

#endif // SE2_TOKEN_MGR_HPP
