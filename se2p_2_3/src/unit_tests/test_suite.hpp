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
 * @file    test_suite.hpp
 * @version 0.1
 *
 * Laufzeitumgebung für tests
 **/

#ifndef SE2_TEST_SUITE_HPP
#define SE2_TEST_SUITE_HPP

#include <vector>
#include <cstddef>
#include "unit_tests/abstract_test.hpp"

namespace se2 {
namespace unit_tests {

template<typename T>
class test_suite {
 public:
  /**
   * Konstruktor
   **/
  test_suite() : m_test() {
    m_test.init();
    m_test.beforeClass();
    m_error = 0;
  }

  /**
   * Default Destruktor
   **/
  ~test_suite() {
    m_test.afterClass();
  }

  /**
   * Startet alle registrierten Tests
   * @return Anzahl der fehlgeschlagenen Tests
   **/
  int run() {
    for (size_t i = 0; i < m_test.m_test_functions.size(); ++i) {
      m_test.before();
      m_error += (m_test.*(m_test.m_test_functions[i]))();
      m_test.after();
    }
    return m_error;
  }

 protected:
  T m_test;
 private:
  int m_error;
};

} // namespace unit_tests
} // namespace se2

#endif // SE2_TEST_SUITE_HPP
