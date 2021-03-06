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
 * @version 0.2
 *
 * Interface/Abstrakte Klasse fuer Singletons
 **/

#ifndef SE2_SINGLETON_MGR_HPP
#define SE2_SINGLETON_MGR_HPP

#include "config.h"

#include "lib/hal/HWaccess.hpp"
#include "lib/util/logging.hpp"
#include "lib/timer/timer_handler.hpp"
#include "lib/serial_bus/serial_channel.hpp"
#include "lib/dispatcher/dispatcher.hpp"
#include "lib/token_mgr.hpp"
#include "lib/util/light_mgr.hpp"

#include "lib/util/mutex.hpp"
#include "lib/util/lock_guard.hpp"
#include "lib/util/abstract_singleton.hpp"

namespace se2 {
namespace util {

enum module_type {
  HAL_PLUGIN,
  LOGGER_PLUGIN,
  SERIAL_PLUGIN,
  DISPATCHER_PLUGIN,
  TIMER_PLUGIN,
  TOKEN_PLUGIN,
  LIGHT_PLUGIN
};

class singleton_mgr {
  static mutex s_lock_hal;
  static mutex s_lock_log;
  static mutex s_lock_timer;
  static mutex s_lock_serial;
  static mutex s_lock_dispatcher;
  static mutex s_lock_token_mgr;
  static mutex s_lock_light_mgr;
 public:
  /**
   * Zurgiff auf ein beliebiges Singleton Module
   * @param  module Gibt den `module_type` des angeforderten Modules
   * @return Ein `abstract_singleton` Pointer auf das Module
   **/
  static abstract_singleton* get_instance(module_type module);

  /**
   * Zerstoert alle Singleton Module
   **/
  static void shutdown();

  /**
   * Erzwingt das erstellen eines noch nicht initialisierten
   * Singleton Modules. Sollte bei Modulen verwendet werden die
   * einen eigenen Thread haben
   * @param module Beschreibt das zu initialisierende Module
   **/
  static void force_initialization(module_type module);
};

}
}

#define TO_HAL(ptr) static_cast<se2::hal::hwaccess*>(ptr)
#define TO_LOG(ptr) static_cast<se2::util::logging*>(ptr)
#define TO_TIMER(ptr) static_cast<se2::timer::timer_handler*>(ptr)
#define TO_SERIAL(ptr) static_cast<se2::serial_bus::serial_channel*>(ptr)
#define TO_DISPATCHER(ptr) static_cast<se2::dispatch::dispatcher*>(ptr)
#define TO_TOKEN_MGR(ptr) static_cast<se2::token_mgr*>(ptr)
#define TO_LIGHT(ptr) static_cast<se2::util::light_mgr*>(ptr)

#endif // SE2_SINGLETON_MGR_HPP
