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
 * @file    dispatcher_test.hpp
 * @version 0.1
 *
 * Unit tests des Dispatchers
 **/

#ifndef SE2_DISPATCHER_TEST_HPP
#define SE2_DISPATCHER_TEST_HPP

#include "config.h"

#include "lib/dispatcher/dispatcher.hpp"
#include "unit_tests/abstract_test.hpp"

namespace se2 {
namespace unit_tests {

class dispatcher_test : public abstract_test<dispatcher_test> {
 public:
  /**
   * Constructor
   **/
  dispatcher_test();

  /**
   * Default Destructor
   **/
  ~dispatcher_test();

  virtual int before_class();
  virtual int before();
  virtual int init();
  virtual int after();
  virtual int after_class();

  /**
   * Werte werden fuer die Mini FSM benoetigt
   **/
  static int s_error;
  static hal::event_values s_assumed_next;
 private:
  /**
   * Test fuer das mapping von `event_values` zu `dispatcher_events`.
   * Das Mapping wird von der Funktion `dispatcher::map_from_event_values()`
   * uebernommen.
   **/
  int test_mapping();

  /**
   * Testet die Belegung im `m_functions` array von `dispatcher`.
   **/
  int test_function_address_reg();

  /**
   * Eine kleine FSM die alle Events einmal benötigt
   **/
  int test_small_fsm();

  dispatch::dispatcher* m_dispatcher;
  int m_error;
};

} // namespace unit_tests
} // namespace se2

#endif // SE2_DISPATCHER_TEST_HPP
