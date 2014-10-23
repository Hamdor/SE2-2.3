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
#include "lib/hal/HWaccess.hpp" // for defines

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
  // nop
}

void iostub::outshort(enum port_num port, uint16_t val) {
  outbyte(port, val);
}

uint16_t iostub::inshort(enum port_num port) {
  return inbyte(port);
}

void iostub::outbyte(enum port_num port, uint8_t val) {
  if (port == PORTA) {
    m_port_a = val;
  } else if (port == PORTB) {
    m_port_b = val;
  } else {
    m_port_c = val;
  }
}

uint8_t iostub::inbyte(enum port_num port) {
  if (port == PORTA) {
    return m_port_a;
  } else if (port == PORTB) {
    return m_port_b;
  } else {
    return m_port_c;
  }
}

void iostub::outbit(enum port_num port, uint8_t pos, bool set) {
  const uint8_t mask = 0x01 << pos;
  if (port == PORTA) {
    if (set) {
      if (pos == SWITCH_BIT) {
        /**
         * Das Bit muss auch gesetzt werden, da der Switch
         * noch einen 2ten anschluss besitzt
         **/
        m_port_b &= ~(0x01 << SWITCH_OPEN_BIT);
      }
      m_port_a |= mask;
    } else {
      if (pos == SWITCH_BIT) {
        /**
         * Das Bit muss auch gesetzt werden, da der Switch
         * noch einen 2ten anschluss besitzt
         **/
        m_port_b |= (0x01 << SWITCH_OPEN_BIT);
      }
      m_port_a &= ~mask;
    }
  } else if (port == PORTB) {
    if (set) {
      m_port_b |= mask;
    } else {
      m_port_b &= ~mask;
    }
  } else {
    if (set) {
      m_port_c |= mask;
    } else {
      m_port_c &= ~mask;
    }
  }
}

uint8_t iostub::inbit(enum port_num port, uint8_t bit) {
  if (port == PORTA) {
    return m_port_a & (0x01 << bit);
  } else if (port == PORTB) {
    return m_port_b & (0x01 << bit);
  } else {
    return m_port_c & (0x01 << bit);
  }
}
