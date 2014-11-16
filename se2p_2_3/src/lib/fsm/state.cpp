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
 * Klasse für Zustandsautomaten
 **/

#include "events.hpp"
#include "state.hpp"
#include "token.hpp"
#include "lib/hal/HWaccess.hpp"
#include "lib/util/singleton_mgr.hpp"
#include "lib/dispatcher/abstract_dispatcher.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatcher;

class state : public events {
 protected:
  token* m_token;

 public:
  state(token* t) { m_token =  t; }
  ~state() { delete m_token; }

  /* Zu implementierende Funktionen
   *
  void dispatched_event_button_start();
  void dispatched_event_button_stop();
  void dispatched_event_button_reset();
  void dispatched_event_button_e_stop();
  void dispatched_event_sensor_entrance();
  void dispatched_event_sensor_height();
  void dispatched_event_sensor_switch();
  void dispatched_event_sensor_slide();
  void dispatched_event_sensor_exit();
  void dispatched_event_serial_data();
  void dispatched_event_serial_msg();
  void dispatched_event_serial_err();
  void dispatched_event_serial_unk();
  void dispatched_event_seg1_exceeded();
  void dispatched_event_seg2_exceeded();
  void dispatched_event_seg3_exceeded();
  void dispatched_event_slide_full();
  void dispatched_event_open_switch();
  void dispatched_event_turn_token();
  void dispatched_event_remove_token();
  void dispatched_event_token_finished();
  void dispatched_event_max();
  */
};

/* Anonymer Puck als Initialisierungszustand */
class anonymous_token : public state {
 public:
  anonymous_token(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
  }

  ~anonymous_token() { }

  void dispatched_event_sensor_entrance() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
    // ID zuweisen und Motor des Laufbands im Rechtslauf starten
    m_token->set_id(m_token->get_next_id());
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    hal->set_motor(MOTOR_RIGHT);
    // Wechsel in den nächsten Zustand
    new (this) b1_realized_object(this->m_token);
  }
};


/******************************************************************************
 *                                BAND 1 FSM                                  *
 ******************************************************************************/

class b1_realized_object : public state {
 public:
  b1_realized_object(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_HEIGHT);
  }

  ~b1_realized_object() { }

  void dispatched_event_sensor_height() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_HEIGHT);
    // TODO:
    // Motor langsam laufen lassen für eine bestimmte Zeit (1-2 Sekunden?)
    // Höhenmesswerte auslesen und im Puck ablegen
  }
};

class b1_height_measurement : public state {
 public:
    // TODO: Lambda-Zustandswechel
    // 1. Höhe NICHT OK -> b1_token_too_small
    // 2. Höhe OK       -> b1_valid_height
};

class b1_token_too_small : public state {
 public:
  b1_token_too_small(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_SLIDE);
  }

  ~b1_token_too_small() { }

  void dispatched_event_sensor_slide() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_SLIDE);
    // TODO:
    // Puck wird zu anonymous_token
  }
};

class b1_valid_height : public state {
 public:
  b1_valid_height(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_SWITCH);
  }

  ~b1_valid_height() { }

  void dispatched_event_sensor_switch() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_SWITCH);
    // TODO:
    // Detektieren des Metalkerns
  }
};

class b1_metal_detection : public state {
 public:
  b1_metal_detection(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_EXIT);
  }

  ~b1_metal_detection() { }

  void dispatched_event_sensor_exit() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_EXIT);
    // TODO:
    // ?
  }
};

class b1_exit : public state {
 public:
    // TODO: Lambda-Zustandswechel
    // 1. Puck NICHT gewendet -> b1_token_upside_down
    // 2. Puck gewendet       -> b2_realized_object
};

class b1_token_upside_down : public state {
 public:
  b1_token_upside_down(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_BUTTON_START);
  }

  ~b1_token_upside_down() { }

  void dispatched_event_button_start() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_BUTTON_START);
    // TODO:
    // Wenn erfolgreich gewendet -> b1_token_ready_for_b2
    // (Im Hintergrund läuft sonst der Timer für den Wendevorgang ab)
  }
};

class b1_token_ready_for_b2 : public state {
 public:
  b1_token_ready_for_b2(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
  }

  ~b1_token_ready_for_b2() { }

  void dispatched_event_sensor_entrance() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
    // TODO:
    // 1. Define für Band 2 abfragen
    // 2. Band 2 muss leer sein
    // 3. Daten des Pucks an Band 2 schicken?
    // 4. Puck nach erfolgreicher Übertragung auf Band 1 wieder anonym machen?
  }
};


/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/

class b2_realized_object : public state {
 public:
  b2_realized_object(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_HEIGHT);
  }

  ~b2_realized_object() { }

  void dispatched_event_sensor_height() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_HEIGHT);
    // TODO:
    // 1. Define für Band 2 abfragen
    // 2. Daten des Pucks von Band 1 anfordern?
  }
};

class b2_height_measurement : public state {
 public:
  // TODO: Lambda-Zustandswechel
  // 1. Puck NICHT gewendet -> b2_token_upside_down
  // 2. Puck gewendet       -> b2_valid_height
};

class b2_token_upside_down : public state {
 public:
  b2_token_upside_down(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_SLIDE);
  }

  ~b2_token_upside_down() { }

  void dispatched_event_sensor_slide() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_SLIDE);
    // TODO:
    // 1. Define für Band 2 abfragen
    // 2. Puck wird zu anonymous_token
  }
};

class b2_valid_height : public state {
 public:
  b2_valid_height(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_SWITCH);
  }

  ~b2_valid_height() { }

  void dispatched_event_sensor_switch() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_SWITCH);
    // TODO:
    // 1. Define für Band 2 abfragen
    // 2. Detektieren des Metalkerns
  }
};

class b2_metal_detection : public state {
 public:
  // TODO: Lambda-Zustandswechel
  // 1. Reihenfolge NICHT korrekt -> b2_is_wrong_order
  // 2. Reihenfolge korrekt       -> b2_is_correct_order
};

class b2_is_wrong_order : public state {
 public:
  b2_is_wrong_order(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
  }

  ~b2_is_wrong_order() { }

  void dispatched_event_sensor_entrance() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
    // TODO:
    // 1. Define von Band 2 abfragen
    // 2. Motor von Band 2 anhalten
    // 3. Entfernungs-Vorgang einleiten und Puck zu anonymous_token machen
  }
};

class b2_is_correct_order : public state {
 public:
  b2_is_correct_order(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_EXIT);
  }

  ~b2_is_correct_order() { }

  void dispatched_event_sensor_exit() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SENSOR_EXIT);
    // TODO:
    // 1. Define für Band 2 abfragen
    // 2. Weiche schliessen -> hwaccess::close_switch();
    // 3. Entfernungs-Vorgang einleiten und Puck zu anonymous_token machen
  }
};
