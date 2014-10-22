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
 * @file    iostub.cpp
 * @version 0.1
 *
 * Implmenentierung iostub
 **/

#include "lib/hal/iostub.hpp"

using namespace se2::hal;

iostub::iostub(uint8_t portA, uint8_t portB, uint8_t portC)
    : m_port_a(portA),
      m_port_b(portB),
      m_port_c(portC) {
  // nop
}

iostub::~iostub() {
  // nop
}

void iostub::init_input_output() {

}

void iostub::outshort(enum port_num port, uint16_t val) {

}

uint16_t iostub::inshort(enum port_num port) {

}

void iostub::outbyte(enum port_num port, uint8_t val) {

}

uint8_t iostub::inbyte(enum port_num port) {

}

void iostub::outbit(enum port_num port, uint8_t pos, bool set) {

}

uint8_t iostub::inbit(enum port_num port, uint8_t bit) {

}
