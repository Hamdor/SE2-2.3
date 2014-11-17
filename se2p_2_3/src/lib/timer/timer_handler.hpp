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

namespace se2 {
namespace util {
class singleton_mgr;
} // namespace util
namespace timer {

class timer_handler : public util::abstract_singleton {
friend class util::singleton_mgr;

  /**
   * Registriert timer
   **/
  int register_timer(duration time, duration interval, int value);
private:
static timer_handler* instance;
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
