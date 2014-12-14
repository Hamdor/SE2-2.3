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
 * @file    logging.hpp
 * @version 0.1
 *
 * Helper Klasse ausgeben/schreiben von Logging
 **/

#ifndef SE2_LOGGING_HPP
#define SE2_LOGGING_HPP

#include "config.h"

#include "lib/constants.hpp"
#include "lib/util/mutex.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/util/abstract_singleton.hpp"

#include <fstream>

namespace se2 {
namespace util {

class singleton_mgr;

class logging : public abstract_singleton {
  friend singleton_mgr;

  /**
   * Default Konstruktor
   **/
  logging();

  static logging* instance;

  /**
   * Initialisierung des Singletons
   **/
  virtual void initialize();

  /**
   * Zerst�rung des Singleton
   **/
  virtual void destroy();

 public:
  /**
   * Fügt einen neuen Eintrag zum Log hinzu.
   * @param str Definiert die Log Message
   * @param lvl Definiert das Log Level
   * @param file_name Der Aufrufer (File)
   * @param line_num Zeilennummer (Aufrufer)
   * @param fun_name name der Funktion (Aufrufer)
   **/
  void log(const char* str, loglevel lvl, const char* file_name, int line_num,
           const char* fun_name);

  /**
   * Default Destruktor
   * @warning Muss aufgerufen werden, da log file descriptor
   *          sonst nicht geschlossen wird.
   **/
  ~logging();

 private:
  std::ofstream m_fstream;
  mutex         m_lock;
};

} // namespace util
} // namespace se2

/**
 * Zugriff auf den LOG bitte nur durch diese Makros!
 **/

#define LOG_TRACE(str)                                                        \
    static_cast<se2::util::logging*>(se2::util::singleton_mgr::get_instance(  \
                   se2::util::LOGGER_PLUGIN))->log(str, se2::util::TRACE,     \
                          __FILE__, __LINE__, __FUNCTION__);

#define LOG_DEBUG(str)                                                        \
  static_cast<se2::util::logging*>(se2::util::singleton_mgr::get_instance(    \
                   se2::util::LOGGER_PLUGIN))->log(str, se2::util::DEBUG,     \
                                         __FILE__, __LINE__, __FUNCTION__);

#define LOG_WARNING(str)                                                      \
  static_cast<se2::util::logging*>(se2::util::singleton_mgr::get_instance(    \
                   se2::util::LOGGER_PLUGIN))->log(str, se2::util::WARNING,   \
                                         __FILE__, __LINE__, __FUNCTION__);

#define LOG_ERROR(str)                                                        \
  static_cast<se2::util::logging*>(se2::util::singleton_mgr::get_instance(    \
                   se2::util::LOGGER_PLUGIN))->log(str,              \
                   se2::util::ERROR, __FILE__, __LINE__, __FUNCTION__);
#endif // SE2_LOGGING_HPP
