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
 * @file    abstract_singleton.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse für Singletons
 **/

#ifndef SE2_ABSTRACT_SINGLETON_HPP
#define SE2_ABSTRACT_SINGLETON_HPP

#include "config.h"

namespace se2 {
namespace util {

class abstract_singleton {
  /**
   * Initialisierung des Singletons
   **/
  virtual void initialize() = 0;

  /**
   * Zerstörung des Singleton
   **/
  virtual void destroy() = 0;
 public:
  virtual ~abstract_singleton() {
    // nop
  }
};

} // namespace util
} // namespace se2

#endif // SE2_ABSTRACT_SINGLETON_HPP
