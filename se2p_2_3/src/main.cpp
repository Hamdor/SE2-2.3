/* 
 * @file    main.cpp
 * @version 0.1
 */

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/dispatcher/dispatcher.hpp"
#include "lib/fsm/events.hpp"
#include "lib/fsm/token.hpp"
#include "lib/fsm/state.hpp"

#include "unit_tests/irq_test.hpp"
#include "unit_tests/test_suite.hpp"
#include "unit_tests/serial_test.hpp"
#include "unit_tests/hal_test_stub1.hpp"
#include "unit_tests/hal_test_stub2.hpp"
#include "unit_tests/dispatcher_test.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;
using namespace se2;
using namespace se2::fsm;
using namespace se2::dispatch;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::unit_tests;

int main(int argc, char *argv[]) {
#ifdef SIMULATION
  IOaccess_open();
#endif
//  /**
//   * Unit Tests
//   **/
//  cout << "run `hal_test_stub1`  errors: "
//       << test_suite<hal_test_stub1>().run()  << endl;
//  cout << "run `hal_test_stub2`  errors: "
//       << test_suite<hal_test_stub2>().run()  << endl;
//  cout << "run `irq_test`        errors: "
//       << test_suite<irq_test>().run()        << endl;
//  cout << "run `dispatcher_test` errors: "
//       << test_suite<dispatcher_test>().run() << endl;
//#ifdef HAS_SERIAL_INTERFACE
//  cout << "run `serial_test`     errors: "
//       << test_suite<serial_test>().run()     << endl;
//#endif
  /**
   * Token Test
   **/

  // Ampel ansteuern
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_light(GREEN, 1);
  std::cout << "main: Ampel wurde auf Gruen geschaltet" << std::endl;

  token* t = new token();
  t->set_state(new anonymous_token(t));
  std::cout << "main: Token wurde erstellt und auf Status anonymous_token gesetzt" << std::endl;

  while(1);

  singleton_mgr::shutdown();
#ifdef SIMULATION
  IOaccess_close();
#endif
  cout << "End of main()" << std::endl;
  return EXIT_SUCCESS;
}
