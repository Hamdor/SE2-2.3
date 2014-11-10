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
      hal->set_led_state(start_button, false);
      hal->set_led_state(reset_button, false);
      hal->set_led_state(q1_button, false);
      hal->set_led_state(q2_button, false);
      hal->set_light(green, true);
      hal->set_light(yellow, true);
      hal->set_light(red, true);
      usleep(500000);
      hal->set_light(green, false);
      hal->set_light(yellow, false);
      hal->set_light(red, false);
      usleep(500000);
      hal->set_motor(motor_right);
      hal->set_motor(motor_slow);
      usleep(100000);
      hal->set_motor(motor_fast);
      hal->set_motor(motor_left);
      if (hal->is_button_pressed(button_start)) {
        std::cout << "Start button is pressed" << std::endl;
      }
      if (hal->is_button_pressed(button_stop)) {
        std::cout << "Stop button is pressed" << std::endl;
      }
      if (hal->is_button_pressed(button_reset)) {
        std::cout << "reset button is pressed" << std::endl;
      }
      if (hal->is_button_pressed(button_estop)) {
        std::cout << "estop is pressed" << std::endl;
      }
      hal->set_led_state(start_button, true);
      hal->set_led_state(reset_button, true);
      hal->set_led_state(q1_button, true);
      hal->set_led_state(q2_button, true);
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
  hal->set_motor(motor_stop);
}

void test_thread::shutdown(){
    std::cout << "Blink_Thread shutdown" << std::endl;
}
