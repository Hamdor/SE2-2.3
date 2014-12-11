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
 * @file    main.cpp
 * @version 0.3
 *
 * Beinhaltet die Main Methode, sowie die Start Routine des Hauptprogrammes.
 **/

#include "config.h"
#include "initialization_run.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/util/HAWThread.hpp"

#include <unistd.h>

using namespace se2;
using namespace se2::util;

#define MAIN_LOOP_SLEEP 250000

int main(int argc, char *argv[]) {
#ifdef SIMULATION
  IOaccess_open();
#endif
  // Initialisierung (Hoehenwerte/Zeitwerte)
  initialization_run init;
  init.start_init();
  hal::hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  if (!hal->is_button_pressed(hal::BUTTON_ESTOP)) {
    // Hauptprogram
    singleton_mgr::force_initialization(SERIAL_PLUGIN);
    singleton_mgr::force_initialization(LIGHT_PLUGIN);
    TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN))->set_state(READY_TO_USE);
    singleton_mgr::force_initialization(TOKEN_PLUGIN);
    while(1) {
      usleep(MAIN_LOOP_SLEEP);
      if (HAWThread::is_global_shutdown()) {
        break;
      }
    }
  }
  singleton_mgr::shutdown();
#ifdef SIMULATION
  IOaccess_close();
#endif
  std::cout << "End of main()" << std::endl;
  return 0;
}
