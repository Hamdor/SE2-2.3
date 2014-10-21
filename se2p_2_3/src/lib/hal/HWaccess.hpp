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
 * @file    HWaccess.hpp
 * @version 0.1
 *
 * Header der HAL - Allgemeiner Zugriff auf Hardware, sowie Ansteuerung
 * der Sensorik und Aktorik.
 **/

#ifndef SE2_HWACCESS_HPP
#define SE2_HWACCESS_HPP

#include "lib/util/mutex.hpp"
#include "lib/hal/abstract_inout.hpp"

#include <sys/neutrino.h>
#include <ioaccess.h>

namespace se2 {
namespace hal {

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
 * Motor modes
 **/
enum motor_modes {
  motot_right = 0,
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

struct hwaccess {
  /**
   * Default Destruktor
   **/
  ~hwaccess();

  /**
   * Setzt den Motor Modus (Aus, Rechtslauf, Linkslauf)
   * @param mode  Gibt den Modus an
   **/
  void set_motor(enum motor_modes mode);

  /**
   * Öffnet die Weiche, wenn Weiche offen keine weitere Reaktion
   **/
  void open_switch();

  /**
   * Schließt die Weiche, wenn Weiche geschlossen keine weitere Reaktion
   **/
  void close_switch();

  /**
   * Setzt eines der Ampellichter auf an oder aus
   * @param light Bestimmt die Lampe
   * @param on    Bestimmt ob an oder aus
   **/
  void set_light(enum light_colors light, bool on);

  /**
   * Prüft ob die angegebene Lichtschranke unterbrochen ist
   * @param  barrier Gibt die Lichtschranke an
   * @return TRUE  wenn Lichtschranke durchbrochen,
   *         FALSE wenn Lichtschranke geschlossen
   **/
  bool obj_in_light_barrier(enum light_barriers barrier) const;

  /**
   * Prüft ob das Objekt im Tolleranzbereich ist
   * @return TRUE  wenn Objekt im Tolleranzbereich
   *         FALSE wenn Objekt nicht im Tolleranzbereich
   **/
  bool obj_has_valid_height() const;

  /**
   * Ruft den absoluten Höhenwert des Sensors ab
   * @return Ein 16 bit unsigned Integer als Absolutwert
   **/
  uint16_t get_height_value();

  /**
   * Prüft ob sich Metall im Metallsensor befindet
   * @return TRUE  wenn Metall im Sensor
   *         FALSE wenn kein Metall im Sensor
   **/
  bool obj_has_metal() const;

  /**
   * Prüft ob die Weiche offen ist
   * @return TRUE  wenn Weiche geöffnet
   *         FALSE wenn Weiche geschlossen
   **/
  bool is_switch_open() const;

  /**
   * Setzt eine LED auf an oder aus
   * @param led Bestimmt die LED
   * @param on  Bestimmt ob an der aus
   **/
  void set_led_state(enum button_leds led, bool on);

  /**
   * Prüft ob ein Button gedrückt ist
   * @param  key Bestimmt den zu prüfenden Button
   * @return TRUE  wenn Button gedrückt
   *         FALSE wenn Button nicht gedrückt
   **/
  bool is_button_pressed(enum buttons key) const;

  /**
   * Singleton
   **/
  /**
   * @return Gibt die Instanz der HAL zurück
   **/
  static hwaccess* get_instance();
 private:
  static hwaccess*   instance;
  static util::mutex s_lock;

  /**
   * Private Konstruktoren
   **/
  hwaccess();
  hwaccess(const hwaccess&);

  /**
   * Member deklaration
   **/
  abstract_inout* m_io;
};

} // namespace hal
} // namespace se2

#endif // SE2_HWACCESS_HPP
