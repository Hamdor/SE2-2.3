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
 * @file    serial_channel.cpp
 * @version 0.1
 *
 * Logik Ebene der Seriellen Schnittstelle
 **/

#include "lib/serial_bus/serial_channel.hpp"
#include "lib/serial_bus/serial_interface.hpp"
#include "lib/util/lock_guard.hpp"

using namespace se2::serial_bus;
using namespace se2::util;

serial_channel* serial_channel::instance = 0;

serial_channel::serial_channel() : m_interface(new serial_interface) {
  // nop
}

serial_channel::~serial_channel() {
  delete m_interface;
  instance = 0;
}

void serial_channel::initialize() {
  // nop
}

void serial_channel::destroy() {
  // nop
}

void serial_channel::execute(void*) {
  telegram data;
  while(1) { /* hier abbruch bedingung vom haw thread benutzen */
    m_interface->read(&data);
    m_queue.push(data);
  }
}
