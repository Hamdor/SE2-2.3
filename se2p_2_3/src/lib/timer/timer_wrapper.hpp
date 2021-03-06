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
 * @file    timer_wrapper.hpp
 * @version 0.2
 *
 * Wrapper fuer timer struct
 **/

#ifndef SE2_TIMER_WAPPER_HPP
#define SE2_TIMER_WAPPER_HPP

#include "config.h"

#include "lib/constants.hpp"

#include <time.h>
#include <sys/neutrino.h>

namespace se2 {
namespace util {
class light_mgr;
}
namespace timer {
class timer_handler;

class timer_wrapper {
  friend class timer_handler;
  friend class util::light_mgr;
  /**
   * Default Konstruktor
   * @param time Dauer des Timers
   * @param pulse_value Wert der die Pulse Message senden soll
   * @param chid Ist die channel ID
   **/
  timer_wrapper(duration time, int pulse_value, int chid);

  /**
   * Default Destruktor
   **/
  ~timer_wrapper();

 public:

  /**
   * Startet Timer
   **/
  void start_timer();

  /**
   * Stopt den Timer
   **/
  void stop_timer();

  /**
   * Pausiert den Timer
   **/
  void pause_timer();

  /**
   * Setzt den Timer fort
   **/
  void continue_timer();

  /**
   * Resetet timer
   **/
  void reset_timer();

  /**
   * Addiert die angegebene Zeit
   *  @param time Zu addierende Zeit
   **/
  void add_time(duration time);

 private:
  /**
   * Timer ID
   **/
  timer_t m_timerid;

  /**
   * Timer Daten
   **/
  itimerspec m_timer;

  /**
   * Temp-Timer fuer Pausierung
   **/
  itimerspec m_temp_timer;

  /**
   * Zu ausfuerendes Event
   **/
  sigevent m_event;

  /**
   * Connection id
   **/
  int m_coid;
  bool m_paused;
  duration m_duration;
};
} // namespace timer
} // namespace se2
#endif // SE2_TIMER_WAPPER_HPP
