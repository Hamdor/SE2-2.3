/* 
 * @file    main.cpp
 * @version 0.1
 */

#include "lib/hal/HWaccess.hpp"
#include "lib/util/singleton_mgr.hpp"

#include <cstdlib>
#include <iostream>

#include <unistd.h>

#if defined (UNIT_TESTS_IRQ) || defined(UNIT_TESTS_STUB)
  #include "unit_tests/hal_test_stub1.hpp"
  #include "unit_tests/hal_test_stub2.hpp"
  #include "unit_tests/irq_test.hpp"
  #include "unit_tests/test_suite.hpp"
#endif

using namespace std;
using namespace se2;
using namespace se2::util;
using namespace se2::hal;

#if defined(UNIT_TESTS_IRQ) || defined (UNIT_TESTS_STUB)
  using namespace se2::unit_tests;
#endif

int main(int argc, char *argv[]) {
#ifdef SIMULATION
  IOaccess_open();
#endif

#ifdef UNIT_TESTS_STUB
  /**
   * Unit Tests kommen hier rein
   **/
   cout << "run `hal_test_stub1` errors: "
        << test_suite<hal_test_stub1>().run() << endl;
   sleep(1);
   cout << "run `hal_test_stub2` errors: "
        << test_suite<hal_test_stub2>().run() << endl;
#elif UNIT_TESTS_IRQ
  cout << "run `irq_tes`        errors: "
       << test_suite<irq_test>().run()        << endl;
#else
  /**
   * Main programm kommt hier rein
   **/
  std::cout << "blub" << std::endl;
  hwaccess* hal = static_cast<hwaccess*>(singleton_mgr::get_instance(HAL));
  std::cout << "blub2" << std::endl;
  hal->open_switch();
  std::cout << "switch should be open" << std::endl;
  sleep(2);
  hal->close_switch();
  std::cout << "switch should be closed" << std::endl;
#endif
  singleton_mgr::shutdown();
#ifdef SIMULATION
  IOaccess_close();
#endif
  return EXIT_SUCCESS;
}
