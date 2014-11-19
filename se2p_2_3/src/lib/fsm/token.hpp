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
 * Interface/Abstrakte Kontext Klasse f�r Puck/Token
 **/

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <cstdio>

#include "constants.hpp"
#include "events.hpp"

namespace se2 {
namespace fsm {

class token : public events {
 public:
  token() : m_state(NULL) {
    m_id = 0;
    m_height1 = 0;
    m_height2 = 0;
    m_is_metal = false;
    m_is_upside_down = 0;
  }

  ~token() { delete m_state; }

  virtual void set_state(events* state) { m_state = state; }

  int get_id();
  void set_id(int id);
  int get_next_id();
  int get_height1();
  void set_height1(int height1);
  int get_height2();
  void set_height2(int height2);
  bool get_is_upside_down();
  void set_is_upside_down(bool b);
  bool get_is_metal();
  void set_is_metal(bool b);
  bool is_valid();

  virtual void dispatched_event_button_start();
  virtual void dispatched_event_button_stop();
  virtual void dispatched_event_button_reset();
  virtual void dispatched_event_button_e_stop();
  virtual void dispatched_event_sensor_entrance();
  virtual void dispatched_event_sensor_height();
  virtual void dispatched_event_sensor_switch();
  virtual void dispatched_event_sensor_slide();
  virtual void dispatched_event_sensor_exit();
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
};

} // namespace fsm
} // namespace se2

#endif // TOKEN_HPP_
