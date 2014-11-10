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
using namespace se2::serial_bus;

#ifdef HAS_SERIAL_INTERFACE
namespace {
  const char* char_device = SERIAL_CHAR_DEV;
} // namespace <anonymous>
#endif

serial_interface::serial_interface() {
#ifdef HAS_SERIAL_INTERFACE
  m_fd = ::open(char_device, O_RDWR);
  if (m_fd == -1) {
    LOG_ERROR("serial_interface::serial_interface() cant open file!")
    return;
  }
  config();
#else
  m_fd = 0;
#endif
}

serial_interface::~serial_interface() {
#ifdef HAS_SERIAL_INTERFACE
  if (close(m_fd) == -1) {
    LOG_ERROR("serial_interface::~serial_interface() cant close file!")
  }
#endif
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
