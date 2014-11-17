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
 * @file    token.cpp
 * @version 0.1
 *
 * Kontext Klasse f�r Puck/Token
 **/

#include "token.hpp"

using namespace se2::fsm;
using namespace se2::hal;

class token : public events {
 public:
  token() : m_state(anonymous_token) {
    m_id = 0;
    m_height1 = 0;
    m_height2 = 0;
    m_type = NO_METAL;
    m_is_upside_down = 0;
  }

  virtual ~token(){
    // Sich aus allen Event-Queues entfernen und das state-Objekt l�schen
    dispatcher* disp = dispatcher::getInstance();
    disp->unregister_from_all(this);
    delete m_state;
  }

  int get_id() { return m_id; }
  void set_id(int id) { m_id = id; }

  int get_next_id() { return ++m_id_counter; }

  int get_height1() { return m_height1; }
  void set_height1(int height1) { m_height1 = height1; }

  int get_height2() { return m_height2; }
  void set_height2(int height2) { m_height2 = height2; }

  bool get_is_upside_down() { return m_is_upside_down; }
  void set_is_upside_down(bool b) { m_is_upside_down = b; }

  token_types get_type() { return m_type; }
  void set_type(token_types type) { m_type = type; }

  bool is_valid() const {
    return !(m_id == 0 && m_height1 == 0 && m_height2 == 0);
  }

  virtual void dispatched_event_button_start() {
    m_state->dispatched_event_button_start();
  }

  virtual void dispatched_event_button_stop() {
    m_state->dispatched_event_button_stop();
  }

  virtual void dispatched_event_button_reset() {
    m_state->dispatched_event_button_reset();
  }

  virtual void dispatched_event_button_e_stop() {
    m_state->dispatched_event_button_e_stop();
  }

  virtual void dispatched_event_sensor_entrance() {
    m_state->dispatched_event_sensor_entrance();
  }

  virtual void dispatched_event_sensor_height() {
    m_state->dispatched_event_sensor_height();
  }

  virtual void dispatched_event_sensor_switch() {
    m_state->dispatched_event_sensor_switch();
  }

  virtual void dispatched_event_sensor_slide() {
    m_state->dispatched_event_sensor_slide();
  }

  virtual void dispatched_event_sensor_exit() {
    m_state->dispatched_event_sensor_exit();
  }

  virtual void dispatched_event_serial_data() {
    m_state->dispatched_event_serial_data();
  }

  virtual void dispatched_event_serial_msg() {
    m_state->dispatched_event_serial_msg();
  }

  virtual void dispatched_event_serial_err() {
    m_state->dispatched_event_serial_err();
  }

  virtual void dispatched_event_serial_unk() {
    m_state->dispatched_event_serial_unk();
  }

  virtual void dispatched_event_seg1_exceeded() {
    m_state->dispatched_event_seg1_exceeded();
  }

  virtual void dispatched_event_seg2_exceeded() {
    m_state->dispatched_event_seg2_exceeded();
  }

  virtual void dispatched_event_seg3_exceeded() {
    m_state->dispatched_event_seg3_exceeded();
  }

  virtual void dispatched_event_slide_full() {
    m_state->dispatched_event_slide_full();
  }

  virtual void dispatched_event_open_switch() {
    m_state->dispatched_event_open_switch();
  }

  virtual void dispatched_event_turn_token() {
    m_state->dispatched_event_turn_token();
  }

  virtual void dispatched_event_remove_token() {
    m_state->dispatched_event_remove_token();
  }

  virtual void dispatched_event_token_finished() {
    m_state->dispatched_event_token_finished();
  }

  virtual void dispatched_event_max() {
    m_state->dispatched_event_max();
  }

 private:
  events* m_state;
  int m_id;
  static int m_id_counter;
  int m_height1;
  int m_height2;
  token_types m_type;
  bool m_is_upside_down;
};

int token::m_id_counter = 1; // Beginne fortlaufende ID's der Pucks bei 1
