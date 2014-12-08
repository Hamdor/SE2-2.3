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
 * @file    initialization_run.hpp
 * @version 0.1
 *
 * In diesem Modul werden HW-Laufzeiten sowie Hoehensensorwerte
 * ausgelesen um eine Konfiguration des Hauptprogramms vorzunehmen
 **/

#ifndef SE2_INITIALIZATION_RUN_HPP
#define SE2_INITIALIZATION_RUN_HPP

#include "config.h"
#include <unistd.h>

namespace se2 {

struct my_times {
  size_t sec;
  size_t msec;
};

class initialization_run {
  /**
   * Program zum auslesen des Hoehenwerte Sensors
   * Benoetigte Puks werden nacheinander auf den Anfang
   * des Bandes gelegt und durch die Hoehenmessung gefahren.
   **/
  void get_heights();

  /**
   * Program zum Messen der einzelnen Zeitwerte
   * Puk auf den Anfang des Bandes legen und warten bis
   * der Puk das Ende des Bandes erreicht hat.
   **/
  void get_times();

  /**
   * Holt die aktuelle Zeit und macht eine differenz zur letzten Zeit
   * @return `my_times` enthaellt die verstrichenen Sekunden sowie
   *         die verstrichenen Millisekunden.
   **/
  my_times get_curr_time();

  /**
   * Gibt einen Zeitwert auf der Konsole aus.
   * @param time ist ein Array auf die Zeit werte
   **/
  void print_time(const my_times time[]);

  /**
   * Startet alle Unit-Tests
   **/
  void run_tests();
 public:
  /**
   * Startet die Unterprogramme der Initialisierung, dazu gehoeren:
   * - `run_tests`   (Anstarten der Unit-Tests)
   * - `get_heights` (Auslesen der Hoehenwerte)
   * - `get_times`   (Auslesen der Zeitwerte)
   * Die Programme werden sequentiell gestartet.
   **/
  void start_init();
};

} // namespace se2

#endif // SE2_INITIALIZATION_RUN_HPP
