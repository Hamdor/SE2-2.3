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
 * @version 0.2
 *
 * Interface/Abstrakte Klasse fuer Fehler Zustandsautomaten
 **/

#ifndef SE2_ERROR_HANDLER_HPP
#define SE2_ERROR_HANDLER_HPP

#include "lib/fsm/state.hpp"

namespace se2 {

class err_slide_full : public fsm::state {
 public:
  err_slide_full(token* t);
  virtual ~err_slide_full() {
    // nop
  }
};

class err_slide_full_quitted : public fsm::state {
 public:
  err_slide_full_quitted(token* t);
  virtual ~err_slide_full_quitted() {
    // nop
  }
};

class err_token_not_removed_from_end : public fsm::state {
 public:
  err_token_not_removed_from_end(token* t);
  virtual ~err_token_not_removed_from_end() {
    // nop
  }
};

class err_token_not_removed_from_end_quitted : public fsm::state {
 public:
  err_token_not_removed_from_end_quitted(token* t);
  virtual ~err_token_not_removed_from_end_quitted() {
    // nop
  }
};

class err_runtime_too_long : public fsm::state {
 public:
  err_runtime_too_long(token* t);
  virtual ~err_runtime_too_long() {
    // nop
  }
};

class err_runtime_too_long_quitted : public fsm::state {
 public:
  err_runtime_too_long_quitted(token* t);
  virtual ~err_runtime_too_long_quitted() {
    // nop
  }
};

class err_runtime_too_short : public fsm::state {
 public:
  err_runtime_too_short(token* t);
  virtual ~err_runtime_too_short() {
    // nop
  }
};

class err_runtime_too_short_quitted : public fsm::state {
 public:
  err_runtime_too_short_quitted(token* t);
  virtual ~err_runtime_too_short_quitted() {
    // nop
  }
};

} // namespace se2

#endif // SE2_ERROR_HANDLER_HPP
