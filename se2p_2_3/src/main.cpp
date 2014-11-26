/* 
 * @file    main.cpp
 * @version 0.1
 */

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/dispatcher/dispatcher.hpp"

#include "unit_tests/irq_test.hpp"
#include "unit_tests/test_suite.hpp"
#include "unit_tests/serial_test.hpp"
#include "unit_tests/hal_test_stub1.hpp"
#include "unit_tests/hal_test_stub2.hpp"
#include "unit_tests/dispatcher_test.hpp"
#include "unit_tests/timer_test.hpp"

#include "lib/fsm/state.hpp"
#include "lib/token.hpp"

#include <unistd.h>

using namespace std;
using namespace se2;
using namespace se2::fsm;
using namespace se2::dispatch;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::unit_tests;
using namespace se2::timer;

/**
 * Unit tests starten
 **/
void run_tests() {
  cout << "run `hal_test_stub1`  errors: "
       << test_suite<hal_test_stub1>().run()  << endl;
  cout << "run `hal_test_stub2`  errors: "
       << test_suite<hal_test_stub2>().run()  << endl;
  //cout << "run `irq_test`        errors: "
  //     << test_suite<irq_test>().run()        << endl;
  cout << "run `dispatcher_test` errors: "
       << test_suite<dispatcher_test>().run() << endl;
  cout << "run `timer_test`      errors: "
       << test_suite<timer_test>().run()      << endl;
  #ifdef HAS_SERIAL_INTERFACE
  cout << "run `serial_test`     errors: "
       << test_suite<serial_test>().run()     << endl;
  #endif
  singleton_mgr::shutdown();
}

/**
 * Initialisierung fuer Puck hoehen
 **/
void get_heights() {
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
  while(idx < 10) {
    std::cout << "Bitte " << puck_names[idx].substr(0,
                                                    puck_names[idx].size()-4)
              << " einlegen" << std::endl;
    while(!hal->is_button_pressed(BUTTON_START)) {
      // nop
    }
    while(!hal->obj_in_light_barrier(SENSOR_ENTRANCE)) {
      // nop
    }
    hal->set_motor(MOTOR_RIGHT);
    while(!hal->obj_in_light_barrier(SENSOR_HEIGHT)) {
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
    while(!hal->obj_in_light_barrier(SENSOR_ENTRANCE)) {
      // nop
    }
    hal->set_motor(MOTOR_STOP);
  }
  // Kopierbarer output der direkt in den Code kann...
  cout << "enum height_values {" << endl;
  for (size_t i = 0; i < 10; ++i) {
    cout << "  " << puck_names[i] << " = " << arr[i]
         << (i == 9 ? "" : ",")   << endl;
  }
  cout << "};" << endl;
}

/**
 * Test fuer einen einzelnen Token durchlauf
 **/
void single_token_test() {
  // Ampel ansteuern
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_light(GREEN, 1);
  std::cout << "main: Ampel wurde auf Gruen geschaltet" << std::endl;
  token* t = new token();
  t->set_state(new anonymous_token(t));
  std::cout << "main: Token wurde erstellt und auf Status anonymous_token gesetzt" << std::endl;
  while(1) {
    sleep(1);
  }
}

/**
 * Hauptprogamm
 **/
void main_program() {
  TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  while(1) {
    sleep(1);
  }
}

int main(int argc, char *argv[]) {
#ifdef SIMULATION
  IOaccess_open();
#endif
  get_heights();
  //run_tests();
  //main_program();
  //single_token_test();
  singleton_mgr::shutdown();
#ifdef SIMULATION
  IOaccess_close();
#endif
  cout << "End of main()" << std::endl;
  return 0;
}
