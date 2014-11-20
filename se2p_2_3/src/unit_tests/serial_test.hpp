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
 * @file    serial_test.hpp
 * @version 0.1
 *
 * Unit tests der seriellen Schnittstelle
 **/

#ifndef SE2_SERIAL_TEST_HPP
#define SE2_SERIAL_TEST_HPP

#include "config.h"

#include "lib/serial_bus/serial_channel.hpp"
#include "unit_tests/abstract_test.hpp"

namespace se2 {
namespace unit_tests {

class serial_test : public abstract_test<serial_test> {
 public:
  /**
   * Constructor
   **/
  serial_test();

  /**
   * Destructor
   **/
  virtual ~serial_test();

  /**
   * Wird einmalig für alle ausgeführt
   * @return 0 wenn erfolgreich
   */
  virtual int before_class();

  /**
   * Funktion wird vor jedem test ausgeführt
   * @return 0 wenn erfolgreich
   */
  virtual int before();

  /**
   * Initialisiert den Funktionsvektor
   * @return 0 wenn erfolgreich
   */
  virtual int init();

  /**
   * Aufräumen der Unit Tests
   * @return 0 wenn erfolgreich
   */
  virtual int after();

  /**
   * Aufräumen der Unit Tests
   * @return 0 wenn erfolgreich
   */
  virtual int after_class();
 private:
  serial_bus::serial_channel* m_serial;
  int m_error;
  int test_serial_channel();

};

} // namespace unit_test
} // namepsace se2

#endif // SE2_SERIAL_TEST_HPP
