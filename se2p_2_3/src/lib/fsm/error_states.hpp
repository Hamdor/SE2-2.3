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

  virtual void dispatched_event_button_reset();
};

class err_slide_full_quitted : public fsm::state {
 public:
  err_slide_full_quitted(token* t);
  virtual ~err_slide_full_quitted() {
    // nop
  }

  virtual void dispatched_event_button_start();
};

class err_token_not_removed_from_end : public fsm::state {
 public:
  err_token_not_removed_from_end(token* t);
  virtual ~err_token_not_removed_from_end() {
    // nop
  }
};

class err_token_not_removed_from_end_token_removed : public fsm::state {
 public:
  err_token_not_removed_from_end_token_removed(token* t);
  virtual ~err_token_not_removed_from_end_token_removed() {
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

  virtual void dispatched_event_button_reset();
};

class err_runtime_too_long_quitted : public fsm::state {
 public:
  err_runtime_too_long_quitted(token* t);
  virtual ~err_runtime_too_long_quitted() {
    // nop
  }

  virtual void dispatched_event_button_start();
};

class err_runtime_too_short : public fsm::state {
 public:
  err_runtime_too_short(token* t);
  virtual ~err_runtime_too_short() {
    // nop
  }

  virtual void dispatched_event_button_reset();
};

class err_runtime_too_short_quitted : public fsm::state {
 public:
  err_runtime_too_short_quitted(token* t);
  virtual ~err_runtime_too_short_quitted() {
    // nop
  }

  virtual void dispatched_event_button_start();
};

class err_unexpected_token : public fsm::state {
 public:
  err_unexpected_token(token* t);
  virtual ~err_unexpected_token() {
    // nop
  }

  virtual void dispatched_event_button_reset();
};

class err_unexpected_token_quitted : public fsm::state {
 public:
  err_unexpected_token_quitted(token* t);
  virtual ~err_unexpected_token_quitted() {
    // nop
  }

  virtual void dispatched_event_button_start();
};

} // namespace se2

#endif // SE2_ERROR_HANDLER_HPP
