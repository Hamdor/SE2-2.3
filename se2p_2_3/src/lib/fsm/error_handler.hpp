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
 * @file    error_handler.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse fuer Error FSM
 **/

#ifndef ERROR_HANDLER_HPP_
#define ERROR_HANDLER_HPP_

#include "state.hpp"

namespace se2 {
namespace fsm {

class error : public state {
 public:
  virtual ~error() { }

  /**
   * Wechselt in den naechsten Zustand der Error FSM
   **/
  virtual void next_state();
 private:
  // Choice point
  // History to normal state
};


class err_slide_full : public error { // success: recover to b1_end_slide_full
 public:                              // success: recover to b2_end_slide_full
  void next_state();
};

class err_slide_full_quitted : public error {
 public:
  void next_state();
};

class err_slide_full_unquitted : public error {
 public:
  void next_state();
};

class err_token_not_turned : public error {
 public:
  void next_state();
};

class err_token_needs_to_be_removed : public error { // success: recover b2_waiting_for_quitting_and_remove
 public:
  void next_state();
};

class err_finished_token_not_removed : public error {
 public:
  void next_state();
};

class err_runtime_too_long : public error {
 public:
  void next_state();
};

class err_runtime_too_long_quitted : public error {
 public:
  void next_state();
};

class err_runtime_too_short : public error {
 public:
  void next_state();
};

class err_runtime_too_short_quitted : public error {
 public:
  void next_state();
};


} // namespace fsm
} // namespace se2

#endif /* ERROR_HANDLER_HPP_ */
