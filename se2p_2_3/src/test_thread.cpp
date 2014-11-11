/**
 * @file    test_thread.cpp
 * @author Gruppe 2.3
 * @version 0.2
 */

#include <unistd.h>
#include <time.h>
#include "test_thread.hpp"

#include "lib/hal/HWaccess.hpp"
#include "lib/util/mutex.hpp"
#include "lib/util/lock_guard.hpp"
#include "lib/util/singleton_mgr.hpp"

using namespace se2::util;
using namespace se2::hal;

mutex test_thread::mtx_;

test_thread::test_thread(uint16_t times): times_(times) {
  // nop
}

test_thread::~test_thread() {
  // nop
}

void test_thread::execute(void*){
  lock_guard lock(mtx_);
  std::cout << "Blink_Thread executing" << std::endl;
  hwaccess* hal = static_cast<hwaccess*>(singleton_mgr::get_instance(HAL_PLUGIN));
  for (int i = 0; i < times_; i++) {
    if (!isStopped()) {
      hal->set_led_state(START_LED, false);
      hal->set_led_state(RESET_LED, false);
      hal->set_led_state(Q1_LED, false);
      hal->set_led_state(Q2_LED, false);
      hal->set_light(GREEN, true);
      hal->set_light(YELLOW, true);
      hal->set_light(RED, true);
      usleep(500000);
      hal->set_light(GREEN, false);
      hal->set_light(YELLOW, false);
      hal->set_light(RED, false);
      usleep(500000);
      hal->set_motor(MOTOR_RIGHT);
      hal->set_motor(MOTOR_SLOW);
      usleep(100000);
      hal->set_motor(MOTOR_FAST);
      hal->set_motor(MOTOR_LEFT);
      if (hal->is_button_pressed(BUTTON_START)) {
        std::cout << "Start button is pressed" << std::endl;
      }
      if (hal->is_button_pressed(BUTTON_STOP)) {
        std::cout << "Stop button is pressed" << std::endl;
      }
      if (hal->is_button_pressed(BUTTON_RESET)) {
        std::cout << "reset button is pressed" << std::endl;
      }
      if (hal->is_button_pressed(BUTTON_ESTOP)) {
        std::cout << "estop is pressed" << std::endl;
      }
      hal->set_led_state(START_LED, true);
      hal->set_led_state(RESET_LED, true);
      hal->set_led_state(Q1_LED, true);
      hal->set_led_state(Q2_LED, true);
      //hal->get_height_value();
      hal->open_switch();
      if (hal->is_switch_open()) {
        std::cout << "switch is open!" << std::endl;
      } else {
        std::cout << "switch isnt open! ERROR!" << std::endl;
      }
      usleep(500000);
      hal->close_switch();
      if (!hal->is_switch_open()) {
        std::cout << "switch is closed!" << std::endl;
      } else {
        std::cout << "switch is open! ERROR!" << std::endl;
      }
    }
  }
  hal->set_motor(MOTOR_STOP);
}

void test_thread::shutdown(){
    std::cout << "Blink_Thread shutdown" << std::endl;
}
