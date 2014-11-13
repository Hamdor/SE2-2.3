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
 * @file    iostub.hpp
 * @version 0.1
 *
 * Stub für IO Zugriffe auf Ports
 **/

#ifndef SE2_IOSTUB_HPP
#define SE2_IOSTUB_HPP

#include "config.h"

#include "lib/hal/abstract_inout.hpp"

namespace se2 {
namespace hal {

struct iostub : public abstract_inout {
  /**
   * Default Konstruktor
   **/
  iostub(uint8_t portA, uint8_t portB, uint8_t portC);

  /**
   * Default Destruktor
   **/
  ~iostub();

  /**
   * Schaltet die Ports auf Eingang oder Ausgang
   **/
  void init_input_output();

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port gibt den port an
   * @param val  gibt den wert an
   **/
  void outshort(enum port_num port, uint16_t val);

  /**
   * Liest einen Wert vom angegebenen Port
   * @return den Wert des Ports
   **/
  uint16_t inshort(enum port_num port);

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port gibt den port an
   * @param val  gibt den wert an
   **/
  void outbyte(enum port_num port, uint8_t val);

  /**
   * Liest einen Wert vom angegebenen Port
   * @return den Wert des Ports
   **/
  uint8_t inbyte(enum port_num port);

  /**
   * Schreibt ein Bit auf die angegebene Position des
   * angegebenen Ports
   * @param port gibt den Port an
   * @param pos gibt das Bit an
   * @param set gibt an ob das Bit gesetzt werden soll
   * @return den Wert des Ports
   **/
  void outbit(enum port_num port, uint8_t pos, bool set);

  /**
   * Liest einen Wert vom angegebenen Port
   * @param port gibt den Port an
   * @param bit gibt das Bit an
   * @return den Wert des Ports
   **/
  uint8_t inbit(enum port_num port, uint8_t bit);

 private:
  uint8_t m_port_a;
  uint8_t m_port_b;
  uint8_t m_port_c;
};

} // namespace hal
} // namespace se2

#endif // SE2_IOSTUB_HPP
