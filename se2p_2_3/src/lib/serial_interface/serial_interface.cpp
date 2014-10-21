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
 * @file    serial_interface.cpp
 * @version 0.1
 *
 * Implementierung der Seriellen Schnittstelle
 **/

#include "lib/serial_interface/serial_interface.hpp"
#include <cerrno>

namespace {
  const char*  char_device = "dev/ser1";
} // namespace <anonymous>

serial_interface::serial_interface() {
  m_fd = ::open(char_device, 0);
  if (m_fd == -1) {
    perror("serial_interface::serial_interface() cant open file!");
  }
}

serial_interface::~serial_interface() {
  if (close(m_fd) == -1) {
    perror("serial_interface::~serial_interface() cant close file!");
  }
}

template<typename T>
bool serial_interface::read(T* buffer) {
  int rc = 0;
  while (rc != sizeof(T)) { 
    int err = ::read(m_fd, static_cast<void*>(buffer) + rc, sizeof(T) - rc);
    if (err == -1) {
      perror("serial_interface::read()");
      break;
    }
    rc += err;
  }
  return rc == sizeof(T);
}

template<typename T>
bool serial_interface::write(T* data) {
  int rc = 0;
  while ( rc != sizeof(T)) {
    int err = ::write(m_fd, static_cast<void*>(data) + rc, sizeof(T) - rc);
    if (err == -1) {
      perror("serial_interface::write()");
      break;
    }
    rc += err;
  }
  return rc == sizeof(T);
}
