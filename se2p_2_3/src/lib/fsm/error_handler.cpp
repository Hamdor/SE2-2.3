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
 * @file    error_handler.cpp
 * @version 0.2
 *
 * Klasse fuer Fehler Zustandsautomaten
 **/

#include "lib/fsm/error_handler.hpp"

using namespace se2;
using namespace se2::fsm;

err_slide_full::err_slide_full(token* t) : state::state(t) {
  // nop
}

err_slide_full_quitted::err_slide_full_quitted(token* t) : state::state(t) {
  // nop
}

err_token_not_removed_from_end::err_token_not_removed_from_end(token* t)
    : state::state(t) {
  // nop
}

err_token_not_removed_from_end_quitted::
  err_token_not_removed_from_end_quitted(token* t) : state::state(t) {
  // nop
}

err_runtime_too_long::err_runtime_too_long(token* t) : state::state(t) {
  // nop
}

err_runtime_too_long_quitted
  ::err_runtime_too_long_quitted(token* t) : state::state(t) {
  // nop
}

err_runtime_too_short::err_runtime_too_short(token* t) : state::state(t) {
  // nop
}

err_runtime_too_short_quitted::err_runtime_too_short_quitted(token* t)
    : state::state(t) {
  // nop
}
