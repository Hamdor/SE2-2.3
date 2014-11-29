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
 * @file    lock_guard.hpp
 * @version 0.1
 *
 * Implementierung des `Scoped Locking` Pattern
 **/

#ifndef SE2_UTIL_LOCK_GUARD_HPP
#define SE2_UTIL_LOCK_GUARD_HPP

#include "config.h"
#include "lib/util/mutex.hpp"

namespace se2 {
namespace util {

struct lock_guard {
  /**
   * Konstructor, lockt das Mutex
   */
  lock_guard(mutex&);
  
  /**
   * Destruktor, gibt das Mutex frei
   */
  virtual ~lock_guard();

 private:
  lock_guard(const lock_guard&);
  mutex& m_ref;
};

} // namespace util
} // namespace se2

#endif // SE2_UTIL_LOCK_GUARD_HPP
