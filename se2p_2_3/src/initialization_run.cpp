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
 * @file    initialization_run.cpp
 * @version 0.1
 *
 * In diesem Modul werden HW-Laufzeiten sowie Hoehensensorwerte
 * ausgelesen um eine Konfiguration des Hauptprogramms vorzunehmen
 **/

#include "initialization_run.hpp"
#include "lib/util/singleton_mgr.hpp"

#include "unit_tests/irq_test.hpp"
#include "unit_tests/test_suite.hpp"
#include "unit_tests/serial_test.hpp"
#include "unit_tests/hal_test_stub1.hpp"
#include "unit_tests/hal_test_stub2.hpp"
#include "unit_tests/dispatcher_test.hpp"
#include "unit_tests/timer_test.hpp"

#include <cstring>

using namespace se2;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::unit_tests;

void initialization_run::get_heights() {
  std::cout << "====== Start of `get_heights` ======" << std::endl;
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  std::string puck_names[] = {
      "TOO_SMALL_LOW",       "TOO_SMALL_HI",
      "HOLE_BOTTOM_UP_LOW",  "HOLE_BOTTOM_UP_HI",
      "HOLE_LOW",            "HOLE_HI",
      "METAL_BOTTOM_UP_LOW", "METAL_BOTTOM_UP_HI",
      "METAL_LOW",           "METAL_HI"
  };
  size_t arr[10];
  size_t min = 0xFFFFFFFF;
  size_t max = 0;
  size_t idx = 0;
  while (idx < 10) {
    std::cout << "Bitte " << puck_names[idx].substr(0,
                                                    puck_names[idx].size()-4)
              << " einlegen" << std::endl;
    while (!hal->is_button_pressed(BUTTON_START)) {
      // nop
    }
    while (!hal->obj_in_light_barrier(SENSOR_ENTRANCE)) {
      // nop
    }
    hal->set_motor(MOTOR_RIGHT);
    while (!hal->obj_in_light_barrier(SENSOR_HEIGHT)) {
      // nop
    }
    hal->set_motor(MOTOR_STOP);
    // MIN - MAX holen
    min = 0xFFFFFFFF;
    max = 0;
    for (int i = 0; i < 250000; ++i) {
      size_t cur = hal->get_height_value();
      max = std::max(max, cur);
      min = std::min(min, cur);
    }
    // Min und max werte merken
    arr[idx++] = min;
    arr[idx++] = max;
    hal->set_motor(MOTOR_LEFT);
    while (!hal->obj_in_light_barrier(SENSOR_ENTRANCE)) {
      // nop
    }
    hal->set_motor(MOTOR_STOP);
  }
  // Kopierbarer output der direkt in den Code kann...
  std::cout << "enum height_values {" << std::endl;
  for (size_t i = 0; i < 10; ++i) {
    std::cout << "  " << puck_names[i] << " = " << arr[i]
              << (i == 9 ? "" : ",")   << std::endl;
  }
  std::cout << "};" << std::endl;
  std::cout << "======  End of `get_heights`  ======" << std::endl;
}

#define SEC_TO_NSEC 1000000000
timespec initialization_run::diff(timespec start, timespec end) {
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = SEC_TO_NSEC + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp;
}

void initialization_run::get_times() {
  std::cout << "====== Start of `get_times` ======" << std::endl;
  // 0 - 3 sind der schnelle durchlauf
  // 3 - 5 sind mit höhenmessung langsame
  timespec times[6];
  int which_timeintervall = 0;
  int run_index = 0; // erster lauf ist der schnelle
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  while (run_index <= 1) {
    std::cout << "Bitte ein Werkstueck einlegen." << std::endl;
    while (!hal->obj_in_light_barrier(SENSOR_ENTRANCE)) {
      // nop
    }
    if (run_index == 0) {
      hal->set_motor(MOTOR_RIGHT);
    }
    get_curr_time();
    while (!hal->obj_in_light_barrier(SENSOR_HEIGHT)) {
      // nop
    }
    // ist nun in Hoehenmessung
    if(run_index == 1) {
      hal->set_motor(MOTOR_SLOW);
    }
    while (!hal->obj_in_light_barrier(SENSOR_HEIGHT)) {
      // nop
    }
    // nicht mehr in Hoehenmessung
    hal->set_motor(MOTOR_FAST);
    times[which_timeintervall++] = get_curr_time();
    while (!hal->obj_in_light_barrier(SENSOR_SWITCH)) {
      // nop
    }
    hal->open_switch();
    times[which_timeintervall++] = get_curr_time();
    while (hal->obj_in_light_barrier(SENSOR_SWITCH)) {
      // nop
    }
    hal->close_switch();
    times[which_timeintervall++] = get_curr_time();
    while (!hal->obj_in_light_barrier(SENSOR_EXIT)) {
      // nop
    }
    hal->set_motor(MOTOR_STOP);
    while (hal->obj_in_light_barrier(SENSOR_EXIT)) {
      // nop
    }
    ++run_index;
  }
  for (int i = 0; i < 3; ++i) {
    times[i + 3] = diff(times[i + 3], times[i]);
  }
  print_time(times);
  std::cout << "======  End of `get_times`  ======" << std::endl;
}

timespec initialization_run::get_curr_time() {
  static timespec oldspec;
  timespec curspec;
  timespec diffspec;
  std::memset(&curspec, 0, sizeof(timespec));
  std::memset(&diffspec, 0, sizeof(timespec));
  clock_gettime(CLOCK_REALTIME, &curspec);
  diffspec = diff(oldspec, curspec);
  oldspec = curspec;
  return diffspec;
}

#define REM_TO_EXPIRE_SEC  0
#define REM_TO_EXPIRE_MSEC 200

#define ADD_TO_LATE_SEC    0
#define ADD_TO_LATE_MSEC   250

#define UNDERFLOW_CHECK(o,n) std::min(o,n)

void initialization_run::print_time(const timespec time[]) {
  std::string time_names[] = {
      "SEGMENT1__SEC", "SEGMENT1_NSEC",
      "SEGMENT2__SEC", "SEGMENT2__NSEC",
      "SEGMENT3__SEC", "SEGMENT3__NSEC",
      "HEIGHT_TIME_OFFSET_SEG1_SEC", "HEIGHT_TIME_OFFSET_SEG1_NSEC",
      "HEIGHT_TIME_OFFSET_SEG2_SEC", "HEIGHT_TIME_OFFSET_SEG2_NSEC",
      "HEIGHT_TIME_OFFSET_SEG3_SEC", "HEIGHT_TIME_OFFSET_SEG3_NSEC",
  };
  size_t idx   = 0;
  for (size_t i = 0; i < 6; ++i) {
    std::cout << "#define "   << time_names[idx++] << " "
              << time[i].tv_sec<< std::endl;
    std::cout << "#define "   << time_names[idx++] << " "
              << time[i].tv_nsec<< std::endl;
  }
}

void initialization_run::run_tests() {
  std::cout << "====== Start of Unit-Tests ======" << std::endl;
  std::cout << "run `hal_test_stub1`  errors: "
            << test_suite<hal_test_stub1>().run()  << std::endl;
  std::cout << "run `hal_test_stub2`  errors: "
            << test_suite<hal_test_stub2>().run()  << std::endl;
  std::cout << "run `irq_test`        errors: "
            << test_suite<irq_test>().run()        << std::endl;
  std::cout << "run `dispatcher_test` errors: "
            << test_suite<dispatcher_test>().run() << std::endl;
  std::cout << "run `timer_test`      errors: "
            << test_suite<timer_test>().run()      << std::endl;
  #ifdef HAS_SERIAL_INTERFACE
  std::cout << "run `serial_test`     errors: "
            << test_suite<serial_test>().run()     << std::endl;
  #endif
  singleton_mgr::shutdown();
  std::cout << "======  End of Unit-Tests  ======" << std::endl;
}

void initialization_run::start_init() {
  //run_tests();
#ifndef SIMULATION
  // Die Simulation hat keinen Motor Linsklauf
  // ausserdem sind die Hoehenwerte in der Simulation falsch.
  //get_heights();
#endif
  get_times();
  HAWThread::reset_shutdown_flag();
}
