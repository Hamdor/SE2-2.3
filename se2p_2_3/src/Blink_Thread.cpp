/**
 * @file    Blink_Thread.cpp
 * @author  Simon Brummer
 * @version 0.1
 * ------------------------------------
 * @author Gruppe 2.3
 * @version 0.2
 */

#include <unistd.h>
#include <time.h>
#include "Blink_Thread.h"

#include "lib/hal/HWaccess.hpp"
#include "lib/util/mutex.hpp"
#include "lib/util/lock_guard.hpp"

using namespace se2::util;
using namespace se2::hal;

/* Zuweisung der Klassenvariablen */
mutex Blink_Thread::mtx_;

/**
 *  Standard Konstruktor.
 *  Genauer beschreibender Text für Doxygen...
 *  @param times bestimmt wie oft das gruene Licht blinken soll. 
 */
Blink_Thread::Blink_Thread(uint16_t times): times_(times) {
  // nop
}

Blink_Thread::~Blink_Thread() {
  // nop
}

void Blink_Thread::execute(void*){
  /* Klassenweiten Mutex, locken. */
  lock_guard lock(mtx_);
  std::cout << "Blink_Thread executing" << std::endl;
  hwaccess* hal = hwaccess::get_instance();
  /* Gruenes Licht blinken lassen inkl. Pruefung ob der Thread extern gestoppt wurde. */
  for (int i = 0; i < times_; i++) {
    /* Pruefen ob der Thread durch stop() beendet wurde. */
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
    }
  }
  hal->set_motor(motor_stop);
}

void Blink_Thread::shutdown(){
    std::cout << "Blink_Thread shutdown" << std::endl;
}
