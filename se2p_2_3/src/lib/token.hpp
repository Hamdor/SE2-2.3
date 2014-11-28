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

#include "events.hpp"

namespace se2 {

class token : public fsm::events {
 public:

  /**
   * Default Konstruktor
   **/
  token()
      : m_state(0), m_id(0), m_height1(0), m_height2(0)
      , m_is_metal(false), m_is_upside_down(false), m_valid_height(false) {
    // nop
  }

  /**
   * Default Destruktor
   **/
  virtual ~token() {
    delete m_state;
  }

  /**
   * Setzt den Token auf einen neuen Zustand
   * @param state ist ein Pointer auf den neuen Zustand
   **/
  void set_state(fsm::events* state) {
    m_state = state;
  }

  /**
   * Gibt die id des Tokens zurueck.
   * @return eine unique ID
   **/
  int get_id() const;

  /**
   * Setzt den ersten Hoehenwert fuer diesen Token
   * @param height1 der erste Hoehenwert des Tokens als Integer
   **/
  void set_height1(int height1);

  /**
   * Gibt den ersten Hoehenwert des Tokens zurueck.
   * @returns den ersten Hoehenwert des Tokens
   **/
  int get_height1() const;

  /**
   * Setzt den zweiten Hoehenwert fuer diesen Token
   * @param height2 der zweiten Hoehenwert des Tokens als Integer
   **/
  void set_height2(int height2);

  /**
   * Gibt den zweiten Hoehenwert des Tokens zurueck.
   * @returns den zweiten Hoehenwert des Tokens
   **/
  int get_height2() const;


  /**
   * Setzt den Token auf verkehrt herum
   * @param value gibt an ob der Token richtig oder falsch herum liegt
   **/
  void set_is_upside_down(bool value);

  /**
   * Fragt ab ob der Token richtig oder falsch herum liegt
   * @return TRUE  wenn der Token falsch herum liegt
   *         FALSE wenn der Token richtig herum liegt
   **/
  bool get_is_upside_down() const;

  /**
   * Setzt den Metallwert des Tokens
   * @param metal ist der neue Metallwert des Tokens
   **/
  void set_is_metal(bool metal);

  /**
   * Gibt an ob der Token einen Metallring besitzt
   * @return TRUE  wenn Metall im Token
   *         FALSE wenn kein Metall im Token
   **/
  bool get_is_metal() const;

  /**
   * Resetet den Token, dieses geschieht beim uebergang
   * vom benutzten Token zurueck auf `anonymous_token`.
   * Ausserdem wird eine neue ID an den Token vergeben.
   **/
  void reset();

  /**
   * Gibt alle Informationen des Tokens auf ueber
   * den standard output aus. (std::cout)
   **/
  void pretty_print() const;

  virtual void dispatched_event_button_start();
  virtual void dispatched_event_button_stop();
  virtual void dispatched_event_button_reset();
  virtual void dispatched_event_button_e_stop();
  virtual void dispatched_event_sensor_entrance();
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
  virtual void dispatched_event_serial_unk();
  virtual void dispatched_event_seg1_exceeded();
  virtual void dispatched_event_seg2_exceeded();
  virtual void dispatched_event_seg3_exceeded();
  virtual void dispatched_event_slide_full();
  virtual void dispatched_event_open_switch();
  virtual void dispatched_event_turn_token();
  virtual void dispatched_event_remove_token();
  virtual void dispatched_event_token_finished();

 private:
  events* m_state;
  int m_id;
  static int m_id_counter;
  int m_height1;
  int m_height2;
  bool m_is_metal;
  bool m_is_upside_down;
  bool m_valid_height;
};

} // namespace se2

#endif // SE2_TOKEN_HPP
