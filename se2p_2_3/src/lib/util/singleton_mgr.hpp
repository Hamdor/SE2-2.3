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
 * @file    abstract_singleton.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse für Singletons
 **/

#ifndef SE2_SINGLETON_MGR_HPP
#define SE2_SINGLETON_MGR_HPP

#include "lib/hal/HWaccess.hpp"

#include "lib/util/mutex.hpp"
#include "lib/util/logging.hpp"
#include "lib/util/lock_guard.hpp"
#include "lib/util/abstract_singleton.hpp"

namespace se2 {
namespace util {

enum module_type {
  HAL,
  LOGGER
};

class singleton_mgr {
  static mutex s_lock_hal;
  static mutex s_lock_log;
 public:
  /**
   * Zurgiff auf ein beliebiges Singleton Module
   * @param  `module_type` des angeforderten Modules
   * @return ein `abstract_singleton` pointer auf das Module
   **/
  static abstract_singleton* get_instance(module_type module);

  /**
   * Zerstört alle Singleton Module
   **/
  static void shutdown();
};

}
}

#endif // SE2_SINGLETON_MGR_HPP
