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
 * @file    serial_test.cpp
 * @version 0.1
 *
 * Unit tests der seriellen Schnittstelle
 **/

#include "unit_tests/serial_test.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/constants.hpp"

using namespace se2::util;
using namespace se2::unit_tests;
using namespace se2::serial_bus;

serial_test::serial_test() : m_serial(0), m_error(0) {
  // nop
}

serial_test::~serial_test() {
  // nop
}

int serial_test::before_class() {
  m_serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  return 0;
}

int serial_test::before() {
  return 0;
}

int serial_test::init() {
  m_test_functions.push_back(&serial_test::test_serial_channel);
  return 0;
}

int serial_test::after() {
  return 0;
}

int serial_test::after_class() {
  return 0;
}

int serial_test::test_serial_channel() {
  token tok;
  tok.m_id    = 1;
  tok.m_wert1 = 2;
  tok.m_wert2 = 3;
  telegram tel;
  tel.m_type = DATA;
  tel.m_msg  = NOTHING;
  tel.m_data = tok;
#ifdef UNIT_TEST_SENDER
  m_serial->send_telegram(tel);
#else
  telegram recieved = m_serial->get_telegram();
  if (recieved.m_type != tel.m_type) {
    ++m_error;
  }
  if (recieved.m_msg != tel.m_msg) {
    ++m_error;
  }
  if (recieved.m_data.m_id != tel.m_data.m_id) {
    ++m_error;
  }
  if (recieved.m_data.m_wert1 != tel.m_data.m_wert1) {
    ++m_error;
  }
  if (recieved.m_data.m_wert2 != tel.m_data.m_wert2) {
    ++m_error;
  }
#endif
  return m_error;


}
