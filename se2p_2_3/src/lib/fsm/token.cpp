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
 * Klasse fuer Token Kontext
 **/

#include "token.hpp"

using namespace se2;

class token : public events {
 public:
  token() : m_state(NULL) {
    m_id = 0;
    m_height1 = 0;
    m_height2 = 0;
    m_is_upside_down = 0;
    m_is_metal = 0;
  }

  virtual ~token(){
    dispatcher* dsp = dispatcher::getInstance();
    dsp->unregister_from_all(this); // Sich selbst aus allen Event-Queues entfernen

    // State Objekt loeschen
    if (m_state != NULL) {
      delete m_state;
      m_state = NULL;
    }
  }

  int get_id() { return m_id; }
  void set_id(int id) { m_id = id; }

  int get_height1() { return m_height1; }
  void set_height1(int height1) { m_height1 = height1; }

  int get_height2() { return m_height2; }
  void set_height2(int height2) { m_height2 = height2; }

  bool get_is_upside_down() { return m_is_upside_down; }
  void set_is_upside_down(bool b) { m_is_upside_down = b; }

  bool get_is_metal() { return m_is_metal; }
  void set_is_metal(bool b) { m_is_metal = b; }

  bool is_valid() const {
    return !(m_id == 0 && m_height1 == 0 && m_height2 == 0);
  }

  virtual void event_entrance_sensor(void) {
    m_state->event_entrance_sensor();
  }

  virtual void event_height_sensor(void) {
    m_state->event_height_sensor();
  }

  virtual void event_switch_sensor(void) {
    m_state->event_switch_sensor();
  }

  virtual void event_slide_sensor(void) {
    m_state->event_slide_sensor();
  }

  virtual void event_exit_sensor(void) {
    m_state->event_exit_sensor();
  }

  virtual void event_button_start(void) {
    m_state->event_button_start();
 }

 private:
  events* m_state;
  int m_id;
  int m_height1;
  int m_height2;
  bool m_is_upside_down;
  bool m_is_metal;
};
