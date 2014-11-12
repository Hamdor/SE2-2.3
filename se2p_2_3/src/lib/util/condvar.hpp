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
 * @file    condvar.hpp
 * @version 0.1
 *
 * `pthread_cond_t` Wrapper
 **/

#ifndef SE2_UTIL_CONDVAR_HPP
#define SE2_UTIL_CONDVAR_HPP

#include "config.h"
#include "mutex.hpp"
#include <pthread.h>

namespace se2 {
namespace util {

struct condvar {
  /**
   * Konsturktor condvar. Erstellt eine neue Conditionvariable,
   * diese ist mit dem referenzierten Mutex verknüpft.
   * @param mutex ist die Referenz auf ein mutex
   */
  condvar(mutex& mutex);

  /**
   * Destruktor
   */
  virtual ~condvar();

  /**
   * Auf dieser Conditionvariable blockieren, bis diese
   * Conditionvariable signalisiert wird.
   *
   * @return EINVAL `pthread_mutex_t` oder `pthread_cond_t` sind ungültig
   *         EINVAL Der aufrufende Thread war/ist nicht im besitzt von
   *         `pthread_mutex_t`
   *         0 bei Erfolg
   */
  int wait();

  /**
   * Diese Funktion entblockt einen Thread der auf diese Conditionvariable
   * blockieren.
   * @returns EINVAL `pthread_cond_t` wurde nicht initialisiert
   *          0 bei Erfolg
   */
  int signal();

  /**
   * Diese Funktion entblockt alle Threads die auf diese Conditionvariable
   * blockieren.
   * @returns EINVAL `pthread_cond_t` wurde nicht initialisiert
   *          0 bei Erfolg
   */
  int broadcast();

 private:
  condvar();
  condvar(const condvar&);

  mutex&         m_lock;
  pthread_cond_t m_cond;
};

} // namespace util
} // namespace se2

#endif // SE2_UTIL_CONDVAR_HPP
