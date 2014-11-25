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
 * @file    singleton_mgr.cpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse f�r Singletons
 **/

#include "lib/util/singleton_mgr.hpp"

using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::timer;
using namespace se2::serial_bus;

mutex singleton_mgr::s_lock_hal;
mutex singleton_mgr::s_lock_log;
mutex singleton_mgr::s_lock_timer;
mutex singleton_mgr::s_lock_serial;
mutex singleton_mgr::s_lock_dispatcher;

abstract_singleton* singleton_mgr::get_instance(module_type module) {
  if (module == HAL_PLUGIN) {
    if (ThreadCtl(_NTO_TCTL_IO_PRIV, NULL) == -1) {
      LOG_ERROR("ThreadCtl() failed!")
    }
    if (!hwaccess::instance) {
      lock_guard guard(s_lock_hal);
      if (!hwaccess::instance) {
        hwaccess::instance = new hwaccess();
        hwaccess::instance->initialize();
      }
    }
    return hwaccess::instance;
  } else if (module == LOGGER_PLUGIN) {
    if (!logging::instance) {
      lock_guard guard(s_lock_log);
      if (!logging::instance) {
        logging::instance = new logging();
        logging::instance->initialize();
      }
    }
    return logging::instance;
  } else if (module == SERIAL_PLUGIN) {
    if (!serial_channel::instance) {
      lock_guard guard(s_lock_serial);
      if (!serial_channel::instance) {
        serial_channel::instance = new serial_channel();
        serial_channel::instance->initialize();
      }
    }
    return serial_channel::instance;
  } else if (module == DISPATCHER_PLUGIN) {
    if (!dispatcher::instance) {
      lock_guard guard(s_lock_dispatcher);
      if (!dispatcher::instance) {
        dispatcher::instance = new dispatcher();
        dispatcher::instance->initialize();
      }
    }
    return dispatcher::instance;
  } else if (module == TIMER_PLUGIN) {
    if (!timer_handler::instance) {
      lock_guard guard(s_lock_timer);
      if(!timer_handler::instance) {
        timer_handler::instance = new timer_handler();
        timer_handler::instance->initialize();
      }
    }
    return timer_handler::instance;
  }
  return NULL;
}

void singleton_mgr::shutdown() {
  // Shutdown all Threads
  HAWThread::shutdownAll();
  if (dispatcher::instance) {
    dispatcher::instance->destroy();
    delete dispatcher::instance;
  }
  if (serial_channel::instance) {
    serial_channel::instance->destroy();
    delete serial_channel::instance;
  }
  if (hwaccess::instance) {
    hwaccess::instance->destroy();
    delete hwaccess::instance;
  }
  if (logging::instance) {
    logging::instance->destroy();
    delete logging::instance;
  }
  if (timer_handler::instance) {
    timer_handler::instance->destroy();
    delete timer_handler::instance;
  }
}
