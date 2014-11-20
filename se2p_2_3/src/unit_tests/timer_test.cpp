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
 * @file    timer_test.hpp
 * @version 0.1
 *
 * Unit tests des Timers
 **/

#include "config.h"

#include "lib/timer/timer_handler.hpp"
#include "unit_tests/abstract_test.hpp"
#include "unit_tests/timer_test.hpp"
#include "lib/util/singleton_mgr.hpp"
#include <unistd.h>

using namespace se2::timer;
using namespace se2::util;
using namespace se2::unit_tests;

timer_test::timer_test() : m_timer(0), m_chid(0), m_error(0) {
  // nop
}

timer_test::~timer_test() {
  // nop
}

int timer_test::before_class() {

  return 0;
}

int timer_test::before() {
  m_chid = ChannelCreate(0);
  m_timer = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_timer->change_channel(m_chid);
  return 0;
}

int timer_test::init() {
  m_test_functions.push_back(&timer_test::test_timer_1msec);
  m_test_functions.push_back(&timer_test::test_timer_intervall);
  m_test_functions.push_back(&timer_test::test_timer_pause);
  m_test_functions.push_back(&timer_test::test_timer_continue);
  m_test_functions.push_back(&timer_test::test_timer_add);
  return 0;
}

int timer_test::after() {
  singleton_mgr::shutdown();
  return 0;
}

int timer_test::after_class() {
  return 0;
}

int timer_test::test_timer_1msec() {
  // ---->>One Shot<<----
  duration time;
  time.sec = 0;
  time.msec = 1;
  m_timer->register_timer(time, 42);
  struct _pulse buffer;
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != 42) {
    m_error++;
  }
  return m_error;
}

int timer_test::test_timer_intervall() {
  duration time;
  time.sec = 0;
  time.msec = 1;
  duration interval;
  interval.sec = 1;
  interval.msec = 0;
  m_timer->register_timer(time, interval, 42);
  struct _pulse buffer;
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != 42) {
    m_error++;
  }
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != 42) {
    m_error++;
  }
  return m_error;
}

int timer_test::test_timer_pause() {
  int timeoutmsg = 53;
  int timemsg = 42;
  duration time;
  time.sec = 0;
  time.msec = 1;
  duration timeout;
  timeout.sec = 5;
  timeout.msec = 0;
  int index = m_timer->register_timer(time, timeoutmsg);
  m_timer->register_timer(timeout, timemsg);
  m_timer->pause_timer(index);
  sleep(2);
  struct _pulse buffer;
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != timeoutmsg) {
    m_error++;
  }
  return m_error;
}

int timer_test::test_timer_continue() {
  int timeoutmsg = 53;
  int timemsg = 42;
  duration time;
  time.sec = 0;
  time.msec = 1;
  duration timeout;
  timeout.sec = 10;
  timeout.msec = 0;
  int index = m_timer->register_timer(time, timemsg);
  m_timer->register_timer(timeout, timeoutmsg);
  m_timer->pause_timer(index);
  sleep(2);
  m_timer->continue_timer(index);
  struct _pulse buffer;
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int == timeoutmsg) {
    m_error++;
  }
  return m_error;
}

int timer_test::test_timer_add() {
  int timeoutmsg = 53;
  int timemsg = 42;
  duration time;
  time.sec = 0;
  time.msec = 1;
  duration add;
  add.sec = 1;
  add.msec = 0;
  duration timeout;
  timeout.sec = 2;
  timeout.msec = 0;
  int index = m_timer->register_timer(time, timemsg);
  m_timer->register_timer(timeout, timeoutmsg);
  m_timer->add_time(index, add);
  struct _pulse buffer;
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int == timeoutmsg) {
    m_error++;
  }
  return m_error;
}

