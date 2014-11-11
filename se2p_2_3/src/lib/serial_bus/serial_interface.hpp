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
 * @file    serial_interface.hpp
 * @version 0.1
 *
 * Serielle Schnittstelle
 **/

#ifndef SE2_SERIAL_INTERFACE_HPP
#define SE2_SERIAL_INTERFACE_HPP

#include "config.h"

#include "lib/util/logging.hpp"
#include <unistd.h>

namespace se2 {
namespace serial_bus {

class serial_channel;
/**
 * Zugriff auf `serial_interface` nur durch `serial_channel`
 **/
class serial_interface {
  friend serial_channel;
 private:
  /**
   * Default Konstruktor  
   **/
  serial_interface();

  /**
   * Default Destruktor
   **/
  ~serial_interface();

  /**
   * Schreibt Daten auf den Seriellen bus
   * @param data gibt die zu schreibenden Daten an
   * @return TRUE  wenn erfolgreich 
   *         FALSE wenn fehlschlägt
   *         FALSE wenn ohne `HAS_SERIAL_INTERFACE` kompiliert
   **/
  template <typename T>
  bool write(T* data) {
#ifdef HAS_SERIAL_INTERFACE
    size_t rc = 0;
    while (rc != sizeof(T)) {
      ssize_t err = ::write(m_fd, reinterpret_cast<char*>(data) + rc, sizeof(T) - rc);
      if (err == -1) {
        perror("serial_interface::write()");
        break;
      }
      rc += err;
    }
    return rc == sizeof(T);
#else
    return false;
#endif
  }

  /**
   * Schreibt Daten auf den Seriellen bus
   * @param data gibt die zu schreibenden Daten an
   * @return TRUE  wenn erfolgreich  
   *         FALSE wenn fehlschlägt
   *         FALSE wenn ohne `HAS_SERIAL_INTERFACE` kompiliert
   **/
  template <typename T>
  bool read(T* buffer) {
#ifdef HAS_SERIAL_INTERFACE
    size_t rc = 0;
    while (rc != sizeof(T)) {
      ssize_t err = ::read(m_fd, reinterpret_cast<char*>(buffer) + rc, sizeof(T) - rc);
      if (err == -1) {
        perror("serial_interface::read()");
        break;
      }
      rc += err;
    }
    return rc == sizeof(T);
#else
    return false;
#endif
  }

 private:
  int m_fd;

  /**
   * Konfiguriert die Serielle Schnittstelle
   **/
  void config();
};

} // namespace serial_bus
} // namespace se2

#endif // SE2_SERIAL_INTERFACE_HPP
