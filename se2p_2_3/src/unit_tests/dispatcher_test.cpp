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

#include <unistd.h>

using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::unit_tests;

int dispatcher_test::s_error = 0;
event_values dispatcher_test::s_assumed_next = hal::EVENT_BUTTON_START;

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
  s_error = 0;
  m_error = 0;
  return 0;
}

int dispatcher_test::init() {
  m_test_functions.push_back(&dispatcher_test::test_mapping);
  m_test_functions.push_back(&dispatcher_test::test_function_address_reg);
  m_test_functions.push_back(&dispatcher_test::test_small_fsm);
  m_test_functions.push_back(&dispatcher_test::dispatcher_thread_test);
  return 0;
}


int dispatcher_test::after() {
  return 0;
}

int dispatcher_test::after_class() {
  // Alle Singletons und Threads nach diesen Test loeschen
  singleton_mgr::shutdown();
  return 0;
}

int dispatcher_test::test_single_mapping_equal(event_values lhs,
                                               dispatcher_events rhs) {
  dispatcher* disp = TO_DISPATCHER(
      singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  return
      (dispatcher::map_from_event_values(disp->m_mapping,lhs) == rhs) ? 0 : 1;
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
  m_error += test_single_mapping_equal(EVENT_SENSOR_ENTRANCE_R,
                                       DISPATCHED_EVENT_SENSOR_ENTRANCE_R);
  m_error += test_single_mapping_equal(EVENT_SENSOR_HEIGHT,
                                       DISPATCHED_EVENT_SENSOR_HEIGHT);
  m_error += test_single_mapping_equal(EVENT_SENSOR_HEIGHT_R,
                                       DISPATCHED_EVENT_SENSOR_HEIGHT_R);
  m_error += test_single_mapping_equal(EVENT_SENSOR_SWITCH,
                                       DISPATCHED_EVENT_SENSOR_SWITCH);
  m_error += test_single_mapping_equal(EVENT_SENSOR_SWITCH_R,
                                       DISPATCHED_EVENT_SENSOR_SWITCH_R);
  m_error += test_single_mapping_equal(EVENT_SENSOR_SLIDE,
                                       DISPATCHED_EVENT_SENSOR_SLIDE);
  m_error += test_single_mapping_equal(EVENT_SENSOR_SLIDE_R,
                                       DISPATCHED_EVENT_SENSOR_SLIDE_R);
  m_error += test_single_mapping_equal(EVENT_SENSOR_EXIT,
                                       DISPATCHED_EVENT_SENSOR_EXIT);
  m_error += test_single_mapping_equal(EVENT_SENSOR_EXIT_R,
                                       DISPATCHED_EVENT_SENSOR_EXIT_R);
  m_error += test_single_mapping_equal(EVENT_SERIAL_DATA,
                                       DISPATCHED_EVENT_SERIAL_DATA);
  m_error += test_single_mapping_equal(EVENT_SERIAL_MSG,
                                       DISPATCHED_EVENT_SERIAL_MSG);
  m_error += test_single_mapping_equal(EVENT_SERIAL_NEXT_OK,
                                       DISPATCHED_EVENT_SERIAL_NEXT_OK);
  m_error += test_single_mapping_equal(EVENT_SERIAL_E_STOPP,
                                       DISPATCHED_EVENT_SERIAL_E_STOPP);
  m_error += test_single_mapping_equal(EVENT_SERIAL_ERR,
                                       DISPATCHED_EVENT_SERIAL_ERR);
  m_error += test_single_mapping_equal(EVENT_SERIAL_UNK,
                                       DISPATCHED_EVENT_SERIAL_UNK);
  m_error += test_single_mapping_equal(EVENT_SEG1_HAS_TO_EXPIRE,
                                       DISPATCHED_EVENT_SEG1_HAS_TO_EXPIRE);
  m_error += test_single_mapping_equal(EVENT_SEG2_HAS_TO_EXPIRE,
                                       DISPATCHED_EVENT_SEG2_HAS_TO_EXPIRE);
  m_error += test_single_mapping_equal(EVENT_SEG3_HAS_TO_EXPIRE,
                                       DISPATCHED_EVENT_SEG3_HAS_TO_EXPIRE);
  m_error += test_single_mapping_equal(EVENT_SEG1_TOO_LATE,
                                       DISPATCHED_EVENT_SEG1_TOO_LATE);
  m_error += test_single_mapping_equal(EVENT_SEG2_TOO_LATE,
                                       DISPATCHED_EVENT_SEG2_TOO_LATE);
  m_error += test_single_mapping_equal(EVENT_SEG3_TOO_LATE,
                                       DISPATCHED_EVENT_SEG3_TOO_LATE);
  m_error += test_single_mapping_equal(EVENT_SLIDE_FULL_TIMEOUT,
                                       DISPATCHED_EVENT_SLIDE_FULL_TIMEOUT);
  m_error += test_single_mapping_equal(EVENT_TURN_TOKEN_TIMEOUT,
                                       DISPATCHED_EVENT_TURN_TOKEN_TIMEOUT);
  m_error += test_single_mapping_equal(EVENT_REMOVE_TOKEN_TIMEOUT,
                                       DISPATCHED_EVENT_REMOVE_TOKEN_TIMEOUT);
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
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_ENTRANCE_R],
      &fsm::events::dispatched_event_sensor_entrance_rising);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_HEIGHT],
      &fsm::events::dispatched_event_sensor_height);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_HEIGHT_R],
      &fsm::events::dispatched_event_sensor_height_rising);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_SWITCH],
      &fsm::events::dispatched_event_sensor_switch);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_SWITCH_R],
      &fsm::events::dispatched_event_sensor_switch_rising);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_SLIDE],
      &fsm::events::dispatched_event_sensor_slide);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_SLIDE_R],
      &fsm::events::dispatched_event_sensor_slide_rising);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_EXIT],
      &fsm::events::dispatched_event_sensor_exit);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SENSOR_EXIT_R],
      &fsm::events::dispatched_event_sensor_exit_rising);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_DATA],
      &fsm::events::dispatched_event_serial_data);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_MSG],
      &fsm::events::dispatched_event_serial_msg);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_NEXT_OK],
      &fsm::events::dispatched_event_serial_next_ok);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_ERR],
      &fsm::events::dispatched_event_serial_err);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SERIAL_UNK],
      &fsm::events::dispatched_event_serial_unk);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG1_HAS_TO_EXPIRE],
      &fsm::events::dispatched_event_seg1_has_to_expire);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG2_HAS_TO_EXPIRE],
      &fsm::events::dispatched_event_seg2_has_to_expire);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG3_HAS_TO_EXPIRE],
      &fsm::events::dispatched_event_seg3_has_to_expire);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG1_TOO_LATE],
      &fsm::events::dispatched_event_seg1_too_late);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG2_TOO_LATE],
      &fsm::events::dispatched_event_seg2_too_late);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SEG3_TOO_LATE],
      &fsm::events::dispatched_event_seg3_too_late);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_SLIDE_FULL_TIMEOUT],
      &fsm::events::dispatched_event_slide_full_timeout);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_TURN_TOKEN_TIMEOUT],
      &fsm::events::dispatched_event_turn_token);
  m_error += test_single_fun_ptr(
      m_dispatcher->m_functions[DISPATCHED_EVENT_REMOVE_TOKEN_TIMEOUT],
      &fsm::events::dispatched_event_remove_token);
  return m_error;
}

class state : public se2::fsm::events {
  void register_for_next(event_values assumed, event_values next) {
    if (dispatcher_test::s_assumed_next != assumed) {
      ++dispatcher_test::s_error;
    }
    dispatcher_test::s_assumed_next = next;
    dispatcher* disp = TO_DISPATCHER(
        singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this, next);
  }

  void dispatched_event_button_start() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_button_start()" << std::endl;
#endif
    register_for_next(EVENT_BUTTON_START, EVENT_BUTTON_STOP);
  }

  void dispatched_event_button_stop() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_button_stop()" << std::endl;
#endif
    register_for_next(EVENT_BUTTON_STOP, EVENT_BUTTON_RESET);
  }

  void dispatched_event_button_reset() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_button_reset()" << std::endl;
#endif
    register_for_next(EVENT_BUTTON_RESET, EVENT_SENSOR_ENTRANCE);
  }

  void dispatched_event_button_e_stop() {
    // nop
  }

  void dispatched_event_button_e_stop_rising() {
    // nop
  }

  void dispatched_event_sensor_entrance() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_entrance()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_ENTRANCE, EVENT_SENSOR_ENTRANCE_R);
  }

  void dispatched_event_sensor_entrance_rising() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_entrance_rising()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_ENTRANCE_R, EVENT_SENSOR_HEIGHT);
  }

  void dispatched_event_sensor_height() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_height()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_HEIGHT, EVENT_SENSOR_HEIGHT_R);
  }

  void dispatched_event_sensor_height_rising() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_height_rising()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_HEIGHT_R, EVENT_SENSOR_SWITCH);
  }

  void dispatched_event_sensor_switch() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_switch()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_SWITCH, EVENT_SENSOR_SWITCH_R);
  }

  void dispatched_event_sensor_switch_rising() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_switch()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_SWITCH_R, EVENT_SENSOR_SLIDE);
  }

  void dispatched_event_sensor_slide() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_slide()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_SLIDE, EVENT_SENSOR_SLIDE_R);
  }

  void dispatched_event_sensor_slide_rising() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_slide_rising()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_SLIDE_R, EVENT_SENSOR_EXIT);
  }

  void dispatched_event_sensor_exit() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_exit()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_EXIT, EVENT_SENSOR_EXIT_R);
  }

  void dispatched_event_sensor_exit_rising() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_sensor_exit_rising()" << std::endl;
#endif
    register_for_next(EVENT_SENSOR_EXIT_R, EVENT_SERIAL_DATA);
  }

  void dispatched_event_serial_data() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_serial_data()" << std::endl;
#endif
    register_for_next(EVENT_SERIAL_DATA, EVENT_SERIAL_MSG);
  }

  void dispatched_event_serial_msg() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_serial_msg()" << std::endl;
#endif
    register_for_next(EVENT_SERIAL_MSG, EVENT_SERIAL_NEXT_OK);
  }

  void dispatched_event_serial_next_ok() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_serial_next_ok()" << std::endl;
#endif
    register_for_next(EVENT_SERIAL_NEXT_OK, EVENT_SERIAL_TRANSFER_FIN);
  }

  void dispatched_event_serial_transfer_fin() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_serial_transfer_fin()" << std::endl;
#endif
    register_for_next(EVENT_SERIAL_TRANSFER_FIN, EVENT_SERIAL_ERR);
  }

  void dispatched_event_serial_e_stopp() {

  }

  void dispatched_event_serial_err() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_serial_err()" << std::endl;
#endif
    register_for_next(EVENT_SERIAL_ERR, EVENT_SERIAL_UNK);
  }

  void dispatched_event_serial_unk() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_serial_unk()" << std::endl;
#endif
    register_for_next(EVENT_SERIAL_UNK, EVENT_SEG1_HAS_TO_EXPIRE);
  }

  void dispatched_event_seg1_has_to_expire() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_seg1_has_to_expire()" << std::endl;
#endif
    register_for_next(EVENT_SEG1_HAS_TO_EXPIRE, EVENT_SEG2_HAS_TO_EXPIRE);
  }

  void dispatched_event_seg2_has_to_expire() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_seg2_has_to_expire()" << std::endl;
#endif
    register_for_next(EVENT_SEG2_HAS_TO_EXPIRE, EVENT_SEG3_HAS_TO_EXPIRE);
  }

  void dispatched_event_seg3_has_to_expire() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_seg3_has_to_expire()" << std::endl;
#endif
    register_for_next(EVENT_SEG3_HAS_TO_EXPIRE, EVENT_SEG1_TOO_LATE);
  }

  void dispatched_event_seg1_too_late() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_seg1_too_late()" << std::endl;
#endif
    register_for_next(EVENT_SEG1_TOO_LATE, EVENT_SEG2_TOO_LATE);
  }

  void dispatched_event_seg2_too_late() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_seg2_too_late()" << std::endl;
#endif
    register_for_next(EVENT_SEG2_TOO_LATE, EVENT_SEG3_TOO_LATE);
  }

  void dispatched_event_seg3_too_late() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_seg3_too_late()" << std::endl;
#endif
    register_for_next(EVENT_SEG3_TOO_LATE, EVENT_SLIDE_FULL_TIMEOUT);
  }

  void dispatched_event_slide_full_timeout() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_slide_full_timeout()" << std::endl;
#endif
    register_for_next(EVENT_SLIDE_FULL_TIMEOUT, EVENT_TURN_TOKEN_TIMEOUT);
  }

  void dispatched_event_turn_token() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_turn_token()" << std::endl;
#endif
    register_for_next(EVENT_TURN_TOKEN_TIMEOUT, EVENT_REMOVE_TOKEN_TIMEOUT);
  }

  void dispatched_event_remove_token() {
#ifdef PRINT_TRANSITIONS
    std::cout << "dispatched_event_remove_token()" << std::endl;
#endif
    if (dispatcher_test::s_assumed_next != EVENT_REMOVE_TOKEN_TIMEOUT) {
      ++dispatcher_test::s_error;
    }
  }
};

class fsm {
 public:
  fsm() : m_state(new state) {
    dispatcher* disp = TO_DISPATCHER(
        singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(m_state, EVENT_BUTTON_START);
  }
  ~fsm() {
    delete m_state;
  }
  state* m_state;
};

int dispatcher_test::test_small_fsm() {
  dispatcher_test::s_assumed_next = EVENT_BUTTON_START;
  ::fsm f;
  m_dispatcher->direct_call_event(EVENT_BUTTON_START);
  m_dispatcher->direct_call_event(EVENT_BUTTON_STOP);
  m_dispatcher->direct_call_event(EVENT_BUTTON_RESET);
  m_dispatcher->direct_call_event(EVENT_SENSOR_ENTRANCE);
  m_dispatcher->direct_call_event(EVENT_SENSOR_ENTRANCE_R);
  m_dispatcher->direct_call_event(EVENT_SENSOR_HEIGHT);
  m_dispatcher->direct_call_event(EVENT_SENSOR_HEIGHT_R);
  m_dispatcher->direct_call_event(EVENT_SENSOR_SWITCH);
  m_dispatcher->direct_call_event(EVENT_SENSOR_SWITCH_R);
  m_dispatcher->direct_call_event(EVENT_SENSOR_SLIDE);
  m_dispatcher->direct_call_event(EVENT_SENSOR_SLIDE_R);
  m_dispatcher->direct_call_event(EVENT_SENSOR_EXIT);
  m_dispatcher->direct_call_event(EVENT_SENSOR_EXIT_R);
  m_dispatcher->direct_call_event(EVENT_SERIAL_DATA);
  m_dispatcher->direct_call_event(EVENT_SERIAL_MSG);
  m_dispatcher->direct_call_event(EVENT_SERIAL_NEXT_OK);
  m_dispatcher->direct_call_event(EVENT_SERIAL_TRANSFER_FIN);
  m_dispatcher->direct_call_event(EVENT_SERIAL_ERR);
  m_dispatcher->direct_call_event(EVENT_SERIAL_UNK);
  m_dispatcher->direct_call_event(EVENT_SEG1_HAS_TO_EXPIRE);
  m_dispatcher->direct_call_event(EVENT_SEG2_HAS_TO_EXPIRE);
  m_dispatcher->direct_call_event(EVENT_SEG3_HAS_TO_EXPIRE);
  m_dispatcher->direct_call_event(EVENT_SEG1_TOO_LATE);
  m_dispatcher->direct_call_event(EVENT_SEG2_TOO_LATE);
  m_dispatcher->direct_call_event(EVENT_SEG3_TOO_LATE);
  m_dispatcher->direct_call_event(EVENT_SLIDE_FULL_TIMEOUT);
  m_dispatcher->direct_call_event(EVENT_TURN_TOKEN_TIMEOUT);
  m_dispatcher->direct_call_event(EVENT_REMOVE_TOKEN_TIMEOUT);
  return s_error;
}

int dispatcher_test::dispatcher_thread_test() {
  ::fsm f;
  dispatcher_test::s_assumed_next = EVENT_BUTTON_START;
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int coid = ConnectAttach(0, 0, hal->get_isr_channel(), 0, 0);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_BUTTON_START);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_BUTTON_STOP);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_BUTTON_RESET);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT,
               EVENT_SENSOR_ENTRANCE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT,
               EVENT_SENSOR_ENTRANCE_R);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_HEIGHT);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_HEIGHT_R);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_SWITCH);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_SWITCH_R);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_SLIDE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_SLIDE_R);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_EXIT);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, INTERRUPT, EVENT_SENSOR_EXIT_R);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, EVENT_SERIAL_DATA);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, EVENT_SERIAL_MSG);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, EVENT_SERIAL_NEXT_OK);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, EVENT_SERIAL_TRANSFER_FIN);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, EVENT_SERIAL_ERR);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, SERIAL, EVENT_SERIAL_UNK);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SEG1_HAS_TO_EXPIRE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SEG2_HAS_TO_EXPIRE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SEG3_HAS_TO_EXPIRE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SEG1_TOO_LATE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SEG2_TOO_LATE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SEG3_TOO_LATE);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_SLIDE_FULL_TIMEOUT);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_TURN_TOKEN_TIMEOUT);
  MsgSendPulse(coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, EVENT_REMOVE_TOKEN_TIMEOUT);
  sleep(1);// 1 sek warten damit die FSM auch fertig ist
            // Das ausloesen der Uebergaenge wird von dem Dispatcher Thread
            // gemacht. Deshalb ist dieser Teil asynchron.
  if (dispatcher_test::s_assumed_next != EVENT_REMOVE_TOKEN_TIMEOUT) {
    // Pruefen ob FSM alle Signale erhalten hat
    ++s_error;
  }
#ifndef SIMULATION
  ConnectDetach(coid);
#endif
  return s_error;
}
