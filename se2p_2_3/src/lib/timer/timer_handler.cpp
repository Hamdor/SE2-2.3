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

size_t timer_handler::IDs              = 0;
timer_handler* timer_handler::instance = 0;

typedef std::map<size_t, timer_wrapper*>::iterator it_type;

void timer_handler::initialize() {

}

void timer_handler::destroy() {

}

timer_handler::timer_handler() {
  m_chid = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->get_isr_channel();
}

timer_handler::~timer_handler() {
  for(it_type iterator = timers.begin(); iterator != timers.end(); ++iterator) {
    delete iterator->second;
  }
  timers.clear();
  IDs      = 0;
  instance = 0;
}

int timer_handler::register_timer(duration time, int value) {
  size_t idx = IDs++;
  timers.insert(std::make_pair(idx, new timer_wrapper(time, value, m_chid)));
  return idx;
}

void timer_handler::change_channel(int chid) {
  m_chid = chid;
}

void timer_handler::delete_timer(size_t pos) {
  delete timers[pos];
  timers.erase(pos);
}

void timer_handler::pause_timer(size_t pos) {
  timers[pos]->pause_timer();
}
void timer_handler::pause_all() {
  for(it_type iterator = timers.begin(); iterator != timers.end(); ++iterator) {
    iterator->second->pause_timer();
  }
}

void timer_handler::continue_timer(size_t pos) {
  timers[pos]->continue_timer();
}

void timer_handler::continue_all() {
  for(it_type iterator = timers.begin(); iterator != timers.end(); ++iterator) {
    iterator->second->continue_timer();
  }
}
void timer_handler::add_time(size_t pos, duration time) {
  timers[pos]->add_time(time);
}

void timer_handler::add_all(duration time) {
  for(it_type iterator = timers.begin(); iterator != timers.end(); ++iterator) {
    iterator->second->add_time(time);
  }
}
