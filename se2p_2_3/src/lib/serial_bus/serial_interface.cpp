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

#include "lib/serial_bus/serial_interface.hpp"

#include <termios.h>
#include <fcntl.h>
#include <stdio.h>

using namespace se2;

namespace {
  const char*  char_device = "/dev/ser1";
} // namespace <anonymous>

serial_interface::serial_interface() {
  m_fd = ::open(char_device, O_RDWR);
  if (m_fd == -1) {
    perror("serial_interface::serial_interface() cant open file!");
    return;
  }
  config();
}

serial_interface::~serial_interface() {
  if (close(m_fd) == -1) {
    perror("serial_interface::~serial_interface() cant close file!");
  }
}

bool serial_interface::read(void* buffer, size_t len) {
  size_t rc = 0;
  while (rc != len) {
    ssize_t err = ::read(m_fd, static_cast<size_t*>(buffer) + rc, len - rc);
    if (err == -1) {
      perror("serial_interface::read()");
      break;
    }
    rc += err;
  }
  return rc == len;
}

bool serial_interface::write(void* data, size_t len) {
  size_t rc = 0;
  while (rc != len) {
    ssize_t err = ::write(m_fd, static_cast<size_t*>(data) + rc, len - rc);
    if (err == -1) {
      perror("serial_interface::write()");
      break;
    }
    rc += err;
  }
  return rc == len;
}

void serial_interface::config() {
  struct termios ts;
  tcflush(m_fd, TCIOFLUSH);
  tcgetattr(m_fd, &ts);
  cfsetispeed(&ts, B19200);
  cfsetospeed(&ts, B19200);
  ts.c_cflag &= ~CSIZE;
  ts.c_cflag &= ~CSTOPB;
  ts.c_cflag &= ~PARENB;
  ts.c_cflag |= CS8;
  ts.c_cflag |= CREAD;
  ts.c_cflag |= CLOCAL;
  tcsetattr(m_fd, TCSANOW, &ts);
}
