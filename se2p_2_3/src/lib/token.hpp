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
 * @file    token.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Kontext Klasse fuer Puck/Token
 **/

#ifndef SE2_TOKEN_HPP
#define SE2_TOKEN_HPP

#include "lib/fsm/events.hpp"

#include <vector>
#include <cstring>

#include <time.h>
#include <unistd.h>

namespace se2 {
namespace serial_bus {
class telegram;
}

typedef std::vector<int> ivec;

class token : public fsm::events {
 public:
  /**
   * Default Konstruktor
   **/
  token()
      : m_state(0), m_id(0), m_height1(0), m_height2(0)
      , m_is_metal(false), m_is_upside_down(false) {
    std::memset(&m_timespec_seg1, 0, sizeof(timespec));
    std::memset(&m_timespec_seg2, 0, sizeof(timespec));
    std::memset(&m_timespec_seg3, 0, sizeof(timespec));
    std::memset(&m_timespec_stop, 0, sizeof(timespec));
  }

  /**
   * Default Destruktor
   **/
  virtual ~token() {
    delete m_state;
  }

  /**
   * Setzt den Token auf einen neuen Zustand
   * @param state Ist ein Pointer auf den neuen Zustand
   **/
  void set_state(fsm::events* state) {
    delete m_state;
    m_state = state;
  }

  /**
   * Setzt alle Token Werte von einem Telegram
   * @param tel Referenz auf ein `telegram`
   **/
  void set_vals_from_telegram(const serial_bus::telegram& tel);

  /**
   * Gibt die ID des Tokens zurueck.
   * @return Eine unique ID
   **/
  int get_id() const;

  /**
   * Setzt den ersten Hoehenwert fuer diesen Token
   * @param height1 Der erste Hoehenwert des Tokens als Integer
   **/
  void set_height1(int height1);

  /**
   * Gibt den ersten Hoehenwert des Tokens zurueck
   * @returns Den ersten Hoehenwert des Tokens
   **/
  int get_height1() const;

  /**
   * Setzt den zweiten Hoehenwert fuer diesen Token
   * @param height2 Der zweiten Hoehenwert des Tokens als Integer
   **/
  void set_height2(int height2);

  /**
   * Gibt den zweiten Hoehenwert des Tokens zurueck
   * @returns Den zweiten Hoehenwert des Tokens
   **/
  int get_height2() const;

  /**
   * Setzt den Token auf verkehrt herum
   * @param value Gibt an ob der Token richtig oder falsch herum liegt
   **/
  void set_is_upside_down(bool value);

  /**
   * Fragt ab ob der Token richtig oder falsch herum liegt
   * @return TRUE  wenn der Token falsch herum liegt
   *         FALSE wenn der Token richtig herum liegt
   **/
  bool is_upside_down() const;

  /**
   * Setzt den Metallwert des Tokens
   * @param metal Ist der neue Metallwert des Tokens
   **/
  void set_is_metal(bool metal);

  /**
   * Gibt an ob der Token einen Metallring besitzt
   * @return TRUE  Wenn Metall im Token
   *         FALSE Wenn kein Metall im Token
   **/
  bool is_metal() const;

  /**
   * Resetet den Token, dieses geschieht beim Uebergang
   * vom benutzten Token zurueck auf `anonymous_token`
   * Ausserdem wird eine neue ID an den Token vergeben
   **/
  void reset();

  /**
   * Gibt alle Informationen des Tokens auf ueber
   * den standard output aus (std::cout)
   **/
  void pretty_print() const;

  /**
   * Fuegt eine Timer ID zu dieser Kontextklasse hinzu
   * @param idx ID des Timers
   **/
  void add_timer_id(int idx);

  /**
   * Loescht alle Timer die diesen Token zugeordnet sind
   **/
  void delete_timers();

  /**
   * Initialisiert die internen Zeiten
   * @param segment Segment Nummer der zu initialisierenden Zeit
   **/
  void init_internal_times(int segment);

  /**
   * Setzt die erwarteten Zeiten zrueck
   **/
  void reset_internal_times();

  /**
   * Fuegt Zeit zu den erwarteten Zeiten hinzu
   * @param sec  Sekunden die addiert werden sollen
   * @param msec Nanosekunden die addiert werden sollen
   **/
  void add_internal_times(int sec, long nsec);

  /**
   * Stoppt die erwartete Zeit
   **/
  void stop_internal_times();

  /**
   * Startet die erwartete Zeit
   **/
  void start_internal_times();

  /**
   * Prueft ob der Token zu frueh dran ist
   * @param  section Nummer des Segmentes
   * @return TRUE  Wenn Token in gueltiger Zeit
   *         FALSE Wenn Token in nicht gueltiger Zeit
   *               oder wenn `section` ungueltig
   **/
  bool check_internal_times(int section);

  /**
   * Aufrufe die an den Zustand delegiert werden
   **/
  virtual void dispatched_event_button_start();
  virtual void dispatched_event_button_stop();
  virtual void dispatched_event_button_reset();
  virtual void dispatched_event_button_e_stop();
  virtual void dispatched_event_sensor_entrance();
  virtual void dispatched_event_sensor_entrance_rising();
  virtual void dispatched_event_sensor_height();
  virtual void dispatched_event_sensor_height_rising();
  virtual void dispatched_event_sensor_switch();
  virtual void dispatched_event_sensor_switch_rising();
  virtual void dispatched_event_sensor_slide();
  virtual void dispatched_event_sensor_slide_rising();
  virtual void dispatched_event_sensor_exit();
  virtual void dispatched_event_sensor_exit_rising();
  virtual void dispatched_event_serial_data();
  virtual void dispatched_event_serial_msg();
  virtual void dispatched_event_serial_err();
  virtual void dispatched_event_serial_next_ok();
  virtual void dispatched_event_serial_transfer_fin();
  virtual void dispatched_event_serial_e_stopp();
  virtual void dispatched_event_serial_unk();
  virtual void dispatched_event_seg1_has_to_expire();
  virtual void dispatched_event_seg2_has_to_expire();
  virtual void dispatched_event_seg3_has_to_expire();
  virtual void dispatched_event_seg1_too_late();
  virtual void dispatched_event_seg2_too_late();
  virtual void dispatched_event_seg3_too_late();
  virtual void dispatched_event_slide_full_timeout();
  virtual void dispatched_event_turn_token_timeout();
  virtual void dispatched_event_remove_token_timeout();
  virtual void dispatched_event_close_switch_time();
  virtual void dispatched_event_tansfer_timeout();

 private:
  events*    m_state;
  int        m_id;
  static int m_id_counter;
  int        m_height1;
  int        m_height2;
  bool       m_is_metal;
  bool       m_is_upside_down;
  ivec       m_timer_ids;

  // interne erwartete Zeiten, fuer Guard Benutzung
  timespec   m_timespec_seg1;
  timespec   m_timespec_seg2;
  timespec   m_timespec_seg3;
  timespec   m_timespec_stop;
};

} // namespace se2

#endif // SE2_TOKEN_HPP
