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
 * @file    mutex.hpp
 * @version 0.1
 *
 * `pthread_mutex_t` Wrapper
 **/

#ifndef SE2_UTIL_MUTEX_HPP
#define SE2_UTIL_MUTEX_HPP

#include <pthread.h>

namespace se2 {
namespace util {

struct condvar;

struct mutex {
  friend struct condvar;

  /**
   * Konstruktor
   */
  mutex();

  /**
   * Destruktor
   */
  virtual ~mutex();

  /**
   * Mutex erlangen. Blockiert bis der Aufrufende Thread
   * das Mutex bekommt.
   * @return EINVAL `pthread_mutex_t` wurde nicht initialisiert
   *         EAGAIN Die maximale Anzahl an Mutexen wurde erreicht
   *         EDEADLK Der aufrufende Thread besitzt das Mutex bereits
   *         0 bei Erfolg
   */
  int acquire();

  /**
   * Mutex zurückgeben.
   * @return EINVAL `pthread_mutex_t` wurde nicht initialisiert
   *         EAGAIN Die maximale Anzahl an Mutexen wurde erreicht
   *         EPERM Der aufrufende Thread besitzt das Mutex nicht
   *         0 bei Erfolg
   */
  int release();

 private:
  mutex(const mutex&);
  pthread_mutex_t m_mutex;
};

} // namespace util
} // namespace se2

#endif // SE2_UTIL_MUTEX_HPP
