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
 * Interface/Abstrakte Klasse f�r Singletons
 **/

#ifndef SE2_SINGLETON_MGR_HPP
#define SE2_SINGLETON_MGR_HPP

#include "config.h"

#include "lib/hal/HWaccess.hpp"

#include "lib/util/mutex.hpp"
#include "lib/util/logging.hpp"
#include "lib/util/lock_guard.hpp"
#include "lib/util/abstract_singleton.hpp"
#include "lib/serial_bus/serial_channel.hpp"

namespace se2 {
namespace util {

enum module_type {
  HAL,
  LOGGER,
  SERIAL
};

class singleton_mgr {
  static mutex s_lock_hal;
  static mutex s_lock_log;
  static mutex s_lock_serial;
 public:
  /**
   * Zurgiff auf ein beliebiges Singleton Module
   * @param  `module_type` des angeforderten Modules
   * @return ein `abstract_singleton` pointer auf das Module
   **/
  static abstract_singleton* get_instance(module_type module);

  /**
   * Zerst�rt alle Singleton Module
   **/
  static void shutdown();
};

}
}

#define TO_HAL(ptr) static_cast<se2::hal::hwaccess*>(ptr)
#define TO_LOG(ptr) static_cast<se2::util::logging*>(ptr)
#define TO_SERIAL(ptr) static_cast<se2::serial_bus::serial_channel*>(ptr)

#endif // SE2_SINGLETON_MGR_HPP
