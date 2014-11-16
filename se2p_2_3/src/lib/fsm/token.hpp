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
 * Interface/Abstrakte Klasse fuer Token Kontext
 **/

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

namespace se2 {

class token : public events {
 public:
  token();
  virtual ~token();

  int get_id();
  void set_id(int id);
  int get_height1();
  void set_height1(int height1);
  int get_height2();
  void set_height2(int height2);
  bool get_is_upside_down();
  void set_is_upside_down(bool b);
  bool get_is_metal();
  void set_is_metal(bool b);
  bool is_valid();

  virtual void event_entrance_sensor(void);
  virtual void event_height_sensor(void);
  virtual void event_switch_sensor(void);
  virtual void event_slide_sensor(void);
  virtual void event_exit_sensor(void);
  virtual void event_button_start(void);

 private:
  events* m_state;
  int m_id;
  int m_height1;
  int m_height2;
  bool m_is_upside_down;
  bool m_is_metal;
};

} // namespace se2

#endif // TOKEN_HPP_
