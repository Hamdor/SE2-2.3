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
#include <cstring>

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
  m_chid = ChannelCreate(0);
  m_timer = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_timer->change_channel(m_chid);
  return 0;
}

int timer_test::before() {
  return 0;
}

int timer_test::init() {
  m_test_functions.push_back(&timer_test::test_timer_1msec);
  m_test_functions.push_back(&timer_test::test_timer_pause);
  m_test_functions.push_back(&timer_test::test_timer_continue);
  m_test_functions.push_back(&timer_test::test_timer_add);
  return 0;
}

int timer_test::after() {
  return 0;
}

int timer_test::after_class() {
  return 0;
}

int timer_test::test_timer_1msec() {
  duration time = { 0, 1 };
  int idx = m_timer->register_timer(time, 42);
  _pulse buffer;
  std::memset(&buffer, 0, sizeof(_pulse));
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != 42) {
    ++m_error;
  }
  m_timer->delete_timer(idx);
  return m_error;
}

int timer_test::test_timer_pause() {
  int timeoutmsg = 53;
  int timemsg = 42;
  duration time = { 0, 250 };
  duration timeout = { 1, 0 };
  int idx_time    = m_timer->register_timer(time, timemsg);
  int idx_timeout = m_timer->register_timer(timeout, timeoutmsg);
  m_timer->pause_timer(idx_time);
  _pulse buffer;
  std::memset(&buffer, 0, sizeof(_pulse));
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != timeoutmsg) {
    ++m_error;
  }
  m_timer->delete_timer(idx_time);
  m_timer->delete_timer(idx_timeout);
  return m_error;
}

int timer_test::test_timer_continue() {
  int timeoutmsg = 53;
  int timemsg = 42;
  duration time = { 0, 500 };
  duration timeout = { 1, 0 };
  int idx_time    = m_timer->register_timer(time, timemsg);
  int idx_timeout = m_timer->register_timer(timeout, timeoutmsg);
  m_timer->pause_timer(idx_time);
  _pulse buffer;
  std::memset(&buffer, 0, sizeof(_pulse));
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != timeoutmsg) {
    ++m_error;
  }
  m_timer->continue_timer(idx_time);
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != timemsg) {
    ++m_error;
  }
  m_timer->delete_timer(idx_time);
  m_timer->delete_timer(idx_timeout);
  return m_error;
}

int timer_test::test_timer_add() {
  int timemsg    = 42;
  int betweenmsg = 15;
  int timeoutmsg = 53;
  duration time    = { 0, 500 };
  duration timeout = { 3, 0 };
  duration between = { 1, 0 };
  int idx_first   = m_timer->register_timer(time, timemsg);
  int idx_between = m_timer->register_timer(between, betweenmsg);
  int idx_timeout = m_timer->register_timer(timeout, timeoutmsg);
  duration add = { 1, 0 };
  m_timer->add_time(idx_first, add);
  _pulse buffer;
  std::memset(&buffer, 0, sizeof(_pulse));
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != betweenmsg) {
    ++m_error;
  }
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != timemsg) {
    ++m_error;
  }
  MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
  if (buffer.value.sival_int != timeoutmsg) {
    ++m_error;
  }
  m_timer->delete_timer(idx_first);
  m_timer->delete_timer(idx_between);
  m_timer->delete_timer(idx_timeout);
  return m_error;
}

