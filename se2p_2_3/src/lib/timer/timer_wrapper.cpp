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
 * @file    timer_wrapper.cpp
 * @version 0.1
 *
 * Wrapper fuer timer struct
 **/

#include "lib/timer/timer_wrapper.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/util/logging.hpp"

using namespace se2;
using namespace timer;
using namespace se2::util;


timer_wrapper::timer_wrapper(duration time, duration interval_value,
                             int pulse_value, int chid)
    : is_running(false)
    , is_paused(true) {
  m_coid = ConnectAttach(0, 0, chid, 0, 0);
  SIGEV_PULSE_INIT(&m_event, m_coid, SIGEV_PULSE_PRIO_INHERIT, TIMER, pulse_value);
  int rc = timer_create(CLOCK_REALTIME, &m_event, &m_timerid);
  if (rc == -1) {
    LOG_ERROR("timer_create() failed")
  }
  //set_time(&m_temp_pause_timer, time, interval_value);
  set_time(&m_timer, time, interval_value);
  start_timer();
}

timer_wrapper::~timer_wrapper() {
  ConnectDetach(m_coid);
  timer_delete(m_timerid);
}

void timer_wrapper::start_timer() {
  if (!is_running) {
    int rc = timer_settime(m_timerid, 0, &m_timer, NULL);
    if (rc == -1) {
      LOG_ERROR("timer_settime() in start_timer failed")
    } else {
      is_running = true;
    }
  }
}

void timer_wrapper::delete_timer() {
  if (is_running) {
    timer_delete(m_timerid);
  }
}

void timer_wrapper::pause_timer() {
  if (!is_paused) {
    int rc = timer_gettime(m_timerid, &m_temp_pause_timer);
    if (rc == -1){
      LOG_ERROR("timer_gettime() in pause_timer failed")
    } else {
      rc = timer_delete(m_timerid);
      if (rc == -1) {
        LOG_ERROR("timer_delete() in pause_timer failed")
      } else {
        is_paused = true;
      }
    }
  }
}

void timer_wrapper::continue_timer() {
  if (is_paused) {
    int rc = timer_settime(m_timerid, 0, &m_temp_pause_timer, 0);
    if (rc == -1) {
      LOG_ERROR("timer_settime() in continue_timer failed")
    } else {
      is_paused = false;
    }
  }
}

void timer_wrapper::change_timer(duration time) {
  set_time(&m_timer, time);
  int rc = timer_settime(m_timerid, 0, &m_timer, NULL);
  if (rc == -1) {
    LOG_ERROR("timer_settime() in change_timer failed")
  }
}

void timer_wrapper::add_time(duration time) {
  if(is_running) {
    m_timer.it_value.tv_sec += time.sec;
    m_timer.it_value.tv_nsec += time.msec * 1000;
  } else {
    m_temp_pause_timer.it_value.tv_sec += time.sec;
    m_temp_pause_timer.it_value.tv_nsec += time.msec * 1000;
  }
}

void timer_wrapper::sub_time(duration time) {
  if(is_running) {
    m_timer.it_value.tv_sec -= time.sec;
    m_timer.it_value.tv_nsec -= time.msec * 1000;
  } else {
    m_temp_pause_timer.it_value.tv_sec -= time.sec;
    m_temp_pause_timer.it_value.tv_nsec -= time.msec * 1000;
  }
}

void timer_wrapper::set_time(itimerspec* spec, duration time) {
  spec->it_value.tv_sec = time.sec;
  spec->it_value.tv_nsec = time.msec * 1000;
}

void timer_wrapper::set_time(itimerspec* spec, duration time, duration interval) {
  spec->it_value.tv_sec = time.sec;
  spec->it_value.tv_nsec = time.msec * 1000;
  spec->it_interval.tv_sec = interval.sec;
  spec->it_interval.tv_nsec = interval.msec * 1000;
}
