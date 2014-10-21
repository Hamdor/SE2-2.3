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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class serial_interface{
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
   * @return TRUE  wenn erfolgreich 
   *         FALSE wenn fehlschlägt
   **/
  template<typename T>
  bool write(T* data);

  /**
   * Schreibt Daten auf den Seriellen bus
   * @param data gibt die zu schreibenden Daten an
   * @return TRUE  wenn erfolgreich  
   *         FALSE wenn fehlschlägt
   **/
  template<typename T>
  bool read(T* buffer);

 private:
  int m_fd;
};

#endif // SE2_SERIAL_INTERFACE_HPP
