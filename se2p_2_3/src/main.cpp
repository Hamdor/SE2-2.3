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
#include "lib/util/logging.hpp"

#include <bitset>

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
   cout << "run `hal_test_stub1` errors: "
        << test_suite<hal_test_stub1>().run() << endl
        << "run `hal_test_stub2` errors: "
        << test_suite<hal_test_stub2>().run() << endl;
#else
  hwaccess* hal = hwaccess::get_instance();
  struct _pulse msg;
  int cnt = 10;
  do {
    MsgReceivePulse(hal->get_isr_channel(), &msg, sizeof(msg), NULL);
    cout << "Msg: "
         << bitset<sizeof(msg.value.sival_int)*8>(msg.value.sival_int)
         << endl;
  } while(--cnt > 0);
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
