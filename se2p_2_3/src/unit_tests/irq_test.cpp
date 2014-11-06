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
 * @file    irq_test.cpp
 * @version 0.1
 *
 * Unit tests der ISR/IRQ
 **/

#include "unit_tests/irq_test.hpp"
#include "lib/util/singleton_mgr.hpp"

#include <bitset>
#include <iostream>

using namespace std;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::unit_tests;

irq_test::irq_test() : m_hal(NULL), m_error(0) {
  // nop
}

irq_test::~irq_test() {
  // nop
}

int irq_test::before_class() {
  m_hal = TO_HAL(singleton_mgr::get_instance(HAL));
  return 0;
}

int irq_test::before() {
  return 0;
}

int irq_test::init() {
  m_test_functions.push_back(&irq_test::open_switch);
  m_test_functions.push_back(&irq_test::close_switch);
  return 0;
}

int irq_test::after() {
  return 0;
}

int irq_test::after_class() {
  delete m_hal;
  return 0;
}

int irq_test::open_switch() {
  m_hal->open_switch();
  struct _pulse msg;
  MsgReceivePulse(m_hal->get_isr_channel(), &msg, sizeof(msg), NULL);
  bitset<32> expected(0b00000000000000001110101110100000);
  bitset<32> value(msg.value.sival_int);
  if (expected != value) {
    m_error++;
  }
  return m_error;
}

int irq_test::close_switch() {
  m_hal->close_switch();
  struct _pulse msg;
  MsgReceivePulse(m_hal->get_isr_channel(), &msg, sizeof(msg), NULL);
  bitset<32> expected(0b00000000000000001100101110100000);
  bitset<32> value(msg.value.sival_int);
  if (expected != value) {
    m_error++;
  }
  return m_error;
}
