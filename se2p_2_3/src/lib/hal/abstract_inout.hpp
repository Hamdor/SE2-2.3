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
 * Interface für IO Zugriffe auf Ports
 **/

#ifndef SE2_ABSTRACT_INOUT_HPP
#define SE2_ABSTRACT_INOUT_HPP

#include "config.h"

#include "lib/constants.hpp"

#include <stdint.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#ifdef SIMULATION
  #include <ioaccess.h>
#endif

namespace se2 {
namespace hal {

struct abstract_inout {
  virtual ~abstract_inout() {
    // nop
  }

  /**
   * Schaltet die Ports auf Eingang oder Ausgang
   **/
  virtual void init_input_output() = 0;

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port Gibt den Port an
   * @param val  Gibt den Wert an
   **/
  virtual void outshort(enum port_num port, uint16_t val) = 0;

  /**
   * Liest einen Wert vom angegebenen Port
   * @return Den Wert des Ports
   **/
  virtual uint16_t inshort(enum port_num port) = 0;

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port Gibt den Port an
   * @param val  Gibt den Wert an
   **/
  virtual void outbyte(enum port_num port, uint8_t val) = 0;

  /**
   * Liest einen Wert vom angegebenen Port
   * @return Den Wert des Ports
   **/
  virtual uint8_t inbyte(enum port_num port) = 0;

  /**
   * Schreibt ein Bit auf die angegebene Position des
   * angegebenen Ports
   * @param port Gibt den Port an
   * @param pos Gibt das Bit an
   * @param set Gibt an ob das Bit auf 1 oder 0 gesetzt werden soll
   **/
  virtual void outbit(enum port_num port, uint8_t pos, bool set) = 0;

  /**
   * Liest einen Wert vom angegebenen Port
   * @param port Gibt den Port an
   * @param bit Gibt das Bit an
   * @return Den Wert des Ports
   **/
  virtual uint8_t inbit(enum port_num port, uint8_t bit) = 0;
};

} // namespace hal
} // namespace se2

#endif // SE2_ABSTRACT_INOUT_HPP
