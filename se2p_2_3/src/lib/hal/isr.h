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

#include "config.h"

#include "lib/constants.hpp"

#ifndef SE2_ISR_H
#define SE2_ISR_H

using namespace se2;
using namespace se2::hal;

int isr_coid = 0;
int port_old = 0;

const struct sigevent* isr(void* arg, int id) {
  struct sigevent* event = static_cast<struct sigevent*>(arg);
  uint8_t irq_val = in8(static_cast<uint16_t>(IRQ_CLEAR_REG));
  out8(static_cast<uint16_t>(IRQ_CLEAR_REG), 0); // Interrupt zuruecksetzen
  if (irq_val == PORTB_INTERRUPT || irq_val == PORTC_INTERRUPT) {
    uint16_t ports = (in8(static_cast<uint16_t>(PORTB)) << 8) |
                      in8(static_cast<uint16_t>(PORTC));
    event->sigev_notify = SIGEV_PULSE;
    event->__sigev_un1.__sigev_coid = isr_coid;
    event->__sigev_un2.__st.__sigev_code = INTERRUPT;
    int changed_bit = port_old ^ ports;
    event->sigev_value.sival_int = changed_bit;
    if (changed_bit == EVENT_ZERO) {
      event = NULL;
    }
    if (port_old < ports) {
      // Steigende flanke
      if (changed_bit == EVENT_SENSOR_ENTRANCE) {
        event->sigev_value.sival_int = EVENT_SENSOR_ENTRANCE_R;
      } else if (changed_bit == EVENT_SENSOR_HEIGHT) {
        event->sigev_value.sival_int = EVENT_SENSOR_HEIGHT_R;
      } else if (changed_bit == EVENT_SENSOR_SWITCH) {
        event->sigev_value.sival_int = EVENT_SENSOR_SWITCH_R;
      } else if (changed_bit == EVENT_SENSOR_SLIDE) {
        event->sigev_value.sival_int = EVENT_SENSOR_SLIDE_R;
      } else if (changed_bit == EVENT_SENSOR_EXIT) {
        event->sigev_value.sival_int = EVENT_SENSOR_EXIT_R;
      }
    }
    // Update port_old
    port_old = ports;
  } else {
    // Ein IRQ von Port A oder etwas anderem auf das wir nicht reagieren
    event = NULL;
  }
  return event;
}

#endif // SE2_ISR_H
