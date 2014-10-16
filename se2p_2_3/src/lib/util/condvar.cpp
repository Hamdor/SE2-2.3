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
 * @file    condvar.cpp
 * @version 0.1
 *
 * `pthread_cond_t` Wrapper
 **/

#include "condvar.hpp"

using namespace se2::util;

condvar::condvar(mutex& mutex) : m_lock(mutex) {
  pthread_cond_init(&m_cond, NULL);
}

condvar::~condvar() {
  pthread_cond_destroy(&m_cond);
}

int condvar::wait() {
  return pthread_cond_wait(&m_cond, &m_lock.m_mutex);
}

int condvar::signal() {
  return pthread_cond_signal(&m_cond);
}

int condvar::broadcast() {
  return pthread_cond_broadcast(&m_cond);
}
