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
 * @file    timer_handler.hpp
 * @version 0.1
 *
 *  Handler fuer timer
 **/

#ifndef SE2_TIMER_HANDLER_HPP
#define SE2_TIMER_HANDLER_HPP

#include "lib/util/abstract_singleton.hpp"
#include "lib/timer/timer_wrapper.hpp"
#include "lib/constants.hpp"

#include <vector>

namespace se2 {
namespace util {
class singleton_mgr;
} // namespace util
namespace timer {

class timer_handler : public util::abstract_singleton {
friend class util::singleton_mgr;
 public:
  /**
   * Registriert timer
   * @param time Dauer des Timers
   * @param value Wert der die Pulsemsg senden soll
   * @return Position des neuen Timers
   **/
  int register_timer(duration time, int value);

  /**
   * Registriert timer
   * @param time Dauer des Timers
   * @param interval Intervall des Timers
   * @param value Wert der die Pulsemsg senden soll
   * @return Position des neuen Timers
   **/
  int register_timer(duration time, duration interval, int value);

  /**
   * Wechselt den Pulse Message Channel
   * @param channel id
   **/
  void change_channel(int chid);

  /**
   * Stopt den Timer
   * @param pos Position des Timers
   **/
  void delete_timer(size_t pos);

  /**
   * Pausiert den Timer
   * @param pos Position des Timers
   **/
  void pause_timer(size_t pos);

  /**
   * Pausiert alle registrierten Timer
   **/
  void pause_all();

  /**
   * Setzt den Timer fort
   * @param pos Position des Timers
   **/
  void continue_timer(size_t pos);

  /**
   * Setzt alle registrierten Timer fort
   **/
  void continue_all();
  /**
   * Addiert auf Timer die Werte
   * @param pos Position des Timers
   * @param time zu addierende Werte
   **/
  void add_time(size_t pos, duration time);

  /**
   * Addiert auf alle Timer die Zeit
   * @param time zu addierende Zeit
   **/
  void add_all(duration time);

  /**
   * Subtrahiert auf Timer die Werte
   * @param pos Position des Timers
   * @param time zu subtrahierende Werte
   **/
  void sub_time(size_t pos, duration time);

  /**
   * Subtrahiert auf alle Timer die Zeit
   * @param time zu subtrahierende Zeit
   **/
  void sub_all(duration time);


private:
static timer_handler* instance;
std::vector<timer_wrapper*> timers;
int m_chid;
/**
 * Initialisierung des Singletons
 **/
virtual void initialize();

/**
 * Zerstoerung des Singleton
 **/
virtual void destroy();

 /**
  * Konstruktor
  **/
 timer_handler();

/**
 * Default Destruktor
 * */
 ~timer_handler();
};

} // namespace timer
} // namespace se2
#endif //SE2_TIMER_HANDLER_HPP
