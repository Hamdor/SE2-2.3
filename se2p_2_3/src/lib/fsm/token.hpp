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
 * Interface/Abstrakte Kontext Klasse für Puck/Token
 **/

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

namespace se2 {
namespace fsm {

using namespace se2::hal;

class token : public events {
 public:
  token();
  virtual ~token();

  int get_id();
  void set_id(int id);
  int get_next_id();
  int get_height1();
  void set_height1(int height1);
  int get_height2();
  void set_height2(int height2);
  bool get_is_upside_down();
  void set_is_upside_down(bool b);
  token_types get_type();
  void set_type(token_types type);
  bool is_valid();

  virtual void dispatched_event_button_start(void);
  virtual void dispatched_event_button_stop(void);
  virtual void dispatched_event_button_reset(void);
  virtual void dispatched_event_button_e_stop(void);
  virtual void dispatched_event_sensor_entrance(void);
  virtual void dispatched_event_sensor_height(void);
  virtual void dispatched_event_sensor_switch(void);
  virtual void dispatched_event_sensor_slide(void);
  virtual void dispatched_event_sensor_exit(void);
  virtual void dispatched_event_serial_data(void);
  virtual void dispatched_event_serial_msg(void);
  virtual void dispatched_event_serial_err(void);
  virtual void dispatched_event_serial_unk(void);
  virtual void dispatched_event_seg1_exceeded(void);
  virtual void dispatched_event_seg2_exceeded(void);
  virtual void dispatched_event_seg3_exceeded(void);
  virtual void dispatched_event_slide_full(void);
  virtual void dispatched_event_open_switch(void);
  virtual void dispatched_event_turn_token(void);
  virtual void dispatched_event_remove_token(void);
  virtual void dispatched_event_token_finished(void);
  virtual void dispatched_event_max(void);

 private:
  events* m_state;
  int m_id;
  static int m_id_counter;
  int m_height1;
  int m_height2;
  token_types m_type;
  bool m_is_upside_down;
};

} // namespace fsm
} // namespace se2

#endif // TOKEN_HPP_
