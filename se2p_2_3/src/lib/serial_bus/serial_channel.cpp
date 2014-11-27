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

#include "lib/hal/HWaccess.hpp"
#include "lib/util/lock_guard.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/serial_bus/serial_channel.hpp"
#include "lib/serial_bus/serial_interface.hpp"

#include <cstring>
#include <sys/neutrino.h>

using namespace se2::hal;
using namespace se2::util;
using namespace se2::serial_bus;

serial_channel* serial_channel::instance = 0;

serial_channel::serial_channel()
    : m_interface(new serial_interface)
    , m_queue()
    , m_lock()
    , m_cond(m_lock) {
  // nop
}

serial_channel::~serial_channel() {
  delete m_interface;
  instance = 0;
}

telegram serial_channel::get_telegram() {
  lock_guard lock(m_lock);
  while(!m_queue.size()) {
    m_cond.wait();
  }
  telegram result = m_queue.front();
  m_queue.pop();
  return result;
}

void serial_channel::initialize() {
  start(0); // startet thread
}

void serial_channel::destroy() {
  // nop
}

void serial_channel::execute(void*) {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int coid = ConnectAttach(0, 0, hal->get_isr_channel(), 0, 0);
  telegram data;
  while(!isStopped()) {
    std::memset(&data, 0, sizeof(telegram));
    m_interface->read(&data);
    event_values value;
    if (data.m_type == MSG) {
      value = EVENT_SERIAL_MSG;
    } else if (data.m_type == DATA) {
      value = EVENT_SERIAL_DATA;
    } else if (data.m_type == ERR) {
      value = EVENT_SERIAL_ERR;
    } else {
      // unkown ...
      value = EVENT_SERIAL_UNK;
    }
    {
      lock_guard lock(m_lock);
      m_queue.push(data);
      m_cond.broadcast();
    }
    MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, value);
  }
#ifndef SIMULATION
  ConnectDetach(coid);
#endif
}

void serial_channel::shutdown() {
  stop();
}

void serial_channel::send_telegram(telegram* tel) {
  m_interface->write(tel);
}
