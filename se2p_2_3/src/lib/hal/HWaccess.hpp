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
 * @version 0.2
 *
 * Header der HAL - Allgemeiner Zugriff auf Hardware, sowie Ansteuerung
 * der Sensorik und Aktorik.
 **/

#ifndef SE2_HWACCESS_HPP
#define SE2_HWACCESS_HPP

#include "config.h"

#include "lib/constants.hpp"
#include "lib/hal/iostub.hpp"
#include "lib/hal/abstract_inout.hpp"

#include "lib/util/mutex.hpp"
#include "lib/util/abstract_singleton.hpp"

#include <sys/siginfo.h>

namespace se2 {
namespace util {
class singleton_mgr;
} // namespace util
namespace hal {

struct hwaccess : public util::abstract_singleton {
  friend class util::singleton_mgr;
  /**
   * Control Klasse fuerr den ISR Teil der HAL
   * Diese Klasse ist nur von `hwaccess` zugaenglich
   * Kapselt alle benoetigten Daten fuer die ISR
   **/
  struct isr_control {
    int m_chid;              // Channel ID fuer Pulse Messages
    int m_coid;              // Connection ID fuer Channel Connection
    int m_interruptid;       // Interrupt ID (IRQ 11)
    struct sigevent m_event; // sigevent Struct
  };

  /**
   * Wechselt den aktuell gesetzten Stub
   **/
  void change_stub(abstract_inout* ptr);

  /**
   * Default Destruktor
   **/
  virtual ~hwaccess();

  /**
   * Setzt den Motor Modus (Aus, Rechtslauf, Linkslauf)
   * @param mode  Gibt den Modus an
   **/
  void set_motor(enum motor_modes mode);

  /**
   * Oeffnet die Weiche, wenn Weiche offen keine weitere Reaktion
   **/
  void open_switch();

  /**
   * Schliesst die Weiche, wenn Weiche geschlossen keine weitere Reaktion
   **/
  void close_switch();

  /**
   * Setzt eines der Ampellichter auf an oder aus
   * @param light Bestimmt die Lampe
   * @param on    Bestimmt ob an oder aus
   **/
  void set_light(enum light_colors light, bool on);

  /**
   * Prueft ob die angegebene Lichtschranke unterbrochen ist
   * @param  barrier Gibt die Lichtschranke an
   * @return TRUE  wenn Lichtschranke durchbrochen,
   *         FALSE wenn Lichtschranke geschlossen
   **/
  bool obj_in_light_barrier(enum light_barriers barrier) const;

  /**
   * Prueft ob das Objekt im Tolleranzbereich ist
   * @return TRUE  wenn Objekt im Tolleranzbereich
   *         FALSE wenn Objekt nicht im Tolleranzbereich
   **/
  bool obj_has_valid_height() const;

  /**
   * Ruft den absoluten Hoehenwert des Sensors ab
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
   * Prueft ob die Weiche offen ist
   * @return TRUE  wenn Weiche geoeffnet
   *         FALSE wenn Weiche geschlossen
   **/
  bool is_switch_open() const;

  /**
   * Setzt eine LED auf an oder aus
   * @param led Bestimmt die LED
   * @param on  Bestimmt ob an der aus
   **/
  void set_led_state(enum leds led, bool on);

  /**
   * Prueft ob ein Button gedrueckt ist
   * @param  key Bestimmt den zu pruefenden Button
   * @return TRUE  wenn Button gedrueckt
   *         FALSE wenn Button nicht gedrueckt
   **/
  bool is_button_pressed(enum buttons key) const;

  /**
   * Gibt den die Channel ID des Channels zurueck
   * den die ISR benutzt
   **/
  int get_isr_channel() const;

 private:
  static hwaccess* instance;
  /**
   * Initialisierung des Singletons
   **/
  virtual void initialize();

  /**
   * Zerstoerrung des Singleton
   **/
  virtual void destroy();

  /**
   * Private Konstruktoren
   **/
  hwaccess();
  hwaccess(const hwaccess&);

  /**
   * Initialisiert die ISR sowie die notwendigen
   * Datenstrutkuren.
   **/
  void init_isr();

  /**
   * ISR Abmelden
   * wird im Destructor aufgerufen
   **/
  void stop_isr();

  /**
   * Member deklaration
   **/
  abstract_inout* m_io;
  isr_control*    m_isr;
};

} // namespace hal
} // namespace se2

#endif // SE2_HWACCESS_HPP
