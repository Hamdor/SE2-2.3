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

namespace se2 {
namespace hal {

#define IOBASE 0x300

enum port_num {
  PORTA = IOBASE + 0x00, // IOBASE + 0x00
  PORTB = IOBASE + 0x01, // IOBASE + 0x01
  PORTC = IOBASE + 0x02  // IOBASE + 0x02
};

struct abstract_inout {
  virtual ~abstract_inout() {
    // nop
  }

  /**
   * Schreibt einen Wert auf den angegebenen Port
   * @param port gibt den port an
   * @param val  gibt den wert an
   **/
  virtual void out8(enum port_num port, uint8_t val) = 0;

  /**
   * Liest einen Wert vom angegebenen Port
   * @return den Wert des Ports
   **/
  virtual uint8_t in8(enum port_num port) = 0;


  /**
   * Schreibt ein Bit auf die angegebene Position des
   * angegebenen Ports
   * @param
   **/
  virtual void out1(enum port_num port, uint8_t pos) = 0;

  /**
   * Liest einen Wert vom angegebenen Port
   * @param port gibt den Port an
   * @param gibt das Bit an
   * @return den Wert des Ports
   **/
  virtual uint8_t in1(enum port_num port, uint8_t bit) = 0;
};

} // namespace hal
} // namespace se2

#endif // SE2_ABSTRACT_INOUT_HPP
