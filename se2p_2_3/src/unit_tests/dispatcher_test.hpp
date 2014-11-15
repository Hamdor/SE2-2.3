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
 private:
  int test_mapping();

  dispatch::dispatcher* m_dispatcher;
  int m_error;
};

} // namespace unit_tests
} // namespace se2

#endif // SE2_DISPATCHER_TEST_HPP
