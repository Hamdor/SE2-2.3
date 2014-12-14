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
 * @version 0.2
 *
 * Unit tests des Timers
 **/

#ifndef SE2_TIMER_TEST_HPP
#define SE2_TIMER_TEST_HPP

#include "config.h"

#include "lib/timer/timer_handler.hpp"
#include "unit_tests/abstract_test.hpp"

namespace se2 {
namespace unit_tests {

class timer_test : public abstract_test<timer_test> {

public:
  /**
   * Default Construktor
   **/
  timer_test();

  /**
   * Default Destruktor
   **/
  virtual ~timer_test();

  virtual int before_class();
  virtual int before();
  virtual int init();
  virtual int after();
  virtual int after_class();

 private:
  /**
   * One shot Test
   **/
  int test_timer_1msec();

  /**
   * Pause Test
   **/
  int test_timer_pause();

  /**
   * Continue Test
   **/
  int test_timer_continue();

  /**
   * Test zum Timer erhoehen
   **/
  int test_timer_add();

  timer::timer_handler* m_timer;
  int m_chid;
  int m_error;
};
} // namespace unit_tests
} // namespace se2
#endif // SE2_TIMER_TEST_HPP
