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
 * @version 0.1
 *
 * Wrapper fuer timer struct
 **/

#ifndef SE2_TIMER_WAPPER_HPP
#define SE2_TIMER_WAPPER_HPP
#include <time.h>
#include <sys/neutrino.h>
#include "lib/hal/HWaccess.hpp"

namespace se2 {
namespace timer {

struct duration {
  size_t sec;
  size_t msec;
};

class timer_wrapper {

 public:
  /**
   * Konstruktor
   * @param time dauer des timers
   * @param interval_value gibt den Interval des Timers an
   * @param pulse_value Wert der die Pulse Message senden soll
   **/
  timer_wrapper(duration time, duration interval_value, int pulse_value);

  /**
   * Default Destruktor
   **/
  ~timer_wrapper();

  /**
   * Startet Timer
   **/
  void start_timer();

  /**
   * Stopt den Timer
   **/
  void delete_timer();

  /**
   * Pausiert den Timer
   **/
  void pause_timer();

  /**
   * Setzt den Timer fort
   **/
  void continue_timer();

  /**
   * Veraendert timer auf neue Werte
   * @param time neue Werte fuer den Timer
   **/
  void change_timer(duration time);

  /**
   * Addiert auf Timer die Werte
   * @param time zu addierende Werte
   **/
  void add_time(duration time);

  /**
   * Subtrahiert auf Timer die Werte
   * @param time zu subtrahierende Werte
   **/
  void sub_time(duration time);

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
  itimerspec m_temp_pause_timer;

  /**
   * Zu ausfuerendes Event
   **/
  sigevent m_event;

  /**
   * Connection id
   **/
  int m_coid;

  /**
   * hal instanz
   **/
  hal::hwaccess* m_hal;

  void set_time(itimerspec* spec, duration time);
  void set_time(itimerspec* spec, duration time, duration interval);
  bool is_running;
  bool is_paused;
};
} // namespace timer
} // namespace se2
#endif // SE2_TIMER_WAPPER_HPP
