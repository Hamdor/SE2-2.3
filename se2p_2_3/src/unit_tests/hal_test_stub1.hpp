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
 * @file    hal_test_stub1.hpp
 * @version 0.1
 *
 * Unit tests der HAL
 **/

#ifndef SE2_HAL_TEST_STUB1_HPP
#define SE2_HAL_TEST_STUB1_HPP

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "unit_tests/abstract_test.hpp"
#include "lib/hal/iostub.hpp"

namespace se2 {
namespace unit_tests {

class hal_test_stub1 : public abstract_test<hal_test_stub1> {
 public:
  /**
   * Default Konstruktor
   **/
  hal_test_stub1();

  /**
   * Default Destruktor
   **/
  virtual ~hal_test_stub1();

  virtual int before_class();
  virtual int before();
  virtual int init();
  virtual int after();
  virtual int after_class();

 private:
  /**
   * Test fuer die Funktionsfaehigkeit des Motors
   * @return Fehlgeschlagene Anzahl
   **/
   int motor_test();

   /**
    * Tests der Weiche
    * @return fehlgeschlagene Anzahl
    **/
   int switch_test();

   /**
    * Test fuer die LEDs der Tasten
    * @return Fehlgeschlagene Anzahl
    **/
   int button_led_test();

   /**
    * Test fuer die Ampeln
    * @return Fehlgeschlagene Anzahl
    **/
   int light_test();

   /**
    * Tests der Funktionsfaehigkeit der Lichtschranken
    * @return Fehlgeschlagene Anzahl
    **/
   int light_barrier_test();

   /**
    * Tests fuer nicht gedrueckte Tasten
    * @return Fehlgeschlagene Anzahl
    **/
   int buttons_not_pressed_test();

   /**
    * Test der Hoehe ausserhalb des Toleranzbereichs
    * @return Fehlgeschlagene Anzahl
    **/
   int invalid_heigt_test();

   /**
    * Test des Metallsensors fuer kein Metall
    * @return Fehlgeschlagene Anzahl
    **/
   int no_metal_test();
 private:
   hal::hwaccess* m_hal;
   int m_error;
};
} // namespace unit_test
} // namespace se2
#endif // SE2_HAL_TEST_STUB1_HPP
