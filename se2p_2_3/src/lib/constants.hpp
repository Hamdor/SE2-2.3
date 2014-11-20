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

#include <cstddef>

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
  MOTOR_RIGHT = 0,
  MOTOR_LEFT  = 1,
  MOTOR_SLOW  = 2,
  MOTOR_STOP  = 3,
  MOTOR_FAST  = 4
};

/**
 * LEDs der einzelnen Knöpfe
 **/
enum button_leds {
  START_LED = 0,
  RESET_LED = 1,
  Q1_LED    = 2,
  Q2_LED    = 3
};

/**
 * Verfügbare Knöpfe
 **/
enum buttons {
  BUTTON_START = 4, // 1 => gedrückt
  BUTTON_STOP  = 5, // 1 => NICHT gedrückt
  BUTTON_RESET = 6, // 1 => gedrückt
  BUTTON_ESTOP = 7  // 1 => NICHT gedrückt
};

/**
 * Lichtschranken des Systems
 **/
enum light_barriers {
  ENTRACE_SENSOR = 0, // einlauf
  HEIGHT_SENSOR  = 1, // höhensensor
  SWITCH_SENSOR  = 3, // weiche
  SLIGHT_SENSOR  = 6, // rutsche
  EXIT_SENSOR    = 7  // auslauf
};

/**
 * Lampen der Ampel
 **/
enum light_colors {
  GREEN  = 5,
  YELLOW = 6,
  RED    = 7
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

/**
 * Defines fuer `event_values` Bereiche
 **/
#define EVENT_BASE 0x01
#define EVENT_PORT_A_OFFSET 0x00
#define EVENT_PORT_C_OFFSET 0x08
#define EVENT_SERIAL_START (EVENT_BASE << EXIT_SENSOR                         \
                                       << EVENT_PORT_C_OFFSET) | 0x01
/**
 * Pulse Message Values
 * Events Port C:
 *   EVENT_BUTTON_START   Start Button getoggelt
 *   EVENT_BUTTON_STOP    Stop Button getoggelt
 *   EVENT_BUTTON_RESET   Reset Button getoggelt
 *   EVENT_BUTTON_E_STOP  E Stop getoggelt
 *
 * Events Port B:
 *   EVENT_ENTRACE_SENSOR Lichtschranke am Band einlauf getoggelt
 *   EVENT_HEIGHT_SENSOR  Lichtschranke am Hoehensensor getoggelt
 *   EVENT_SWITCH_SENSOR  Lichtschranke am Switch getoggelt
 *   EVENT_SLIGHT_SENSOR  Lichtschranke der Rutsche getoggelt
 *   EVENT_EXIT_SENSOR    Lichtschranke am Ausgang des Bandes getoggelt
 *
 * Serielle Schnittstelle:
 *   EVENT_SERIAL_DATA    Daten empfangen     (Token Daten)
 *   EVENT_SERIAL_MSG     Nachricht empfangen (Keine Daten)
 *   EVENT_SERIAL_ERR     Errornachricht empfagen
 *   EVENT_SERIAL_UNK     Unbekannte Nachricht empfangen
 *                        (Fehlerhafte Nachricht?)
 *
 * Timer Events:
 *   EVENT_SEG1_EXCEEDED   Token zwischen Einlauf und Hoehenmessung (Segment 1)
 *   EVENT_SEG2_EXCEEDED   Token zwischen Hoehenmessung und Weiche (Segment 2)
 *   EVENT_SEG3_EXCEEDED   Token zwischen Weiche und Auslauf (Segment 3)
 *   EVENT_SLIDE_FULL      Rutsche ist voll wenn abgelaufen
 *   EVENT_OPEN_SWITCH     Oeffnungsdauer der Weiche
 *   EVENT_TURN_TOKEN      Zeit zum Wenden eines Puks/Tokens
 *   EVENT_REMOVE_TOKEN    Zeit zum Entfernen eines Puks/Tokens
 *   EVENT_TOKEN_FINISHED  Zeit bis der Token das Ende von Band 2 erreicht hat
 **/
enum event_values {
  // Port C
  EVENT_BUTTON_START   = EVENT_BASE << BUTTON_START   << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_STOP    = EVENT_BASE << BUTTON_STOP    << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_RESET   = EVENT_BASE << BUTTON_RESET   << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_E_STOP  = EVENT_BASE << BUTTON_ESTOP   << EVENT_PORT_A_OFFSET,
  // Port B
  EVENT_ENTRACE_SENSOR = EVENT_BASE << ENTRACE_SENSOR << EVENT_PORT_C_OFFSET,
  EVENT_HEIGHT_SENSOR  = EVENT_BASE << HEIGHT_SENSOR  << EVENT_PORT_C_OFFSET,
  EVENT_SWITCH_SENSOR  = EVENT_BASE << SWITCH_SENSOR  << EVENT_PORT_C_OFFSET,
  EVENT_SLIGHT_SENSOR  = EVENT_BASE << SLIGHT_SENSOR  << EVENT_PORT_C_OFFSET,
  EVENT_EXIT_SENSOR    = EVENT_BASE << EXIT_SENSOR    << EVENT_PORT_C_OFFSET,
  // Serial Interface
  EVENT_SERIAL_DATA = EVENT_SERIAL_START, // EVENT_SERIAL_START + 0x00
  EVENT_SERIAL_MSG,                       // EVENT_SERIAL_START + 0x01
  EVENT_SERIAL_ERR,                       // EVENT_SERIAL_START + 0x02
  EVENT_SERIAL_UNK,                       // EVENT_SERIAL_START + 0x03
  // Timer
  EVENT_SEG1_EXCEEDED,                    // EVENT_SERIAL_UNK + 0x01
  EVENT_SEG2_EXCEEDED,                    // EVENT_SERIAL_UNK + 0x02
  EVENT_SEG3_EXCEEDED,                    // EVENT_SERIAL_UNK + 0x03
  EVENT_SLIDE_FULL,                       // EVENT_SERIAL_UNK + 0x04
  EVENT_OPEN_SWITCH,                      // EVENT_SERIAL_UNK + 0x05
  EVENT_TURN_TOKEN,                       // EVENT_SERIAL_UNK + 0x06
  EVENT_REMOVE_TOKEN,                     // EVENT_SERIAL_UNK + 0x07
  EVENT_TOKEN_FINISHED                    // EVENT_SERIAL_UNK + 0x08
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
  int m_type;
  int m_msg;
  int m_id;
  int m_height1;
  int m_height2;
};

} // namespace serial_bus

namespace timer {
struct duration {
  size_t sec;
  size_t msec;
};


} // namespace timer
} // namespace se2

#endif // SE2_CONSTANTS_HPP
