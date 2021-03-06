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
 * @file    token_mgr.hpp
 * @version 0.2
 *
 * Manager fuer Token
 **/

#ifndef SE2_TOKEN_MGR_HPP
#define SE2_TOKEN_MGR_HPP

#include "config.h"

#include "lib/token.hpp"
#include "lib/util/abstract_singleton.hpp"

#include <time.h>

namespace se2 {
namespace util {
class singleton_mgr;
}
class token_mgr : public util::abstract_singleton {
  friend token;
  friend util::singleton_mgr;

  /**
   * Enum fuer den als naechstes erwarteten Token,
   * ALL     - Egal welcher Token (da vorher noch keiner auf dem Band war)
   * METAL   - Nur Metall wird akzeptiert
   * PLASTIC - Nur Plastic wird akzeptiert
   **/
  enum expected_token {
    ALL,
    METAL,
    PLASTIC
  };

  /**
   * Default Konstruktor
   **/
  token_mgr();

  /**
   * Destruktor
   **/
  virtual ~token_mgr();

  virtual void initialize();
  virtual void destroy();

  /**
   * Motoreinstellungen aktualisieren, dieses geschieht
   * abhaengig der Werte von `m_alife`, `m_motor_slow` sowie
   * `m_motor_stop`.
   **/
  void update();
 public:
  /**
   * Sendet an conveyor 1 das wieder Platz fuer einen Token ist
   **/
  void send_free();

  /**
   * Mit dieser Funktion meldet sich der neue `token`
   * einmal bei dem `token_mgr` an
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void notify_existence(bool update = true);

  /**
   * Mit dieser Funktion meldet sich der `token`
   * vom `token_mgr` ab
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void notify_death(bool update = true);

  /**
   * Schnellen Motor anfragen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_fast_motor(bool update = true);

  /**
   * Langsamen Motor anfragen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_slow_motor(bool update = true);

  /**
   * Motor linkslauf anfragen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_left_motor(bool update = true);

  /**
   * Motor linkslauf zurueckziehen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void unrequest_left_motor(bool update = true);

  /**
   * Motor stop anfragen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_stop_motor(bool update = true);

  /**
   * Motor stop Anfrage zurueckziehen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void unrequest_stop_motor(bool update = true);

  /**
   * Weiche oeffnen anfragen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_open_switch(bool update = true);

  /**
   * Weiche oeffnen Anfrage zurueckziehen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void unrequest_open_switch(bool update = true);

  /**
   * Token drehen anfragen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_turnover(bool update = true);

  /**
   * Token drehen Anfrage zurueckziehen
   * @param update Sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void unrequest_turnover(bool update = true);

  /**
   * System in sicheren Zustand bringen
   * @param send_serial Beschreibt ob die Information, dass
   *        der `safe_state` betreten wurde, ueber die Serielle-
   *        Schnittstelle gesendet werden soll (Default: TRUE)
   **/
  void enter_safe_state(bool send_serial = true);

  /**
   * Prueft ob das aktueller Token zur korrekten
   * Reihenfolge gehoert
   * @param  metal Beschreibt ob das aktueller Token aus Metall ist
   * @return TRUE  Wenn Reihenfolge korrekt
   *         FALSE Wenn Reihenfolge nicht korrekt
   **/
  bool check_order(bool metal);

  /**
   * Prueft ob Conveyor 2 bereit fuer einen neuen Token ist
   * @return TRUE  Wenn conveyor 2 einen neuen Token bearbeiten kann
   *         FALSE Wenn ein Token auf Conveyor 2 liegt
   **/
  bool check_conveyor2_ready() const;

  /**
   * Gibt dem `token_mgr` bescheid, dass ein Token an Conveyor 2
   * uebergeben wurde
   **/
  void notify_token_trasition();

  /**
   * Wird aufgerufen sobald `B2_FREE` per Serieller Schnittstelle
   * empfangen wurde
   **/
  void notify_ready_for_next();

  /**
   * Ein Event wurde ausgeloest wo nicht sicher ist welches Werkstueck
   * es ausgeloest hat. Wahrscheinlich wurde ein Werkstueck mitten auf
   * das Band gelegt und hat eine Schranke durchbrochen auf die von
   * keinem anderen Werkstueck gehoert wurde
   **/
  void notify_pseudo_token_not_known_event();

  /**
   * Pruft ob der Motor langsam laeuft
   * @return TRUE  wenn der Motor langsam laeuft
   *         FALSE wenn der Motor schnell laeuft
   **/
  bool is_motor_slow() const;

  /**
   * Prueft ob der Motor gestoppt ist
   * @return TRUE  wenn der Motor gestoppt ist
   *         FALSE wenn der Motor laeuft
   **/
  bool is_motor_stopped() const;

  /**
   * Gibt die verbleibende Zeit zurueck bis der Motor wieder
   * schnell gestellt wird
   * @return ein neues timespec mit der restlichen Zeit
   **/
  timespec get_motor_slow_diff() const;

 private:
  static token_mgr* instance;
  token             m_tokens[NUM_OF_TOKENS];
  token             m_pseudo_token;
  int               m_alife;
  int               m_motor_slow;
  int               m_switch_open;
  int               m_wait_turnover;
  int               m_motor_stop;
  bool              m_motor_left;
  expected_token    m_expected_token;
  bool              m_is_b2_ready;
  bool              m_safe_state;
  timespec          m_motor_slow_time;
};

}

#endif // SE2_TOKEN_MGR_HPP
