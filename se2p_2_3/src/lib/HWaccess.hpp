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

#include <sys/neutrino.h>
#include <hw/inout.h>
#include <ioaccess.h>

namespace se2 {

/**
 * Motor modes
 **/
enum motor_modes {
  motor_stop,
  motot_right,
  motor_left
};

/**
 * LEDs der einzelnen Knöpfe
 **/
enum button_leds {
  start_button,
  reset_button,
  q1_button,
  q2_button
};

/**
 * Verfügbare Knöpfe
 **/
enum buttons {
  button_start,
  button_stop,
  button_estop
};

/**
 * Lichtschranken des Systems
 **/
enum light_barriers {
  entrace_sensor,
  height_sensor,
  switch_sensor,
  light_sensor,
  exit_sensor
};

/**
 * Lampen der Ampel
 **/
enum light_colors {
  red,
  yellow,
  green
};

struct hwaccess {
  /**
   * Setzt den Motor Modus (Aus, Rechtslauf, Linkslauf)
   * @param mode  Gibt den Modus an
   * @param slow  Wenn true läuft der Motor langsam
   **/
  void set_motor(enum motor_modes mode, bool slow);

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
  void set_light(enum button_leds light, bool on);

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
 private:
  static hwaccess*   instance;
  static util::mutex s_lock;
  /**
   * @return Gibt die Instanz der HAL zurück
   **/
  static hwaccess* get_instance();

  /**
   * Private Konstruktoren
   **/
  hwaccess();
  hwaccess(const hwaccess&);
};

} // namespace se2

#endif // SE2_HWACCESS_HPP
