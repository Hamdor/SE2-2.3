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
 * @file    abstract_inout.hpp
 * @version 0.1
 *
 * Wrapper für QNX in8 und out8
 **/

#ifndef SE2_IOWRAPPER_HPP
#define SE2_IOWRAPPER_HPP

#include "config.h"

#include "lib/hal/abstract_inout.hpp"

namespace se2 {
namespace hal {

struct iowrapper : public abstract_inout {
  /**
   * Default Konstruktor
   **/
  iowrapper();

  /**
   * Default Destruktor
   **/
  ~iowrapper();

  /**
   * Schaltet die Ports auf Eingang oder Ausgang
   **/
  void init_input_output();

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port Gibt den Port an
   * @param val  Gibt den Wert an
   **/
  void outshort(enum port_num port, uint16_t val);

  /**
   * Liest einen Wert vom angegebenen Port
   * @return Den Wert des Ports
   **/
  uint16_t inshort(enum port_num port);

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port Gibt den Port an
   * @param val  Gibt den Wert an
   **/
  void outbyte(enum port_num port, uint8_t val);

  /**
   * Liest einen Wert vom angegebenen Port
   * @return Den Wert des Ports
   **/
  uint8_t inbyte(enum port_num port);

  /**
   * Schreibt ein Bit auf die angegebene Position des
   * angegebenen Ports
   * @param port Gibt den Port an
   * @param pos Gibt das Bit an
   * @param set Gibt an ob das Bit gesetzt werden soll
   **/
  void outbit(enum port_num port, uint8_t pos, bool set);

  /**
   * Liest einen Wert vom angegebenen Port
   * @param port Gibt den Port an
   * @param bit Gibt das Bit an
   * @return Wenn Positiv, dann ist das Bit gesetzt
   *         Wenn 0, dann ist das Bit nicht gesetzt
   **/
  uint8_t inbit(enum port_num port, uint8_t bit);
};

} // namespace hal
} // namespace se2

#endif // SE2_IOWRAPPER_HPP
