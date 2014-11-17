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
#include "lib/fsm/events.hpp"

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
  m_dispatcher = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  return 0;
}

int dispatcher_test::before() {
  return 0;
}

int dispatcher_test::init() {
  m_test_functions.push_back(&dispatcher_test::test_mapping);
  m_test_functions.push_back(&dispatcher_test::test_function_address_reg);
  return 0;
}


int dispatcher_test::after() {
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
  m_error += test_single_mapping_equal(EVENT_SENSOR_ENTRANCE,
                                       DISPATCHED_EVENT_SENSOR_ENTRANCE);
  m_error += test_single_mapping_equal(EVENT_SENSOR_HEIGHT,
                                       DISPATCHED_EVENT_SENSOR_HEIGHT);
  m_error += test_single_mapping_equal(EVENT_SENSOR_SWITCH,
                                       DISPATCHED_EVENT_SENSOR_SWITCH);
  m_error += test_single_mapping_equal(EVENT_SENSOR_SLIDE,
                                       DISPATCHED_EVENT_SENSOR_SLIDE);
  m_error += test_single_mapping_equal(EVENT_SENSOR_EXIT,
                                       DISPATCHED_EVENT_SENSOR_EXIT);
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

int test_single_fun_ptr(const func_t rhs, const func_t lhs) {
  return rhs == lhs ? 0 : 1;
}

int dispatcher_test::test_function_address_reg() {
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_BUTTON_START],
      &fsm::events::dispatched_event_button_start);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_BUTTON_STOP],
      &fsm::events::dispatched_event_button_stop);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_BUTTON_RESET],
      &fsm::events::dispatched_event_button_reset);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_BUTTON_E_STOP],
      &fsm::events::dispatched_event_button_e_stop);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_ENTRANCE],
      &fsm::events::dispatched_event_sensor_entrance);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_HEIGHT],
      &fsm::events::dispatched_event_sensor_height);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_SWITCH],
      &fsm::events::dispatched_event_sensor_switch);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_SLIDE],
      &fsm::events::dispatched_event_sensor_slide);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_EXIT],
      &fsm::events::dispatched_event_sensor_exit);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_DATA],
      &fsm::events::dispatched_event_serial_data);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_MSG],
      &fsm::events::dispatched_event_serial_msg);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_ERR],
      &fsm::events::dispatched_event_serial_err);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_UNK],
      &fsm::events::dispatched_event_serial_unk);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG1_EXCEEDED],
      &fsm::events::dispatched_event_seg1_exceeded);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG2_EXCEEDED],
      &fsm::events::dispatched_event_seg2_exceeded);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG3_EXCEEDED],
      &fsm::events::dispatched_event_seg3_exceeded);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SLIDE_FULL],
      &fsm::events::dispatched_event_slide_full);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_OPEN_SWITCH],
      &fsm::events::dispatched_event_open_switch);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_TURN_TOKEN],
      &fsm::events::dispatched_event_turn_token);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_REMOVE_TOKEN],
      &fsm::events::dispatched_event_remove_token);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_TOKEN_FINISHED],
      &fsm::events::dispatched_event_token_finished);
  return m_error;
}

