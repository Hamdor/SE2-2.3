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
 * @file    time_utils.cpp
 * @version 0.1
 *
 * Time Utilities, abrufen von Systemzeiten, Rechnen mit Systemzeiten.
 **/

#include "lib/timer/time_utils.hpp"

using namespace se2;
using namespace se2::timer;

#define MILISEC_TO_NANOSEC 1000000
#define SEC_TO_NANOSEC     1000000000

timespec time_utils::add(timespec a, timespec b) {
  long sec  = a.tv_sec  + b.tv_sec;
  long nsec = a.tv_nsec + b.tv_nsec;
  if (nsec >= SEC_TO_NANOSEC) {
      nsec -= SEC_TO_NANOSEC;
      ++sec;
  }
  timespec temp;
  temp.tv_sec  = sec;
  temp.tv_nsec = nsec;
  return temp;
}

timespec time_utils::diff(timespec begin, timespec end) {
  timespec tempspec;
  if ((end.tv_nsec - begin.tv_nsec) < 0) {
    tempspec.tv_sec  = end.tv_sec - begin.tv_sec - 1;
    tempspec.tv_nsec = SEC_TO_NANOSEC + end.tv_nsec - begin.tv_nsec;
  } else {
    tempspec.tv_sec  = end.tv_sec  - begin.tv_sec;
    tempspec.tv_nsec = end.tv_nsec - begin.tv_nsec;
  }
  if (tempspec.tv_sec == unsigned(0xFFFFFFFF)) {
    tempspec.tv_sec = 0;
  }
  return tempspec;
}

timespec time_utils::sub(timespec a, timespec b) {
  return diff(a, b);
}

bool time_utils::smaller_then(timespec smaller, timespec bigger) {
  timespec diffspec = diff(bigger, smaller);
  if (diffspec.tv_sec > bigger.tv_sec && diffspec.tv_sec > smaller.tv_sec) {
    return true;
  } else if (diffspec.tv_sec < bigger.tv_sec) {
    return true;
  } else {
    if (diffspec.tv_sec == 0) {
      if (bigger.tv_nsec > smaller.tv_nsec) {
        return true;
      } else {
        return false;
      }
    }
    return false;
  }
  return true;
}
