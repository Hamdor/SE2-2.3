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
 * @file    isr.h
 * @version 0.1
 *
 * Implementierung der ISR
 **/

#include "lib/hal/HWaccess.hpp"
#include "lib/constants.hpp"

#ifndef SE2_ISR_H
#define SE2_ISR_H

using namespace se2::hal;

int isr_coid;

const struct sigevent* isr(void* arg, int id) {
  struct sigevent* event = static_cast<struct sigevent*>(arg);
  uint8_t irq_val = in8(static_cast<uint16_t>(IRQ_CLEAR_REG));
  out8(static_cast<uint16_t>(IRQ_CLEAR_REG), 0); // Interrupt zurücksetzen
  if (irq_val == PORTB_INTERRUPT || irq_val == PORTC_INTERRUPT) {
    uint8_t portb = in8(static_cast<uint16_t>(PORTB));
    uint8_t portc = in8(static_cast<uint16_t>(PORTC));
    event->sigev_notify = SIGEV_PULSE;
    event->__sigev_un1.__sigev_coid = isr_coid;
    event->__sigev_un2.__st.__sigev_code = 0;
    event->sigev_value.sival_int = portb << 8 | portc;
  } else {
    event = NULL;
  }
  return event;
}

#endif // SE2_ISR_H
