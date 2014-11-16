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
 * @file    state.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse fuer Token FSM
 **/

#ifndef STATE_HPP_
#define STATE_HPP_

namespace se2 {

class state : public events {
 public:
  state(token* t) { }
  virtual ~state() { }

  virtual void event_entrance_sensor(void);
  virtual void event_height_sensor(void);
  virtual void event_switch_sensor(void);
  virtual void event_slide_sensor(void);
  virtual void event_exit_sensor(void);
  virtual void event_button_start(void);
};

} // namespace se2

#endif /* STATE_HPP_ */
