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
 * @version 0.1
 *
 * Manager fuer Token
 **/

#ifndef SE2_TOKEN_MGR_HPP
#define SE2_TOKEN_MGR_HPP

#include "config.h"

#include "lib/token.hpp"
#include "lib/util/abstract_singleton.hpp"

namespace se2 {
namespace fsm {
class state;
}
namespace util {
class singleton_mgr;
}
class token_mgr : public util::abstract_singleton {
  friend token;
  friend util::singleton_mgr;

  /**
   * `safe_state` Speichert alle wichtigen Zustaende
   * der Maschine zum zeitpunkt des drucks auf den E-Stop.
   **/
  struct safe_state {
    bool m_switch_open;
    bool m_motor_running;
  };

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
   * Sendet an conveyor 1 dass wieder Platz fuer einen Token ist
   **/
  void send_free();

  /**
   * Mit dieser Funktion meldet sich der neue `token`
   * einmal bei dem `token_mgr` an.
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void notify_existence(bool update = true);

  /**
   * Mit dieser Funktion meldet sich der `token`
   * vom `token_mgr` ab.
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void notify_death(bool update = true);

  /**
   * Schnellen Motor anfragen
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_fast_motor(bool update = true);

  /**
   * Langsamen Motor anfragen
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_slow_motor(bool update = true);

  /**
   * Motor linkslauf anfragen
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_left_motor(bool update = true);

  /**
   * Motor linkslauf zurueckziehen
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void unrequest_left_motor(bool update = true);

  /**
   * Motor stop anfragen
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void request_stop_motor(bool update = true);

  /**
   * Motor stop anfrage zurueckziehen
   * @param update sollte FALSE sein wenn `update()` nicht
   *               ausgefuehrt werden soll (Default: TRUE)
   **/
  void unrequest_stop_motor(bool update = true);

  /**
   * Neu an E-Stop event registrieren (Gedrueckt)
   **/
  void reregister_e_stop();

  /**
   * Neu an E-Stop event registrieren (Nicht mehr gedrueckt)
   **/
  void reregister_e_stop_rising();

  /**
   * System in sicheren Zustand bringen
   **/
  void enter_safe_state();

  /**
   * System wieder in alten Zustand bringen
   **/
  void exit_safe_state();

  /**
   * Prueft ob das aktueller Token zur korrekten
   * Reihenfolge gehoert.
   * @param  metal beschreibt ob das aktueller Token aus Metall ist
   * @return TRUE  wenn Reihenfolge korrekt
   *         FALSE wenn Reihenfolge nicht korrekt
   **/
  bool check_order(bool metal);

  /**
   * Prueft ob Conveyor 2 bereit fuer einen neuen Token ist.
   * @return TRUE  wenn conveyor 2 einen neuen Token bearbeiten kann
   *         FALSE wenn ein Token auf Conveyor 2 liegt
   **/
  bool check_conveyor2_ready() const;

  /**
   * Gibt dem `token_mgr` bescheid, dass ein Token an Conveyor 2
   * uebergeben wurde.
   **/
  void notify_token_trasition();

  /**
   * Wird aufgerufen sobald `B2_FREE` per Serieller Schnittstelle
   * empfangen wurde.
   **/
  void notify_ready_for_next();
 private:
  static token_mgr* instance;
  token m_tokens[NUM_OF_TOKENS];
  /**
   * Dieser Zustand ist permanent und wartet nur auf den E-Stop
   **/
  fsm::state* m_e_stop_listener;
  fsm::state* m_conveyor_free_listener;

  int            m_alife;
  int            m_motor_slow;
  bool           m_motor_stop;
  bool           m_motor_left;
  safe_state     m_safe;
  expected_token m_expected_token;
  bool           m_is_b2_ready;
};

}

#endif // SE2_TOKEN_MGR_HPP
