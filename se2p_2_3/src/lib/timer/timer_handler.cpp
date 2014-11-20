/******************************************************************************
 *                   ____    ____       ___    ____                           *
 *                  /\  _`\ /\  _`\   /'___`\ /\  _`\                         *
 *                  \ \,\L\_\ \ \L\_\/\_\ /\ \\ \ \L\ \                       *
 *                   \/_\__ \\ \  _\L\/_/// /__\ \ ,__/                       *
 *                     /\ \L\ \ \ \L\ \ // /_\ \\ \ \/                        *
 *                     \ `\____\ \____//\______/ \ \_\                        *
 *                      \/_____/\/___/ \/_____/   \/_/                        *
 *                                                                            *
 * Copyright (C) 2014                                                         *
 * Katja Kirstein    <katja.kirstein    (at) haw-hamburg.de>                  *
 * Anne-Lena Kowalka <anne-lena.kowalka (at) haw-hamburg.de>                  *
 * Marian Triebe     <marian.triebe     (at) haw-hamburg.de>                  *
 * Eugen Winter      <eugen.winter      (at) haw-hamburg.de>                  *
 *                                                                            *
 * Gruppe 2.3                                                                 *
 ******************************************************************************/
/**
 * @file    timer_handler.cpp
 * @version 0.1
 *
 *  Handler fuer timer
 **/

#include "lib/timer/timer_handler.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/hal/HWaccess.hpp"

using namespace se2::hal;
using namespace se2::util;
using namespace se2::timer;

timer_handler* timer_handler::instance = 0;

void timer_handler::initialize() {

}

void timer_handler::destroy() {

}

timer_handler::timer_handler() {
  m_chid = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->get_isr_channel();
}

timer_handler::~timer_handler() {
  for (std::vector<timer_wrapper*>::iterator it = timers.begin();
       it != timers.end(); ++it) {
    (*it)->delete_timer();
    delete *it;
  }
  timers.clear();
  instance = 0;
}

int timer_handler::register_timer(duration time, int value) {
  duration dur;
  dur.msec = 0;
  dur.sec  = 0;
  return register_timer(time, dur, value);
}

int timer_handler::register_timer(duration time, duration interval,
                                  int value) {
  timers.push_back (new timer_wrapper(time, interval, value, m_chid));
  return timers.size() - 1;
}

void timer_handler::change_channel(int chid) {
  m_chid = chid;
}

void timer_handler::delete_timer(size_t pos) {
  if (pos < timers.size()) {
    timer_wrapper* temp = timers[pos];
    temp->delete_timer();
    timers.erase(timers.begin() + pos);
    delete temp;
  }
}

void timer_handler::pause_timer(size_t pos) {
  if (pos < timers.size()) {
    timers[pos]->pause_timer();
  }
}
void timer_handler::pause_all() {
  for (std::vector<timer_wrapper*>::iterator it = timers.begin();
       it != timers.end(); ++it) {
    (*it)->pause_timer();
  }
}

void timer_handler::continue_timer(size_t pos) {
  if (pos < timers.size()) {
    timers[pos]->continue_timer();
  }
}

void timer_handler::continue_all() {
  for (std::vector<timer_wrapper*>::iterator it = timers.begin();
       it != timers.end(); ++it) {
    (*it)->continue_timer();
  }
}

void timer_handler::add_time(size_t pos, duration time) {
  if (pos < timers.size()) {
    timers[pos]->add_time(time);
  }
}
void timer_handler::add_all(duration time) {
  for (std::vector<timer_wrapper*>::iterator it = timers.begin();
       it != timers.end(); ++it) {
    (*it)->add_time(time);
  }
}

void timer_handler::sub_time(size_t pos, duration time) {
  if (pos < timers.size()) {
    timers[pos]->sub_time(time);
  }
}
void timer_handler::sub_all(duration time) {
  for (std::vector<timer_wrapper*>::iterator it = timers.begin();
       it != timers.end(); ++it) {
    (*it)->sub_time(time);
  }
}
