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

#include "lib/token.hpp"

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

/**
 * Pulse Message Herkunft
 **/
enum event_source {
  INTERRUPT = 0,
  SERIAL    = 1,
  TIMER     = 2
};

/**
 * Pulse Message Values
 * TODO: erweitern
 **/
enum event_values {
  NEW_SERIAL_DATA = 0, // Daten vom anderen Band
  NEW_SERIAL_MSG  = 1, // Nachricht des anderen Bandes
  NEW_SERIAL_ERR  = 2, // Error Nachricht des anderen Bandes
  NEW_SERIAL_UNK  = 3  // Unbekannte Nachricht
};

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
  IRQ_RNC_REG    = IOBASE + 0x12, // IOBASE + 0x12 (Read and Clear)
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

/**
 * Rueckgabewerte des Hoehensensors
 * fuer die einzelnen Puck typen.
 **/
enum height_values {
#if defined(FESTO_3)
  TOO_SMALL_LOW       = 2835, // zu flach (Min)
  TOO_SMALL_HI        = 2845, // zu flach (Max)
  HOLE_BUTTOM_UP_LOW  = 2562, // Bohrung (Falsch herum) (Min)
  HOLE_BUTTOM_UP_HI   = 2573, // Bohrung (Falsch herum) (Max)
  HOLE_LOW            = 3608, // Bohrung (Richtig herum) (Min)
  HOLE_HI             = 3617, // Bohrung (Richtig herum) (Max)
  METAL_BUTTOM_UP_LOW = 2547, // Mit Metall (Falsch herum) (Min)
  METAL_BUTTOM_UP_HI  = 2557, // Mit Metall (Falsch herum) (Max)
  METAL_LOW           = 3620, // Mit Metall (Richtig herum) (Min)
  METAL_HI            = 3629  // Mit Metall (Richtig herum) (Max)
#elif defined(FESTO_9)
  TOO_SMALL_LOW       = 2720, // zu flach (Min)
  TOO_SMALL_HI        = 2735, // zu flach (Max)
  HOLE_BUTTOM_UP_LOW  = 2455, // Bohrung (Falsch herum) (Min)
  HOLE_BUTTOM_UP_HI   = 2470, // Bohrung (Falsch herum) (Max)
  HOLE_LOW            = 3475, // Bohrung (Richtig herum) (Min)
  HOLE_HI             = 3490, // Bohrung (Richtig herum) (Max)
  // (WURDEN NICHT GEMESSEN)
  //METAL_BUTTOM_UP_LOW = , // Mit Metall (Falsch herum) (Min)
  //METAL_BUTTOM_UP_HI  = , // Mit Metall (Falsch herum) (Max)
  METAL_LOW           = 3515, // Mit Metall (Richtig herum) (Min)
  METAL_HI            = 3535  // Mit Metall (Richtig herum) (Max)
#else
  // TODO: Default Werte? Mittelwert?
#endif
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

namespace serial_bus {

/**
 * Telegram Typ
 **/
enum telegram_type {
  MSG  = 0,  // Nachricht (keine Daten)
  DATA = 1,  // Daten werden übertragen
  ERR  = 2   // letzte packet neu senden
};

/**
 * Nachricht typen
 **/
enum msg_type {
  ERR_STOP = 0,  // Fehler auf einem Band, stoppen
  ERR_QUIT = 1,  // Fehler quittiert
  RESUME   = 2,  // Weiterlaufen/Start
  B2_FREE  = 3,  // Band 2 wieder frei von Puk
  E_STOP   = 4,  // Estop gedrückt
  STOP     = 5,  // Stop taste gedrückt
  NOTHING  = 6   // Keine MSG
};

/**
 * Telegram komplett
 **/
struct telegram {
  telegram_type m_type; // (SYNC, MSG, ACK, DATA, ERROR)
  msg_type      m_msg;  // Nachricht (optional bei Daten)
  token         m_data; // Daten (optional bei Message)
};

} // namespace serial_bus
} // namespace se2

#endif // SE2_CONSTANTS_HPP
