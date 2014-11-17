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
using namespace se2::serial_bus;

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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    hal->set_motor(MOTOR_SLOW);
    // TODO: Prüfen, ob Timer für die langsame Strecke notwendig ist
    // m_token->set_is_upside_down(hal->get_is_upside_down()); // FIXME
    new (this) b1_height_measurement(this->m_token);
  }
};

class b1_height_measurement : public state {
 public:
  b1_height_measurement(token* t) : state::state(t) {
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));

    if (hal->obj_has_valid_height()) {
      m_token->set_height1(hal->get_height_value());
      hal->set_motor(MOTOR_FAST);
      new (this) b1_valid_height(this->m_token);
    } else {
      hal->set_motor(MOTOR_FAST);
      hal->close_switch();
      new (this) b1_token_too_small(this->m_token);
    }
  }

  ~b1_height_measurement() { }
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
    new (this) anonymous_token(this->m_token);
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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    hal->open_switch();
    m_token->set_type(hal->obj_has_metal());
    new (this) b1_metal_detection(this->m_token);
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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    hal->close_switch();
    new (this) b1_exit(this->m_token);
  }
};

class b1_exit : public state {
 public:
  b1_exit(token* t) : state::state(t) {
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));

    if (m_token->get_is_upside_down()) {
      hal->set_motor(MOTOR_STOP);
      // TODO: Springt hier in Fehlerbehandlung
      new (this) b1_token_upside_down(this->m_token);
    } else {
      if (1/* FIXME hal oder dispatcher abfragen, ob Band 2 frei ist */) {
        new (this) b1_token_ready_for_b2(this->m_token);
      }
    }
  }

  ~b1_exit() { }
};

class b1_token_upside_down : public state { // TODO: Wird ausgelagert in eigenen Fehlerzustand
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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
    // TODO:
    // 1. Define für Band 2 abfragen
    // 2. Band 2 muss leer sein (Queue von Band 2 mit Größe 1 muss leer sein / anonymous_token)
          // 3. Wenn alles OK, Motor starten: hal->set_motor(MOTOR_RIGHT);

    // Telegramm fuer den Versand erstellen
    telegram* tg = new telegram;
    tg->m_type = m_token->get_type();
    tg->m_id = m_token->get_id();
    tg->m_height1 = m_token->get_height1();

    serial->send_telegram(tg); // 4. Daten des Pucks an Band 2 schicken?
    // TODO: Ausgabe der Puck Daten auf dem Terminal
    new (this) b2_receive_data(this->m_token);
  }
};


/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/


class b2_receive_data : public state {
 public:
  b2_receive_data(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SERIAL_DATA);
  }

  ~b2_receive_data() { }

  void dispatched_event_serial_data() {
    // Beende das Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->unregister_listener(this->m_token, EVENT_SERIAL_DATA);
    serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));

    // Werte aus Telegramm holen
    telegram* t = serial->get_telegram();
    this->m_token->set_id(t->m_id);
    this->m_token->set_height1(t->m_height1);
    this->m_token->set_type(t->m_type);
  }
};

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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    hal->set_motor(MOTOR_SLOW);
    // TODO: Prüfen, ob Timer für die langsame Strecke notwendig ist
    // 1. Define für Band 2 abfragen
    // m_token->set_is_upside_down(hal->get_is_upside_down()); // FIXME
    new (this) b2_height_measurement(this->m_token);
  }
};

class b2_height_measurement : public state {
 public:
  b2_height_measurement(token* t) : state::state(t) {
    // TODO: Define für Band 2 abfragen
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));

    if (1 /* hal->obj_is_upside_down() */) {
      hal->set_motor(MOTOR_FAST);
      hal->close_switch();
      new (this) b2_token_upside_down(this->m_token);
    } else {
      m_token->set_height2(hal->get_height_value());
      hal->set_motor(MOTOR_FAST);
      new (this) b2_valid_height(this->m_token);
    }
  }

  ~b2_height_measurement() { }
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
    new (this) anonymous_token(this->m_token);
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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    m_token->set_is_metal(hal->obj_has_metal());
    new (this) b2_metal_detection(this->m_token);
  }
};

class b2_metal_detection : public state {
 public:
  b2_metal_detection() {
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));

    if (1 /* wrong_order */) { // FIXME
      hal->set_motor(MOTOR_LEFT);
      new (this) b2_is_wrong_order(this->m_token);
    } else {
      hal->open_switch();
      new (this) b2_is_correct_order(this->m_token);
    }
  }

  ~b2_metal_detection() { }
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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    // TODO 1. Define von Band 2 abfragen
    hal->set_motor(MOTOR_STOP);// 2. Motor von Band 2 anhalten
    // TODO: Entfernungs-Vorgang einleiten und dort Puck zu anonymous_token machen
    // new (this) err_remove_token(this->m_token);
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
    hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    // TODO: 1. Define für Band 2 abfragen
    hal->set_motor(MOTOR_STOP);
    hal->close_switch();
    // new (this) token_finished(this->m_token); // Entfernungs-Vorgang einleiten und Puck zu anonymous_token machen
  }
};


/******************************************************************************
 *                                ERROR FSM                                   *
 ******************************************************************************/

// class err_b1_...
