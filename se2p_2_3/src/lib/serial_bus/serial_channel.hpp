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

#include "token.hpp"

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

enum telegram_type {
  MSG  = 0,  // Nachricht (keine Daten)
  DATA = 1,  // Daten werden übertragen
  ERR  = 2   // letzte packet neu senden
};

enum msg_type {
  ERR_STOP = 0,  // Fehler auf einem Band, stoppen
  ERR_QUIT = 1,  // Fehler quittiert
  RESUME   = 2,  // Weiterlaufen/Start
  B2_FREE  = 3,  // Band 2 wieder frei von Puk
  E_STOP   = 4,  // Estop gedrückt
  STOP     = 5   // Stop taste gedrückt
};

struct telegram {
  telegram_type m_type; // (SYNC, MSG, ACK, DATA, ERROR)
  msg_type      m_msg;  // Nachricht (optional bei Daten)
  token         m_data; // Daten (optional bei Message)
};

#define TELEGRAM_SIZE sizeof(telegram)

class serial_channel : public util::abstract_singleton
                     , public util::HAWThread {
  friend class util::singleton_mgr;
  serial_channel();
  virtual ~serial_channel();
  static serial_channel* instance;

  virtual void initialize();
  virtual void destroy();

  virtual void execute(void*);
  virtual void shutdown() {
    // nop
  }

  serial_interface* m_interface;
  std::queue<telegram> m_queue;
};

} // namepsace serial_bus
} // namespace se2

#endif // SE2_SERIAL_CHANNEL_HPP
