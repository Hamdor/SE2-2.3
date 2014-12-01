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
#include "lib/token.hpp"

/**
 * Bit Position der Weiche auf Port A
 **/
#define SWITCH_BIT 4

/**
 * Bit Position fuer geoeffnete Weiche Sensor auf Port B
 **/
#define SWITCH_OPEN_BIT 5

/**
 * Bit Position fuer Hoehensensor auf Port B
 * (Tolleranzbereich, nicht Absolutwert)
 **/
#define HEIGHT_BIT 2

/**
 * Bit Position fuer Metallsensor auf Port B
 **/
#define METAL_BIT 4

/**
 * Basis Addresse des IO Registers
 **/
#define IOBASE    0x300         // IO Base Register

/**
 * Bitposition des Bits was von dem Hoehensensor gesetzt wird,
 * sobald der Wert im Register steht
 **/
#define HEIGHT_SENSOR_OK_MASK 0x01 << 7

/**
 * Maximale Anzahl an loops bis der Hoehenwert gelesen wird
 **/
#define HEIGHT_SENSOR_MAX_LOOPS 50

/**
 * Maximale und minimale Toleranz fuer den Puck/Hoehenwert
 **/
#define HEIGHT_SENSOR_TOLERANCE_MIN 50
#define HEIGHT_SENSOR_TOLERANCE_MAX 50

/**
 * Addresse des Control-Registers
 **/
#define IOCTLADDR IOBASE + 0x03 // Register zum definieren der Ports

/**
 * Maske fuer das Control-Register
 **/
#define IOCTLPORT 0x8A

/**
 * Addressen/Defines Analog-Port
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
 * Verfuegbare Modi des Motor
 **/
enum motor_modes {
  MOTOR_RIGHT  = 0,
  MOTOR_LEFT   = 1,
  MOTOR_SLOW   = 2,
  MOTOR_STOP   = 3,
  MOTOR_FAST   = 4,
  MOTOR_RESUME = 5
};

/**
 * Verfuegbare LEDs
 **/
enum leds {
  LED_START = 0,
  LED_RESET = 1,
  LED_Q1    = 2,
  LED_Q2    = 3
};

/**
 * Verfuegbare Knoepfe
 **/
enum buttons {
  BUTTON_START = 4, // 1 => gedrueckt
  BUTTON_STOP  = 5, // 1 => NICHT gedrueckt
  BUTTON_RESET = 6, // 1 => gedrueckt
  BUTTON_ESTOP = 7  // 1 => NICHT gedrueckt
};

/**
 * Lichtschranken des Systems
 **/
enum light_barriers {
  SENSOR_ENTRANCE = 0, // Einlauf
  SENSOR_HEIGHT   = 1, // Hoehensensor
  SENSOR_SWITCH   = 3, // Weiche
  SENSOR_SLIDE    = 6, // Rutsche
  SENSOR_EXIT     = 7  // Auslauf
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
 * fuer die einzelnen Puck Typen.
 **/
enum height_values {
#if defined(FESTO_3)
  TOO_SMALL_LOW       = 2835, // zu flach (Min)
  TOO_SMALL_HI        = 2845, // zu flach (Max)
  HOLE_BOTTOM_UP_LOW  = 2562, // Bohrung (Falsch herum) (Min)
  HOLE_BOTTOM_UP_HI   = 2573, // Bohrung (Falsch herum) (Max)
  HOLE_LOW            = 3608, // Bohrung (Richtig herum) (Min)
  HOLE_HI             = 3617, // Bohrung (Richtig herum) (Max)
  METAL_BOTTOM_UP_LOW = 2547, // Mit Metall (Falsch herum) (Min)
  METAL_BOTTOM_UP_HI  = 2557, // Mit Metall (Falsch herum) (Max)
  METAL_LOW           = 3620, // Mit Metall (Richtig herum) (Min)
  METAL_HI            = 3629  // Mit Metall (Richtig herum) (Max)
#elif defined(FESTO_9)
  TOO_SMALL_LOW       = 2720, // zu flach (Min)
  TOO_SMALL_HI        = 2735, // zu flach (Max)
  HOLE_BOTTOM_UP_LOW  = 2455, // Bohrung (Falsch herum) (Min)
  HOLE_BOTTOM_UP_HI   = 2470, // Bohrung (Falsch herum) (Max)
  HOLE_LOW            = 3475, // Bohrung (Richtig herum) (Min)
  HOLE_HI             = 3490, // Bohrung (Richtig herum) (Max)
  METAL_LOW           = 3515, // Mit Metall (Richtig herum) (Min)
  METAL_HI            = 3535  // Mit Metall (Richtig herum) (Max)
#elif defined(FESTO_5)
  TOO_SMALL_LOW = 2774,
  TOO_SMALL_HI = 2833,
  HOLE_BOTTOM_UP_LOW = 2533,
  HOLE_BOTTOM_UP_HI = 2571,
  HOLE_LOW = 3572,
  HOLE_HI = 3612,
  METAL_BOTTOM_UP_LOW = 2514,
  METAL_BOTTOM_UP_HI = 2573,
  METAL_LOW = 3587,
  METAL_HI = 3642
#elif defined(FESTO_7)
  TOO_SMALL_LOW = 2804,
  TOO_SMALL_HI = 2831,
  HOLE_BOTTOM_UP_LOW = 2532,
  HOLE_BOTTOM_UP_HI = 2583,
  HOLE_LOW = 3553,
  HOLE_HI = 3617,
  METAL_BOTTOM_UP_LOW = 2556,
  METAL_BOTTOM_UP_HI = 2617,
  METAL_LOW = 3595,
  METAL_HI = 3665
#elif defined(FESTO_1)
  TOO_SMALL_LOW = 2799,
  TOO_SMALL_HI = 2826,
  HOLE_BOTTOM_UP_LOW = 2521,
  HOLE_BOTTOM_UP_HI = 2574,
  HOLE_LOW = 3577,
  HOLE_HI = 3625,
  METAL_BOTTOM_UP_LOW = 2560,
  METAL_BOTTOM_UP_HI = 2585,
  METAL_LOW = 3623,
  METAL_HI = 3663
#else
  // nop
#endif
};

/**
 * Defines fuer `event_values` Bereiche
 **/
#define EVENT_BASE 0x01
#define EVENT_PORT_A_OFFSET 0x00
#define EVENT_PORT_C_OFFSET 0x08
#define EVENT_SERIAL_START (EVENT_BASE << SENSOR_EXIT | 1                     \
                                       << EVENT_PORT_C_OFFSET) | 0x01
/**
 * Pulse Message Values
 *   EVENT_ZERO            Nullevent
 * Events Port C:
 *   EVENT_BUTTON_START    Start Button getoggelt
 *   EVENT_BUTTON_STOP     Stop Button getoggelt
 *   EVENT_BUTTON_RESET    Reset Button getoggelt
 *   EVENT_BUTTON_E_STOP   E-Stop getoggelt
 *   EVENT_BUTTON_E_STOP_R E-Stop getoggelt (Steigende Flanke)
 *
 * Events Port B:
 *   EVENT_SENSOR_ENTRANCE Lichtschranke am Band einlauf getoggelt
 *   EVENT_SENSOR_HEIGHT   Lichtschranke am Hoehensensor (Fallende Flanke)
 *   EVENT_SENSOR_HEIGHT_R Lichtschranke am Hoehensensor (Steigende Flanke)
 *   EVENT_SENSOR_SWITCH   Lichtschranke am Switch (Fallende Flanke)
 *   EVENT_SENSOR_SWITCH_R Lichtschranke am Switch (Steigende Flanke)
 *   EVENT_SENSOR_SLIDE    Lichtschranke der Rutsche (Fallende Flanke)
 *   EVENT_SENSOR_SLIDE_R  Lichtschranke der Rutsche (Steigende Flanke)
 *   EVENT_SENSOR_EXIT     Lichtschranke am Ausgang (Fallende Flanke)
 *   EVENT_SENSOR_EXIT_R   Lichtschranke am Ausgang (Steigende Flanke)
 *
 * Serielle Schnittstelle:
 *   EVENT_SERIAL_DATA     Daten empfangen     (Token Daten)
 *   EVENT_SERIAL_MSG      Nachricht empfangen (Keine Daten)
 *   EVENT_SERIAL_ERR      Errornachricht empfagen
 *   EVENT_SERIAL_UNK      Unbekannte Nachricht empfangen
 *                         (Fehlerhafte Nachricht?)
 *
 * Timer Events:
 *   EVENT_SEG1_EXCEEDED   Token zwischen Einlauf und Hoehenmessung (Segment 1)
 *   EVENT_SEG2_EXCEEDED   Token zwischen Hoehenmessung und Weiche  (Segment 2)
 *   EVENT_SEG3_EXCEEDED   Token zwischen Weiche und Auslauf (Segment 3)
 *   EVENT_SLIDE_FULL      Rutsche ist voll wenn abgelaufen
 *   EVENT_OPEN_SWITCH     Oeffnungsdauer der Weiche
 *   EVENT_TURN_TOKEN      Zeit zum Wenden eines Pucks
 *   EVENT_REMOVE_TOKEN    Zeit zum Entfernen eines Pucks
 *   EVENT_TOKEN_FINISHED  Zeit bis der Puck das Ende von Band 2 erreicht hat
 **/
enum event_values {
  EVENT_ZERO            = 0,
  // Port C
  EVENT_BUTTON_START    = EVENT_BASE << BUTTON_START    << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_STOP     = EVENT_BASE << BUTTON_STOP     << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_RESET    = EVENT_BASE << BUTTON_RESET    << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_E_STOP   = EVENT_BASE << BUTTON_ESTOP    << EVENT_PORT_A_OFFSET,
  EVENT_BUTTON_E_STOP_R = (EVENT_BASE<<BUTTON_ESTOP<< EVENT_PORT_A_OFFSET) | 1,
  // Port B
  EVENT_SENSOR_ENTRANCE = EVENT_BASE << SENSOR_ENTRANCE << EVENT_PORT_C_OFFSET,
  EVENT_SENSOR_HEIGHT   = EVENT_BASE << SENSOR_HEIGHT   << EVENT_PORT_C_OFFSET,
  EVENT_SENSOR_HEIGHT_R = (EVENT_BASE<<SENSOR_HEIGHT<<EVENT_PORT_C_OFFSET) | 1,
  EVENT_SENSOR_SWITCH   = EVENT_BASE << SENSOR_SWITCH   << EVENT_PORT_C_OFFSET,
  EVENT_SENSOR_SWITCH_R = (EVENT_BASE<<SENSOR_SWITCH<<EVENT_PORT_C_OFFSET) | 1,
  EVENT_SENSOR_SLIDE    = EVENT_BASE << SENSOR_SLIDE    << EVENT_PORT_C_OFFSET,
  EVENT_SENSOR_SLIDE_R  = (EVENT_BASE<< SENSOR_SLIDE<<EVENT_PORT_C_OFFSET) | 1,
  EVENT_SENSOR_EXIT     = EVENT_BASE << SENSOR_EXIT     << EVENT_PORT_C_OFFSET,
  EVENT_SENSOR_EXIT_R   = (EVENT_BASE<<SENSOR_EXIT << EVENT_PORT_C_OFFSET) | 1,
  // Serial Interface
  EVENT_SERIAL_DATA     = EVENT_SERIAL_START, // EVENT_SERIAL_START + 0x00
  EVENT_SERIAL_MSG,                           // EVENT_SERIAL_START + 0x01
  EVENT_SERIAL_ERR,                           // EVENT_SERIAL_START + 0x02
  EVENT_SERIAL_NEXT_OK,                       // EVENT_SERIAL_START + 0x03
  EVENT_SERIAL_UNK,                           // EVENT_SERIAL_START + 0x04
  // Timer
  EVENT_SEG1_EXCEEDED,                        // EVENT_SERIAL_UNK + 0x01
  EVENT_SEG2_EXCEEDED,                        // EVENT_SERIAL_UNK + 0x02
  EVENT_SEG3_EXCEEDED,                        // EVENT_SERIAL_UNK + 0x03
  EVENT_SLIDE_FULL,                           // EVENT_SERIAL_UNK + 0x04
  EVENT_OPEN_SWITCH,                          // EVENT_SERIAL_UNK + 0x05
  EVENT_TURN_TOKEN,                           // EVENT_SERIAL_UNK + 0x06
  EVENT_REMOVE_TOKEN,                         // EVENT_SERIAL_UNK + 0x07
  EVENT_TOKEN_FINISHED,                       // EVENT_SERIAL_UNK + 0x08
  // Unkown / Not handled inputs
  EVENT_UNKOWN1 = 8192                        // Ich weiss nicht woher dieses Event kommt...
};

} // namespace hal

namespace dispatch {

/**
 * Interne Events fuer Dispatcher Matrix
 * Bildet von `event_values` auf Dispatcher interne `dispatcher_events` ab
 **/
enum dispatcher_events {
  DISPATCHED_EVENT_BUTTON_START = 0,
  DISPATCHED_EVENT_BUTTON_STOP,
  DISPATCHED_EVENT_BUTTON_RESET,
  DISPATCHED_EVENT_BUTTON_E_STOP,   // Fallende Flanke
  DISPATCHED_EVENT_BUTTON_E_STOP_R, // Steigende Flanke
  DISPATCHED_EVENT_SENSOR_ENTRANCE,
  DISPATCHED_EVENT_SENSOR_HEIGHT,   // Fallende Flanke
  DISPATCHED_EVENT_SENSOR_HEIGHT_R, // Steigende Flanke
  DISPATCHED_EVENT_SENSOR_SWITCH,   // Fallende Flanke
  DISPATCHED_EVENT_SENSOR_SWITCH_R, // Steigende Flanke
  DISPATCHED_EVENT_SENSOR_SLIDE,    // Fallende Flanke
  DISPATCHED_EVENT_SENSOR_SLIDE_R,  // Steigende Flanke
  DISPATCHED_EVENT_SENSOR_EXIT,     // Fallende Flanke
  DISPATCHED_EVENT_SENSOR_EXIT_R,   // Steigende Flanke
  DISPATCHED_EVENT_SERIAL_DATA,
  DISPATCHED_EVENT_SERIAL_MSG,
  DISPATCHED_EVENT_SERIAL_ERR,
  DISPATCHED_EVENT_SERIAL_NEXT_OK,
  DISPATCHED_EVENT_SERIAL_UNK,
  DISPATCHED_EVENT_SEG1_EXCEEDED,
  DISPATCHED_EVENT_SEG2_EXCEEDED,
  DISPATCHED_EVENT_SEG3_EXCEEDED,
  DISPATCHED_EVENT_SLIDE_FULL,
  DISPATCHED_EVENT_OPEN_SWITCH,
  DISPATCHED_EVENT_TURN_TOKEN,
  DISPATCHED_EVENT_REMOVE_TOKEN,
  DISPATCHED_EVENT_TOKEN_FINISHED,
  DISPATCHED_EVENT_MAX
};

}

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
  DATA = 1,  // Daten werden uebertragen
  ERR  = 2   // Letztes Paket neu senden
};

/**
 * Nachricht Typen
 **/
enum msg_type {
  ERR_STOP = 0,  // Fehler auf einem Band, stoppen
  ERR_QUIT = 1,  // Fehler quittiert
  RESUME   = 2,  // Weiterlaufen/Start
  B2_FREE  = 3,  // Band 2 wieder frei von Puck
  E_STOP   = 4,  // E-Stop gedrueckt
  STOP     = 5,  // Stop Taste gedrueckt
  NOTHING  = 6   // Keine MSG
};

/**
 * Telegram komplett
 **/
struct telegram {
  telegram() : m_type(MSG), m_msg(NOTHING), m_id(0)
             , m_height1(0)
             , m_height2(0) {
    // nop
  }
  telegram(msg_type msg) : m_type(MSG), m_msg(msg), m_id(0)
                         , m_height1(0), m_height2(0) {
    // nop
  }
  telegram(const token* t) : m_type(DATA), m_msg(NOTHING), m_id(t->get_id())
                           , m_height1(t->get_height1())
                           , m_height2(t->get_height2()) {
    // nop
  }
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
