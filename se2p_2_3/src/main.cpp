/* 
 * @file    main.cpp
 * @version 0.1
 */

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "lib/util/singleton_mgr.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#ifdef UNIT_TESTS
  #include "unit_tests/irq_test.hpp"
  #include "unit_tests/test_suite.hpp"
  #include "unit_tests/serial_test.hpp"
  #include "unit_tests/hal_test_stub1.hpp"
  #include "unit_tests/hal_test_stub2.hpp"
  #include "unit_tests/dispatcher_test.hpp"
#endif

using namespace std;
using namespace se2;
using namespace se2::util;
using namespace se2::hal;

#ifdef UNIT_TESTS
  using namespace se2::unit_tests;
#endif

int main(int argc, char *argv[]) {
#ifdef SIMULATION
  IOaccess_open();
#endif
  /**
   * Unit Tests
   **/
  cout << "run `hal_test_stub1`  errors: "
       << test_suite<hal_test_stub1>().run()  << endl;
  cout << "run `hal_test_stub2`  errors: "
       << test_suite<hal_test_stub2>().run()  << endl;
  cout << "run `irq_test`        errors: "
       << test_suite<irq_test>().run()        << endl;
  cout << "run `dispatcher_test` errors: "
       << test_suite<dispatcher_test>().run() << endl;
#ifdef HAS_SERIAL_INTERFACE
  cout << "run `serial_test`     errors: "
       << test_suite<serial_test>().run()     << endl;
#endif
  singleton_mgr::shutdown();
#ifdef SIMULATION
  IOaccess_close();
#endif
  return EXIT_SUCCESS;
}
