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
 * @file    constants.hpp
 * @version 0.1
 *
 * Definiert Konstanten / Addressen / Pin Belegungen
 **/

#ifndef SE2_CONSTANTS_HPP
#define SE2_CONSTANTS_HPP

/**
 * Bit Position der Weiche auf Port A
 **/
#define SWITCH_BIT 4

/**
 * Bit Position für geöffnete Weiche Sensor auf Port B
 **/
#define SWITCH_OPEN_BIT 5

/**
 * Bit Position für Höhensensor auf Port B
 * (Tolleranzbereich, nicht Absolutwert)
 **/
#define HEIGHT_BIT 2

/**
 * Bit Position für Metallsensor auf Port B
 **/
#define METAL_BIT 4

/**
 * Basis Addresse des IO Registers
 **/
#define IOBASE    0x300         // IO Base Register

/**
 * Addresse des Controllregisters
 **/
#define IOCTLADDR IOBASE + 0x03 // Register zum definieren der input/output Ports

/**
 * Maske für das Controllregister
 **/
#define IOCTLPORT 0x8A

/**
 * Addressen/Defines Analogport
 **/
#define IOANALOG_BASE        0x320
#define IOANALOG_OFFSET      0x02
#define START_HEIGHT_MEASURE 0x10

/**
 * Addressen/Defines Interrupt
 **/
#define IRQ_ENABLE_MASK         0xF9
#define PORTB_INTERRUPT         2
#define PORTC_INTERRUPT         8
#define IO_IRQ                  11

namespace se2 {
namespace hal {

/**
 * Offset der Ports
 **/
enum port_num {
  PORTA          = IOBASE + 0x00, // IOBASE + 0x00
  PORTB          = IOBASE + 0x01, // IOBASE + 0x01
  PORTC          = IOBASE + 0x02, // IOBASE + 0x02
  IRQ_ENABLE_REG = IOBASE + 0x0B, // IOBASE + 0x0B
  IRQ_CLEAR_REG  = IOBASE + 0x0F, // IOBASE + 0x0F
  ANALOG_BASE    = IOANALOG_BASE, // IOANALOG_BASE (0x320)
  ANALOG_PORT_A  = IOANALOG_BASE + IOANALOG_OFFSET // IOANALOG_BASE + 0x02
};

/**
 * Motor modes
 **/
enum motor_modes {
  motor_right = 0,
  motor_left  = 1,
  motor_slow  = 2,
  motor_stop  = 3,
  motor_fast  = 4
};

/**
 * LEDs der einzelnen Knöpfe
 **/
enum button_leds {
  start_button = 0,
  reset_button = 1,
  q1_button    = 2,
  q2_button    = 3
};

/**
 * Verfügbare Knöpfe
 **/
enum buttons {
  button_start = 4, // 1 => gedrückt
  button_stop  = 5, // 1 => NICHT gedrückt
  button_reset = 6, // 1 => gedrückt
  button_estop = 7  // 1 => NICHT gedrückt
};

/**
 * Lichtschranken des Systems
 **/
enum light_barriers {
  entrace_sensor = 0, // einlauf
  height_sensor  = 1, // höhensensor
  switch_sensor  = 3, // weiche
  slight_sensor  = 6, // rutsche
  exit_sensor    = 7  // auslauf
};

/**
 * Lampen der Ampel
 **/
enum light_colors {
  green  = 5,
  yellow = 6,
  red    = 7
};

} // namespace hal

namespace util {

enum loglevel {
  TRACE   = 0,  // Unwichtig
  DEBUG   = 1,  // Debug
  WARNING = 2,  // Warnung
  ERROR   = 3   // Fehler
};

} // namespace util
} // namespace se2

#endif // SE2_CONSTANTS_HPP
