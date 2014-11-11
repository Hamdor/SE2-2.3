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

#include <sys/neutrino.h>

using namespace se2::hal;
using namespace se2::util;
using namespace se2::serial_bus;

serial_channel* serial_channel::instance = 0;

serial_channel::serial_channel() : m_interface(new serial_interface) {
  // nop
}

serial_channel::~serial_channel() {
  delete m_interface;
  instance = 0;
}

telegram serial_channel::get_telegram() {
  while(!m_queue.size()) {
    // TODO: Mit Condition Variable implementieren
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
  telegram data;
  while(!isStopped()) {
    m_interface->read(&data.m_header);
    event_values value;
    if (data.m_header.m_type == MSG) {
      value = NEW_SERIAL_MSG;
    } else if (data.m_header.m_type == DATA) {
      value = NEW_SERIAL_DATA;
      m_interface->read(&data.m_id);
      m_interface->read(&data.m_height1);
      m_interface->read(&data.m_height2);
    } else if (data.m_header.m_type == ERR) {
      value = NEW_SERIAL_ERR;
    } else {
      // unkown ...
      value = NEW_SERIAL_UNK;
    }
    m_queue.push(data);
    MsgSendPulse(hal->get_isr_channel(), 0, SERIAL, value);
  }
}

void serial_channel::shutdown() {
  stop();
}

void serial_channel::send_telegram(telegram& tel) {
  m_interface->write(&tel.m_header);
  m_interface->write(&tel.m_id);
  m_interface->write(&tel.m_height1);
  m_interface->write(&tel.m_height2);
}
