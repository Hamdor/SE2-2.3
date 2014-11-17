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
 * Interface/Abstrakte Klasse f�r Zustandsautomaten
 **/

#ifndef STATE_HPP_
#define STATE_HPP_

namespace se2 {
namespace fsm {

class state : public events {
 public:
  state(token* t) {
    // nop
  }

  virtual ~state() {
    // nop
  }

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
};

class anonymous_token : public state {
 public:
  anonymous_token(token* t);
  ~anonymous_token();

  virtual void dispatched_event_sensor_entrance();
};


/******************************************************************************
 *                                BAND 1 FSM                                  *
 ******************************************************************************/

class b1_realized_object : public state {
 public:
  b1_realized_object(token* t);
  ~b1_realized_object();

  virtual void dispatched_event_sensor_height();
};

class b1_height_measurement : public state {
 public:
  b1_height_measurement(token* t);
  ~b1_height_measurement();
};

class b1_token_too_small : public state {
 public:
  b1_token_too_small(token* t);
  ~b1_token_too_small();

  virtual void dispatched_event_sensor_slide();
};

class b1_valid_height : public state {
 public:
  b1_valid_height(token* t);
  ~b1_valid_height();

  virtual void dispatched_event_sensor_switch();
};

class b1_metal_detection : public state {
 public:
  b1_metal_detection(token* t);
  ~b1_metal_detection();

  virtual void dispatched_event_sensor_exit();
};

class b1_exit : public state {
 public:
  b1_exit(token* t);
  ~b1_exit();
};

class b1_token_upside_down : public state {
 public:
  b1_token_upside_down(token* t);
  ~b1_token_upside_down();

  virtual void dispatched_event_button_start();
};

class b1_token_ready_for_b2 : public state {
 public:
  b1_token_ready_for_b2(token* t);
  ~b1_token_ready_for_b2();

  virtual void dispatched_event_sensor_entrance();
};


/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/

class b2_realized_object : public state {
 public:
  b2_realized_object(token* t);
  ~b2_realized_object();

  virtual void dispatched_event_sensor_height();
};

class b2_height_measurement : public state {
 public:
  b2_height_measurement(token* t);
  ~b2_height_measurement();
};

class b2_token_upside_down : public state {
 public:
  b2_token_upside_down(token* t);
  ~b2_token_upside_down();

  virtual void dispatched_event_sensor_slide();
};

class b2_valid_height : public state {
 public:
  b2_valid_height(token* t);
  ~b2_valid_height();

  virtual void dispatched_event_sensor_switch();
};

class b2_metal_detection : public state {
 public:
  b2_metal_detection(token* t);
  ~b2_metal_detection();
};

class b2_is_wrong_order : public state {
 public:
  b2_is_wrong_order(token* t);
  ~b2_is_wrong_order();

  virtual void dispatched_event_sensor_entrance();
};

class b2_is_correct_order : public state {
 public:
  b2_is_correct_order(token* t);
  ~b2_is_correct_order();

  virtual void dispatched_event_sensor_exit();
};

} // namespace fsm
} // namespace se2

#endif /* STATE_HPP_ */
