/**
 * @file    Blink_Thread.hpp
 * @author  Simon Brummer
 * @version 0.1
 * ------------------------------------
 * @author Gruppe 2.3
 * @version 0.2
 */

#ifndef TEST_THREAD_HPP
#define TEST_THREAD_HPP

#include "config.h"

#include <stdint.h>
#include <pthread.h>

#include "lib/util/HAWThread.hpp"
#include "lib/util/mutex.hpp"

using namespace se2::util;

class test_thread : public HAWThread {
 private:
  uint16_t const times_;
  static mutex mtx_;
 public:
  test_thread(uint16_t times);
  virtual ~test_thread();
 private:
  test_thread(const test_thread& b);
  test_thread& operator=(test_thread& b);
  virtual void execute(void*);
  virtual void shutdown();
};

#endif // TEST_THREAD_HPP
