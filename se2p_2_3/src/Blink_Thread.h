/**
 * @file    Blink_Thread.hpp
 * @author  Simon Brummer
 * @version 0.1
 * ------------------------------------
 * @author Gruppe 2.3
 * @version 0.2
 */

#ifndef BLINK_THREAD_H_
#define BLINK_THREAD_H_

#include <stdint.h>
#include <pthread.h>

#include "lib/util/HAWThread.hpp"
#include "lib/util/mutex.hpp"

using namespace se2::util;

class Blink_Thread: public HAWThread {
 private:
  uint16_t const times_;
  static mutex mtx_;
 public:
  Blink_Thread(uint16_t times);
  virtual ~Blink_Thread();
 private:
  Blink_Thread(const Blink_Thread& b);
  Blink_Thread& operator=(Blink_Thread& b);
  virtual void execute(void*);
  virtual void shutdown();
};

#endif
