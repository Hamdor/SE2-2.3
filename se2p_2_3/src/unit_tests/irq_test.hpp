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
 * @file    irq_test.hpp
 * @version 0.1
 *
 * Unit tests der ISR/IRQ
 **/

#ifndef SE2_IRQ_TEST_HPP
#define SE2_IRQ_TEST_HPP

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "unit_tests/abstract_test.hpp"

namespace se2 {
namespace unit_tests {

class irq_test : public abstract_test<irq_test> {
 public:
  /**
   * Default Konstruktor
   **/
  irq_test();

  /**
   * Default Destruktor
   **/
  virtual ~irq_test();

  virtual int before_class();
  virtual int before();
  virtual int init();
  virtual int after();
  virtual int after_class();

 private:
  int open_switch();
  int close_switch();

  hal::hwaccess* m_hal;
  int m_error;
};

} // namespace unit_test
} // namepsace se2

#endif // SE2_IRQ_TEST_HPP
