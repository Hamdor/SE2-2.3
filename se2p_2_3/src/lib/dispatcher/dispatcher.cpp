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
 * @file    dispatcher.cpp
 * @version 0.1
 *
 * Dispatcher
 **/

#include "lib/dispatcher/dispatcher.hpp"

using namespace se2::dispatch;

using se2::hal::event_values;

dispatcher* dispatcher::instance = 0;

dispatcher::dispatcher() {
  // nop
}

dispatcher::~dispatcher() {
  instance = 0;
}

bool dispatcher::register_listener(void* listener, hal::event_values event) {
  return false;
}

bool dispatcher::unregister_listener(void* listener, hal::event_values event) {
  return false;
}

void dispatcher::unregister_from_all(void* listener) {

}

void dispatcher::direct_call_event(hal::event_values event) {

}

dispatcher_events dispatcher::map_from_event_values(event_values val) {
  return DISPATCHED_EVENT_MAX;
}

void dispatcher::initialize() {

}

void dispatcher::destroy() {

}
