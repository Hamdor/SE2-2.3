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
 * @file    logging.cpp
 * @version 0.1
 *
 * Helper Klasse ausgeben/schreiben von Logging
 **/

#include "lib/util/logging.hpp"
#include "lib/util/lock_guard.hpp"

#include <sstream>
#include <iostream>

#include <unistd.h>

using namespace se2::util;

logging* logging::instance = NULL;
mutex    logging::s_lock;

namespace {
  std::string output_file = "/root/logs/log_";
} // namespace <anonymous>

logging::logging() : m_fstream(), m_lock() {
  std::ostringstream fname;
  fname << output_file << getpid() << "_" << time(0) << ".log";
  m_fstream.open(fname.str().c_str(), std::ios::out | std::ios::app);
  m_lock.acquire();
  m_fstream << "[Started logfile]" << std::endl;
  m_lock.release();
}

void logging::log(const char* str, loglevel lvl,
                  const char* file_name, int line_num) {
  if (!m_fstream.is_open()) {
    std::cerr << "Log file already closed!" << std::endl
              << "Log message:"             << std::endl
              << str                        << std::endl;
  }
  std::string prefix;
  switch (lvl) {
  case TRACE:
    prefix = "[TRACE]";
    break;
  case DEBUG:
    prefix = "[DEBUG]";
    break;
  case WARNING:
    prefix = "[WARNING]";
    break;
  case ERROR:
    prefix = "[ERROR]";
    break;
  default:
    prefix = "[UNKOWN]";
    break;

  }
  std::ostringstream ss;
  ss << prefix
     << " file: " << file_name
     << ":"       << line_num;
  m_lock.acquire();
  m_fstream << ss.str() << " " << str << std::endl;
  if (lvl != TRACE) {
    std::cerr << ss.str() << " " << str << std::endl;
  }
  m_lock.release();
}

logging::~logging() {
  m_lock.acquire();
  m_fstream << "[END Logfile]" << std::endl;
  m_fstream.close();
  m_lock.release();
}

logging* logging::get_instance() {
  if (!instance) {
    lock_guard guard(s_lock);
    if (!instance) {
      instance = new logging();
    }
  }
  return instance;
}
