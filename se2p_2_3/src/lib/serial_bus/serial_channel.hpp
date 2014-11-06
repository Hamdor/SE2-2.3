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

#include "lib/util/mutex.hpp"
#include "lib/util/abstract_singleton.hpp"

#include <stdint.h>

namespace se2 {
namespace util {
class singleton_mgr;
} // namespace util
namespace serial_bus {

enum telegram_type {
  SYNC = 0,  // Zurücksetzen
  MSG  = 1,  // Nachricht (keine Daten)
  DATA = 2,  // Daten werden übertragen
  ACK  = 3,  // erfolgreich übertragen
  ERR  = 4   // letzte packet neu senden
};

enum msg_type {
  ERR_STOP = 0,  // Fehler auf einem Band, stoppen
  ERR_QUIT = 1,  // Fehler quittiert
  RESUME   = 2,  // Weiterlaufen/Start
  B2_FREE  = 3,  // Band 2 wieder frei von Puk
  E_STOP   = 4,  // Estop gedrückt
  STOP     = 5   // Stop taste gedrückt
};

enum data_type { // typen die für typename T erlaubt sind
  PUK,           // PUK/Tocken Datentyp
  INT,           // 32 bit Integer
  FLOAT          // 32 bit Float
};

struct telegram_header {
  telegram_type m_type; // (SYNC, MSG, ACK, DATA, ERROR)
  size_t        m_len;  // länge des bodys
};

template <typename T>
struct telegram_body {
  msg_type m_msg;  // Nachricht (optional bei Daten)
  T m_data;        // Daten (optional bei Message)
};

template <typename T>
struct telegram {
  telegram_header  m_header; // Header
  telegram_body<T> m_body;   // Body
  uint32_t         m_crc;    // CRC über ganze telegram
};

#define TELEGRAM_SIZE sizeof(telegram)

class serial_channel : public util::abstract_singleton {
  friend class util::singleton_mgr;
  serial_channel();
  virtual ~serial_channel();
  static serial_channel* instance;
  virtual void initialize();
  virtual void destroy();
};

} // namepsace serial_bus
} // namespace se2

#endif // SE2_SERIAL_CHANNEL_HPP
