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

#ifndef SE2_HAL_TEST_HPP
#define SE2_HAL_TEST_HPP

#include "lib/hal/HWaccess.hpp"
#include "unit_tests/abstract_test.hpp"
#include "lib/hal/iostub.hpp"

namespace se2 {
namespace unit_tests {

class hal_test_stub1 : public abstract_test<hal_test_stub1> {
 public:
  /**
   * Constructor
   **/
  hal_test_stub1();

  /**
   * Default Destructor
   **/
  ~hal_test_stub1();

  virtual int beforeClass();
  virtual int before();
  virtual int init();
  virtual int after();
  virtual int afterClass();

 private:
  /**
   * Test für die funktionsfähigkeit des Motors
   * @return fehlgeschlagene Anzahl
   **/
   int motor_test();

   /**
    * Tests der Weiche
    * @return fehlgeschlagene Anzahl
    **/
   int switch_test();

   /**
    * Test für die LEDs der Tasten
    * @return fehlgeschlagene Anzahl
    **/
   int button_led_test();

   /**
    * Test für die Ampeln
    * @return fehlgeschlagene Anzahl
    **/
   int light_test();

   /**
    * Tests der funktionsfähigkeit der Lichtschranken
    * @return fehlgeschlagene Anzahl
    **/
   int light_barrier_test();

   /**
    * Tests für nicht gedrückte Tasten
    * @return fehlgeschlagene Anzahl
    **/
   int buttons_not_pressed_test();

   /**
    * Test der Höhe außerhalb des toleranzbereichs
    * @return fehlgeschlagene Anzahl
    **/
   int invalid_heigt_test();

   /**
    * Test des Metallsensors für kein metall
    * @return fehlgeschlagene Anzahl
    **/
   int no_metal_test();
 private:
   hal::hwaccess* m_hal;
   int m_error;
};
} // namespace unit_test
} // namespace se2
#endif // SE2_HAL_TEST_HPP
