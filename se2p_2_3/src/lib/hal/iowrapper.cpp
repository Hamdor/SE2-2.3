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
 * Implementierung des Wrappers für QNX in8 und out8
 **/

#include "lib/hal/iowrapper.hpp"

#include <hw/inout.h>

using namespace se2::hal;

iowrapper::iowrapper() {
  // nop
}

iowrapper::~iowrapper() {
  // nop
}

void iowrapper::outbyte(enum port_num port, uint8_t val) {
  ::out8(static_cast<uint16_t>(port), val);
}

uint8_t iowrapper::inbyte(enum port_num port) {
  return ::in8(static_cast<uint16_t>(port));
}

void iowrapper::out1(enum port_num port, uint8_t pos, bool set) {
  const uint8_t bit = 0x01 << pos;
  if (set) {
    ::out8(static_cast<uint16_t>(port), ::in8(static_cast<uint16_t>(port)) | bit);
  } else {
    ::out8(static_cast<uint16_t>(port), ::in8(static_cast<uint16_t>(port)) & ~bit);
  }
}

uint8_t iowrapper::in1(enum port_num port, uint8_t bit) {
  const uint8_t mask = 0x01 << bit;
  return ::in8(static_cast<uint16_t>(port)) & mask;
}
