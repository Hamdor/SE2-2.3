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
 * @file    hal_test_stub2.hpp
 * @version 0.1
 *
 * Unit tests der HAL
 **/

#ifndef SE2_HAL_TEST_STUB2_HPP
#define SE2_HAL_TEST_STUB2_HPP

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "unit_tests/abstract_test.hpp"
#include "lib/hal/iostub.hpp"

namespace se2 {
namespace unit_tests {

class hal_test_stub2 : public abstract_test<hal_test_stub2> {
 public:
  /**
   * Constructor
   **/
  hal_test_stub2();

  /**
   * Destructor
   **/
  ~hal_test_stub2();

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
  /**
    * Test für durchbrochene Lichtschranken
    * @return fehlgeschlagene Anzahl
    */
  int light_barrier_interrupted_test();

  /**
    * Test für Höhe im Toleranzbereich
    * @return fehlgeschlagene Anzahl
    */
  int valid_height_test();

  /**
    * Test für metall
    * @return fehlgeschlagene Anzahl
    */
  int metal_detected_test();

  /**
    * Test für gedrückte Tasten
    * @return fehlgeschlagene Anzahl
    */
  int buttons_pressed_test();

  hal::hwaccess* m_hal;
  int m_error;
};
} // namespace unit_test
} // namepsace se2

#endif // SE2_HAL_TEST_STUB2_HPP
