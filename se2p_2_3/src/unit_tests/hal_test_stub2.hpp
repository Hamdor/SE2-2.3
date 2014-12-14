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
   * Default Konstruktor
   **/
  hal_test_stub2();

  /**
   * Default Destruktor
   **/
  virtual ~hal_test_stub2();

  virtual int before_class();
  virtual int before();
  virtual int init();
  virtual int after();
  virtual int after_class();

 private:
  /**
    * Test fuer durchbrochene Lichtschranken
    * @return Fehlgeschlagene Anzahl
    */
  int light_barrier_interrupted_test();

  /**
    * Test fuer Hoehe im Toleranzbereich
    * @return Fehlgeschlagene Anzahl
    */
  int valid_height_test();

  /**
    * Test fuer Metall
    * @return Fehlgeschlagene Anzahl
    */
  int metal_detected_test();

  /**
    * Test fuer gedrueckte Tasten
    * @return Fehlgeschlagene Anzahl
    */
  int buttons_pressed_test();

  hal::hwaccess* m_hal;
  int m_error;
};
} // namespace unit_test
} // namepsace se2

#endif // SE2_HAL_TEST_STUB2_HPP
