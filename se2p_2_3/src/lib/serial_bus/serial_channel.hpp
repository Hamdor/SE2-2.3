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
#include "lib/util/condvar.hpp"
#include "lib/util/HAWThread.hpp"
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
   * Default Konstruktor
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
   * Serial channel Instanz
   **/
  static serial_channel* instance;
  serial_interface* m_interface;

  /**
   * FIFO der Telegrams
   **/
  std::queue<telegram> m_queue;

  /**
   * Lock fuer einfuegen/entfernen aus Queue
   **/
  util::mutex   m_lock;

  /**
   * Condition Variable
   * Wird signalisiert sobald eine Nachricht in die
   * Queue kommt
   **/
  util::condvar m_cond;
 public:
  /**
   * Ruft das naechste Telegram ab und entfertnt es
   * @return naechstes Telegram
   **/
  telegram get_telegram();

  /**
   * Sendet ein Telegram
   **/
  void send_telegram(telegram* tel);
};

} // namepsace serial_bus
} // namespace se2

#endif // SE2_SERIAL_CHANNEL_HPP
