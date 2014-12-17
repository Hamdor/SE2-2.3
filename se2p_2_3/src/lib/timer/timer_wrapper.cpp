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
 * @version 0.2
 *
 * Wrapper fuer timer struct
 **/

#include "lib/timer/timer_wrapper.hpp"
#include "lib/timer/time_utils.hpp"
#include "lib/util/logging.hpp"
#include <cstring>

using namespace se2;
using namespace se2::util;
using namespace se2::timer;

#define MILISEC_TO_NANOSEC 1000000

timer_wrapper::timer_wrapper(duration time, int pulse_value, int chid)
    : m_coid(0), m_paused(false) {
  m_coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
  if (m_coid == -1) {
    LOG_ERROR("ConnectAttach() failed in timer_wrapper()")
    perror("");
  }
  SIGEV_PULSE_INIT(&m_event, m_coid,
                   SIGEV_PULSE_PRIO_INHERIT, TIMER, pulse_value);
  int rc = timer_create(CLOCK_REALTIME, &m_event, &m_timerid);
  if (rc == -1) {
    LOG_ERROR("timer_create() failed in timer_wrapper()")
  }
  m_duration = time;
  reset_timer();
  start_timer();
}

timer_wrapper::~timer_wrapper() {
  stop_timer();
  int rc = ConnectDetach(m_coid);
  if (rc == -1) {
    LOG_ERROR("ConnectDetach() failed in ~timer_wrapper()")
  }
  rc = timer_delete(m_timerid);
  if (rc == -1) {
    LOG_ERROR("timer_delete() failed in ~timer_wrapper()")
  }
}

void timer_wrapper::reset_timer() {
  m_timer.it_value.tv_sec = m_duration.sec;
  m_timer.it_value.tv_nsec = m_duration.msec * MILISEC_TO_NANOSEC;
  m_timer.it_interval.tv_sec = 0;
  m_timer.it_interval.tv_nsec = 0;
}

void timer_wrapper::start_timer() {
  int rc = timer_settime(m_timerid, 0, &m_timer, NULL);
  if (rc == -1) {
    LOG_ERROR("timer_settime() failed in start_timer()")
  }
}

void timer_wrapper::stop_timer() {
  std::memset(&m_timer, 0, sizeof(itimerspec));
  int rc = timer_settime(m_timerid, 0, &m_timer, NULL);
  if (rc == -1) {
    LOG_ERROR("timer_settime() failed in stop_timer()")
  }
  reset_timer();
}

void timer_wrapper::pause_timer() {
  if (!m_paused) {
    itimerspec nullspec;
    std::memset(&nullspec, 0, sizeof(itimerspec));
    int rc = timer_settime(m_timerid, 0, &nullspec, &m_temp_timer);
    if (rc == -1) {
      LOG_ERROR("timer_settime() failed in pause_timer()")
    }
    m_paused = true;
  }
}

void timer_wrapper::continue_timer() {
  if (m_paused) {
    int rc = timer_settime(m_timerid, 0, &m_temp_timer, NULL);
    if (rc == -1) {
      LOG_ERROR("timer_settime() failed in continue_timer()")
    }
    m_paused = false;
  }
}

void timer_wrapper::add_time(duration timer) {
  itimerspec tempspec;
  std::memset(&tempspec, 0, sizeof(itimerspec));
  timer_gettime(m_timerid, &tempspec);
  stop_timer();
  const timespec durspec = { timer.sec, (long)timer.msec * MILISEC_TO_NANOSEC };
  timespec curspec  = tempspec.it_value;
  tempspec.it_value = time_utils::add(curspec, durspec);
  m_timer = tempspec;
  start_timer();
}
