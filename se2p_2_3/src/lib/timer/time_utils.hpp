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
 * @file    time_utils.hpp
 * @version 0.1
 *
 * Time Utilities, abrufen von Systemzeiten, Rechnen mit Systemzeiten.
 **/

#ifndef SE2_TIME_UTILS_HPP
#define SE2_TIME_UTILS_HPP

#include <time.h>
#include <unistd.h>

namespace se2 {
namespace timer {

struct time_utils {
  /**
   * Addiert zwei `timespec` und gibt den Wert in einem neuen
   * `timespec` zurueck
   * @param a Erster `timespec`
   * @param b Zweiter `timespec`
   * @return Ein neuer `timespec` der `a + b` darstellt.
   **/
  static timespec add(timespec a, timespec b);

  /**
   * Bildet die differenz von zwei `timespec` und  gibt den
   * Wert in einem neuen `timespec` zurueck
   * @param begin Beginnender Zeitpunkt (muss juenger sein als `end`)
   * @param end   Endzeitpunkt (muss aelter sein als `start`)
   * @return Ein neuer `timespec` der die differenz darstellt
   **/
  static timespec diff(timespec begin, timespec end);

  /**
   * Substrahiert zwei `timespec` und gibt den Wert in einem neuen
   * `timespec` zurueck
   * @param a Erster `timespec`
   * @param b Zweiter `timespec`
   * @return Ein neuer `timespec` der die subtraktion darstellt
   **/
  static timespec sub(timespec a, timespec b);

  /**
   * Prueft ob `smaller` kleiner als `bigger` ist
   * @param smaller Der vermutet kleinere `timespec`
   * @param bigger Der vermutet groessere `timespec`
   * @return TRUE  wenn `smaller` kleiner als `bigger`
   *         FALSE wenn `smaller` nicht kleiner als `bigger`
   **/
  static bool smaller_then(timespec smaller, timespec bigger);
};

} // namespace timer
} // namespace se2

#endif // SE2_TIME_UTILS_HPP
