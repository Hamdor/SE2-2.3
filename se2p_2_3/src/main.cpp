/* 
 * @file    main.cpp
 * @version 0.1
 */

#include <cstdlib>
#include <iostream>
#include "test_thread.hpp"
#include "lib/hal/HWaccess.hpp"

#include "unit_tests/hal_test_stub1.hpp"
#include "unit_tests/hal_test_stub2.hpp"

#include "unit_tests/test_suite.hpp"

#include "lib/serial_bus/serial_interface.hpp"

using namespace std;
using namespace se2;
using namespace se2::util;
using namespace se2::unit_tests;

int main(int argc, char *argv[]) {
  #ifdef SIMULATION
      IOaccess_open();
  #endif

#ifdef UNIT_TESTS
  /**
   * Unit Tests kommen hier rein
   **/
   cout << "run `hal_test_stub1` errors: "
        << test_suite<hal_test_stub1>().run() << endl
        << "run `hal_test_stub2` errors: "
        << test_suite<hal_test_stub2>().run() << endl;
#else
  /**
   * Hier wird die eigentliche Logik angestartet
   * TODO: HAL/Thread test entfernen/ersetzen
   **/
  test_thread th1(5);
  test_thread th2(3);

  th1.start(NULL);
  th2.start(NULL);

  th2.join();
  th1.join();

  serial_interface bus;
  int integer = 0;
  bus.read(&integer, sizeof(integer));
#endif

  #ifdef SIMULATION
     IOaccess_close();
  #endif

  return EXIT_SUCCESS;
}
