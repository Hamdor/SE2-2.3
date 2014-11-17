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
 * @file    dispatcher_test.cpp
 * @version 0.1
 *
 * Unit tests des Dispatchers
 **/

#include "unit_tests/dispatcher_test.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/constants.hpp"

using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::unit_tests;

dispatcher_test::dispatcher_test()
    : m_dispatcher(NULL), m_error(0) {
  // nop
}

dispatcher_test::~dispatcher_test() {
  // nop
}

int dispatcher_test::before_class() {
  return 0;
}

int dispatcher_test::before() {
  m_dispatcher = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  return 0;
}

int dispatcher_test::init() {
  m_test_functions.push_back(&dispatcher_test::test_mapping);
  return 0;
}


int dispatcher_test::after() {
  // Instanzen loeschen, damit der naechste
  // Test frische Instanzen bekommt
  singleton_mgr::shutdown();
  return 0;
}

int dispatcher_test::after_class() {
  return 0;
}

int test_single_mapping_equal(event_values lhs, dispatcher_events rhs) {
  return (dispatcher::map_from_event_values(lhs) == rhs) ? 0 : 1;
}

int dispatcher_test::test_mapping() {
  m_error += test_single_mapping_equal(EVENT_BUTTON_START,
                                       DISPATCHED_EVENT_BUTTON_START);
  m_error += test_single_mapping_equal(EVENT_BUTTON_STOP,
                                       DISPATCHED_EVENT_BUTTON_STOP);
  m_error += test_single_mapping_equal(EVENT_BUTTON_RESET,
                                       DISPATCHED_EVENT_BUTTON_RESET);
  m_error += test_single_mapping_equal(EVENT_BUTTON_E_STOP,
                                       DISPATCHED_EVENT_BUTTON_E_STOP);
  m_error += test_single_mapping_equal(EVENT_ENTRACE_SENSOR,
                                       DISPATCHED_EVENT_ENTRACE_SENSOR);
  m_error += test_single_mapping_equal(EVENT_HEIGHT_SENSOR,
                                       DISPATCHED_EVENT_HEIGHT_SENSOR);
  m_error += test_single_mapping_equal(EVENT_SWITCH_SENSOR,
                                       DISPATCHED_EVENT_SWITCH_SENSOR);
  m_error += test_single_mapping_equal(EVENT_SLIGHT_SENSOR,
                                       DISPATCHED_EVENT_SLIGHT_SENSOR);
  m_error += test_single_mapping_equal(EVENT_EXIT_SENSOR,
                                       DISPATCHED_EVENT_EXIT_SENSOR);
  m_error += test_single_mapping_equal(EVENT_SERIAL_DATA,
                                       DISPATCHED_EVENT_SERIAL_DATA);
  m_error += test_single_mapping_equal(EVENT_SERIAL_MSG,
                                       DISPATCHED_EVENT_SERIAL_MSG);
  m_error += test_single_mapping_equal(EVENT_SERIAL_ERR,
                                       DISPATCHED_EVENT_SERIAL_ERR);
  m_error += test_single_mapping_equal(EVENT_SERIAL_UNK,
                                       DISPATCHED_EVENT_SERIAL_UNK);
  m_error += test_single_mapping_equal(EVENT_SEG1_EXCEEDED,
                                       DISPATCHED_EVENT_SEG1_EXCEEDED);
  m_error += test_single_mapping_equal(EVENT_SEG2_EXCEEDED,
                                       DISPATCHED_EVENT_SEG2_EXCEEDED);
  m_error += test_single_mapping_equal(EVENT_SEG3_EXCEEDED,
                                       DISPATCHED_EVENT_SEG3_EXCEEDED);
  m_error += test_single_mapping_equal(EVENT_SLIDE_FULL,
                                       DISPATCHED_EVENT_SLIDE_FULL);
  m_error += test_single_mapping_equal(EVENT_OPEN_SWITCH,
                                       DISPATCHED_EVENT_OPEN_SWITCH);
  m_error += test_single_mapping_equal(EVENT_TURN_TOKEN,
                                       DISPATCHED_EVENT_TURN_TOKEN);
  m_error += test_single_mapping_equal(EVENT_REMOVE_TOKEN,
                                       DISPATCHED_EVENT_REMOVE_TOKEN);
  m_error += test_single_mapping_equal(EVENT_TOKEN_FINISHED,
                                       DISPATCHED_EVENT_TOKEN_FINISHED);
  return m_error;
}
