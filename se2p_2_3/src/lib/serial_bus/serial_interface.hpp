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

#include "lib/util/logging.hpp"
#include <unistd.h>

namespace se2 {

class serial_interface {
 public:  
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
   * @param len  gibt die Länge des Datentypens an
   * @return TRUE  wenn erfolgreich 
   *         FALSE wenn fehlschlägt
   **/
  template <typename T>
  bool write(T* data) {
    size_t rc = 0;
    while (rc != sizeof(T)) {
      ssize_t err = ::write(m_fd, data + rc, sizeof(T) - rc);
      if (err == -1) {
        LOG_ERROR("serial_interface::write()")
        break;
      }
      rc += err;
    }
    return rc == sizeof(T);
  }

  /**
   * Schreibt Daten auf den Seriellen bus
   * @param data gibt die zu schreibenden Daten an
   * @param len  gibt die Länge des Datentypens an
   * @return TRUE  wenn erfolgreich  
   *         FALSE wenn fehlschlägt
   **/
  template <typename T>
  bool read(T* buffer) {
    size_t rc = 0;
    while (rc != sizeof(T)) {
      ssize_t err = ::read(m_fd, buffer + rc, sizeof(T) - rc);
      if (err == -1) {
        LOG_ERROR("serial_interface::read()")
        break;
      }
      rc += err;
    }
    return rc == sizeof(T);
  }

 private:
  int m_fd;

  /**
   * Konfiguriert die Serielle Schnittstelle
   **/
  void config();
};

} // namespace se2

#endif // SE2_SERIAL_INTERFACE_HPP
