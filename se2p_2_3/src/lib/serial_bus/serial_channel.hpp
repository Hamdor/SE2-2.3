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
 * @file    serial_channel.hpp
 * @version 0.1
 *
 * Logik Ebene der Seriellen Schnittstelle
 **/

#ifndef SE2_SERIAL_CHANNEL_HPP
#define SE2_SERIAL_CHANNEL_HPP

#include "config.h"
#include "lib/constants.hpp"
#include "lib/util/HAWThread.hpp"
#include "lib/util/mutex.hpp"
#include "lib/util/abstract_singleton.hpp"

#include <queue>
#include <stdint.h>

namespace se2 {
namespace util {
class singleton_mgr;
} // namespace util
namespace serial_bus {

class serial_interface;

#define TELEGRAM_SIZE sizeof(telegram)

class serial_channel : public util::abstract_singleton
                     , public util::HAWThread {
  friend class util::singleton_mgr;
  /**
   * Konstruktor
   **/
  serial_channel();

  /**
   * Default Destruktor
   **/
  virtual ~serial_channel();

  virtual void initialize();
  virtual void destroy();
  virtual void execute(void*);
  virtual void shutdown();

  /**
   * Serial channel instanz
   **/
  static serial_channel* instance;
  serial_interface* m_interface;

  /**
   * FIFO der telegrams
   **/
  std::queue<telegram> m_queue;
 public:
  /**
   * Ruft das naechste telegram ab und entfertnt es
   * @return next telegram
   **/
  telegram get_telegram();
};

} // namepsace serial_bus
} // namespace se2

#endif // SE2_SERIAL_CHANNEL_HPP
