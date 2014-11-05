/* 
 * @file    main.cpp
 * @version 0.1
 */

#include <cstdlib>
#include <iostream>
#include "test_thread.hpp"
#include "lib/hal/HWaccess.hpp"

#ifdef UNIT_TESTS
  #include "unit_tests/hal_test_stub1.hpp"
  #include "unit_tests/hal_test_stub2.hpp"
  #include "unit_tests/irq_test.hpp"
  #include "unit_tests/test_suite.hpp"
#endif

#include "lib/serial_bus/serial_interface.hpp"
#include "lib/util/logging.hpp"

using namespace std;
using namespace se2;
using namespace se2::util;
using namespace se2::unit_tests;
using namespace se2::hal;

int main(int argc, char *argv[]) {
  #ifdef SIMULATION
      IOaccess_open();
  #endif

#ifdef UNIT_TESTS
  /**
   * Unit Tests kommen hier rein
   **/
   /*cout << "run `hal_test_stub1` errors: "
        << test_suite<hal_test_stub1>().run() << endl;
   sleep(1);
   cout << "run `hal_test_stub2` errors: "
        << test_suite<hal_test_stub2>().run() << endl;
   sleep(1);*/
   cout << "run `irq_tes`        errors: "
        << test_suite<irq_test>().run()       << endl;
#else
  /**
   * Main programm kommt hier rein
   **/
#endif
  /**
   * TODO Add proper way to handle singletons
   **/
  delete se2::hal::hwaccess::get_instance();
  delete se2::util::logging::get_instance();
#ifdef SIMULATION
   IOaccess_close();
#endif
  return EXIT_SUCCESS;
}
