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
 * @file    state.cpp
 * @version 0.1
 *
 * Klasse fuer Token FSM
 **/

#include "events.hpp"
#include "state.hpp"
#include "token.hpp"
#include "lib/dispatcher/abstract_dispatcher.hpp"

using namespace se2;

class state : public events {
 protected:
  token* m_token;

 public:
  state(token* t) { m_token =  t; }
  ~state() { delete m_token; }

  virtual void event_entrance_sensor(void);
  virtual void event_height_sensor(void);
  virtual void event_switch_sensor(void);
  virtual void event_slide_sensor(void);
  virtual void event_exit_sensor(void);
  virtual void event_button_start(void);
};

class anonymous_token : public state {
 public:
  void event_entrance_sensor() {
    // Initialise token
  }
};

class b1_realized_object : public state {
 public:
  void event_height_sensor() {
    // Set height measurements
  }
};

class b1_height_measurement : public state {
 public:
    // Too small or metal detection
    // -> b1_token_too_small ||
    // -> b1_valid_height
    // Lambda Ubergaenge erzeugen
  }
};

class b1_token_too_small : public state {
 public:
  void event_slide_sensor() {
    // -> anonymous_token
  }
};

class b1_valid_height : public state {
 public:
  void event_switch_sensor() {
    // Detect metal
  }
};

class b1_metal_detection : public state {
 public:
  void event_exit_sensor() {

  }
};

class b1_exit : public state {
 public:
    // Lambda Uebergaenge erzeugen
    // -> b1_token_upside_down
    // -> b2_realized_object
};

class b1_token_upside_down : public state {
 public:
  void event_button_start() {
    // if turned -> b1_token_ready_for_b2
  }
};

class b1_token_ready_for_b2 : public state {
 public:
  void event_entrance_sensor() {
    // define for band2 must be activated!
    // band2 hast to be empty
  }
};

// Band 2

class b2_realized_object : public state {
 public:
  void event_height_sensor() {
    // Get token-data from serial interface
  }
};

class b2_height_measurement : public state {
 public:
  // Too small or metal detection
  // -> b2_token_upside_down ||
  // -> b2_valid_height
  // Lambda Uebergaenge erzeugen
};

class b2_token_upside_down : public state {
 public:
  void event_slide_sensor() {
    // Become anonymous token
  }
};

class b2_valid_height : public state {
 public:
  void event_switch_sensor() {
    // Detect metal
  }
};

class b2_metal_detection : public state {
 public:
  //
  //
};

class b2_exit : public state {
 public:
  void next_state(const token* t) {
    if (get_next_pulse_message() == SENSOR_EXIT) {
      new (this) anonymous_token;
    }
  }
};
