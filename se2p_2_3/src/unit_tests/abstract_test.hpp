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
 * @file    abstract_test.hpp
 * @version 0.1
 *
 * Interface für Tests
 **/

#ifndef SE2_ABSTRACT_TEST_HPP
#define SE2_ABSTRACT_TEST_HPP

#include "config.h"

#include <vector>

namespace se2 {
namespace unit_tests {

template <typename T>
class abstract_test {
 public:
  /**
   * Default Destruktor
   */
  virtual ~abstract_test() {
    // nop
  }

  /**
   * Wird einmalig für alle ausgeführt
   * @return 0 Wenn erfolgreich
   */
  virtual int before_class() = 0;

  /**
   * Funktion wird vor jedem Test ausgeführt
   * @return 0 Wenn erfolgreich
   */
  virtual int before() = 0;

  /**
   * Initialisiert den Funktionsvektor
   * @return 0 Wenn erfolgreich
   */
  virtual int init() = 0;

  /**
   * Aufräumen der Unit Tests
   * @return 0 Wenn erfolgreich
   */
  virtual int after() = 0;

  /**
   * Aufräumen der Unit Tests
   * @return 0 Wenn erfolgreich
   */
  virtual int after_class() = 0;

  std::vector<int (T::*)(void)> m_test_functions;
};

} // unit_tests
} // se2

#endif // SE2_ABSTRACT_TEST_HPP
