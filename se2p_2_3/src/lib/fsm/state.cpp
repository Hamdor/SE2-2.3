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
 * Klasse fuer Zustandsautomaten
 **/

#include "state.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::serial_bus;

state::~state() { delete m_token; }

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
*/

void state::dispatched_event_button_start() {
  // nop
}

void state::dispatched_event_button_stop() {
  // nop
}

void state::dispatched_event_button_reset() {
  // nop
}

void state::dispatched_event_button_e_stop() {
  // nop
}

void state::dispatched_event_sensor_entrance() {
  // nop
}

void state::dispatched_event_sensor_height() {
  // nop
}

void state::dispatched_event_sensor_switch() {
  // nop
}

void state::dispatched_event_sensor_slide() {
  // nop
}

void state::dispatched_event_sensor_exit() {
  // nop
}

void state::dispatched_event_serial_data() {
  // nop
}

void state::dispatched_event_serial_msg() {
  // nop
}

void state::dispatched_event_serial_err() {
  // nop
}

void state::dispatched_event_serial_unk() {
  // nop
}

void state::dispatched_event_seg1_exceeded() {
  // nop
}

void state::dispatched_event_seg2_exceeded() {
  // nop
}

void state::dispatched_event_seg3_exceeded() {
  // nop
}

void state::dispatched_event_slide_full() {
  // nop
}

void state::dispatched_event_open_switch() {
  // nop
}

void state::dispatched_event_turn_token() {
  // nop
}

void state::dispatched_event_remove_token() {
  // nop
}

void state::dispatched_event_token_finished() {
  // nop
}

// anonymous_token
anonymous_token::anonymous_token(token* t) : state::state(t) {
  std::cout << "Konstruktor von anonymous_token()" << std::endl; //FIXME
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
}

anonymous_token::~anonymous_token() { }

void anonymous_token::dispatched_event_sensor_entrance() {
  std::cout << "Aufruf von anonymous_token::dispatched_event_sensor_entrance()" << std::endl; //FIXME
  // ID zuweisen und Motor des Laufbands im Rechtslauf starten
  m_token->set_id(m_token->get_next_id());
  // Wechsel in den naechsten Zustand
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_STOP);
#ifdef IS_CONVEYOR_1
  new (this) b1_realized_object(this->m_token);
#endif
#ifdef IS_CONVEYOR_2
  new (this) b2_receive_data(this->m_token);
#endif
}



/******************************************************************************
 *                                BAND 1 FSM                                  *
 ******************************************************************************/
#ifdef IS_CONVEYOR_1
// b1_realized_object
b1_realized_object::b1_realized_object(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_realized_object()" << std::endl; //FIXME
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_HEIGHT);

  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_RIGHT);
}

b1_realized_object::~b1_realized_object() { }

void b1_realized_object::dispatched_event_sensor_height() {
  std::cout << "b1_realized_object::dispatched_event_sensor_height()" << std::endl; //FIXME
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_SLOW);
  // TODO: Pruefen, ob Timer fuer die langsame Strecke notwendig ist
  // TODO: Funktion in der HAL fuer is_upside_down implementieren m_token->set_is_upside_down(*** HAL -> get_is_upside_down() ***)
  new (this) b1_height_measurement(this->m_token);
}

// b1_height_measurement
b1_height_measurement::b1_height_measurement(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_height_measurement()" << std::endl; //FIXME
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_token->set_height1(hal->get_height_value());


  if (m_token->get_height1() <= TOO_SMALL &&  m_token->get_height1() >= HOLE_BOTTOM_UP) { //FIXME: Hier korrekte Hoehe feststellen!
    new (this) b1_valid_height(this->m_token);
  } else {
    hal->close_switch();
    new (this) b1_token_too_small(this->m_token);
  }
}

b1_height_measurement::~b1_height_measurement() { }

// b1_token_too_small
b1_token_too_small::b1_token_too_small(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_token_too_small()" << std::endl; //FIXME
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_SLIDE);

  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_FAST);
}

b1_token_too_small::~b1_token_too_small() { }

void b1_token_too_small::dispatched_event_sensor_slide() {
  new (this) anonymous_token(this->m_token);
}

// b1_valid_height
b1_valid_height::b1_valid_height(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_valid_height()" << std::endl; //FIXME
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_SWITCH);

  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_FAST);
}

b1_valid_height::~b1_valid_height() { }

void b1_valid_height::dispatched_event_sensor_switch() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->open_switch();
  m_token->set_is_metal(hal->obj_has_metal());
  new (this) b1_metal_detection(this->m_token);
}

// b1_metal_detection
b1_metal_detection::b1_metal_detection(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_metal_detection()" << std::endl; //FIXME
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_EXIT);
}

b1_metal_detection::~b1_metal_detection() { }

void b1_metal_detection::dispatched_event_sensor_exit() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_STOP);
  hal->close_switch();
  new (this) b1_exit(this->m_token);
}

// b1_exit
b1_exit::b1_exit(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_exit()" << std::endl; //FIXME
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));

  if (m_token->get_is_upside_down()) { //FIXME upside_down abruf einbauen aus der hal
    hal->set_motor(MOTOR_STOP);
    // TODO: Hier in Fehlerbehandlung springen
    new (this) b1_token_upside_down(this->m_token);
  } else {
    if (1/* TODO: Abfrage, ob Band 2 frei ist */) {
//      hal->set_motor(MOTOR_RIGHT);
      new (this) b1_token_ready_for_b2(this->m_token);
    }
  }
}

b1_exit::~b1_exit() { }

// b1_token_upside_down !!!!!!!!!! TODO: Wird ausgelagert in eigenen Fehlerzustand !!!!!!!!!!!
b1_token_upside_down::b1_token_upside_down(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_token_upside_down()" << std::endl;
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_BUTTON_START);
}

b1_token_upside_down::~b1_token_upside_down() { }

void b1_token_upside_down::dispatched_event_button_start() {
  // TODO: Wenn Wendevorgang erfolgreich -> b1_token_ready_for_b2
}

// b1_token_ready_for_b2
b1_token_ready_for_b2::b1_token_ready_for_b2(token* t) : state::state(t) {
  std::cout << "Konstruktor von b1_token_ready_for_b2()" << std::endl;

  serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  // TODO:
  // 1. Define fuer Band 2 abfragen
  // 2. Band 2 muss leer sein (Queue von Band 2 mit Groesse 1 muss leer sein / anonymous_token)
  // 3. Wenn alles OK, Motor starten: hal->set_motor(MOTOR_RIGHT);

  // Telegramm fuer den Versand erstellen
  telegram tg;
  tg.m_type = DATA;                      // Daten sollen verschickt werden
  tg.m_id = m_token->get_id();           // ID mitschicken
  tg.m_height1 = m_token->get_height1(); // Hoehe 1 mitschicken

  serial->send_telegram(&tg); // Fertiges Telegramm an Band 2 schicken

//   TODO: Ausgabe der Puck Daten auf dem Terminal

//   XXX: Pruefen, ob der Puck auf Band 1 anonymisiert werden muss
//  new (this) b2_receive_data(this->m_token);

  std::cout << "b1_token_ready_for_b2() vor set_motor(right)" << std::endl;
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_FAST);
  hal->set_motor(MOTOR_RIGHT);
  hal->set_light(RED,1);
  std::cout << "b1_token_ready_for_b2() MOTOR_FAST wurde gesetzt" << std::endl;
  new (this) anonymous_token(this->m_token);
}

b1_token_ready_for_b2::~b1_token_ready_for_b2() { }

#endif

/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/
#ifdef IS_CONVEYOR_2
// b2_receive_data
b2_receive_data::b2_receive_data(token* t) : state::state(t) {
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SERIAL_DATA);
}

b2_receive_data::~b2_receive_data() { }

void b2_receive_data::dispatched_event_serial_data() {
  serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));

  // Werte aus Telegramm holen
  telegram tg = serial->get_telegram();
  this->m_token->set_id(tg.m_id);
  this->m_token->set_height1(tg.m_height1);

  new (this) b2_received_object(this->m_token);
}

// b2_received_object
b2_received_object::b2_received_object(token* t) : state::state(t) {
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
}

b2_received_object::~b2_received_object() { }

void b2_received_object::dispatched_event_sensor_entrance() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_FAST);
  new (this) b2_realized_object(this->m_token);
}

// b2_realized_object
b2_realized_object::b2_realized_object(token* t) : state::state(t) {
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_HEIGHT);
}

b2_realized_object::~b2_realized_object() { }

void b2_realized_object::dispatched_event_sensor_height() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_SLOW);
  // TODO: Pruefen, ob Timer fuer die langsame Strecke notwendig ist
  // 1. Define fuer Band 2 abfragen
  // m_token->set_is_upside_down(hal->get_is_upside_down()); // FIXME
  new (this) b2_height_measurement(this->m_token);
}

// b2_height_measurement
b2_height_measurement::b2_height_measurement(token* t) : state::state(t) {
  // TODO: Define fuer Band 2 abfragen
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  hal->set_motor(MOTOR_FAST);

  if (1 /* hal->obj_is_upside_down() */) {
    hal->close_switch();
    new (this) b2_token_upside_down(this->m_token);
  } else {
    m_token->set_height2(hal->get_height_value());
    new (this) b2_valid_height(this->m_token);
  }
}

b2_height_measurement::~b2_height_measurement() { }

// b2_token_upside_down
b2_token_upside_down::b2_token_upside_down(token* t) : state::state(t) {
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_SLIDE);
}

b2_token_upside_down::~b2_token_upside_down() { }

void b2_token_upside_down::dispatched_event_sensor_slide() {
  new (this) anonymous_token(this->m_token);
}

// b2_valid_height
b2_valid_height::b2_valid_height(token* t) : state::state(t) {
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_SWITCH);
}

b2_valid_height::~b2_valid_height() { }

void b2_valid_height::dispatched_event_sensor_switch() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_token->set_is_metal(hal->obj_has_metal());
  new (this) b2_metal_detection(this->m_token);
}

// b2_metal_detection
b2_metal_detection::b2_metal_detection(token* t) : state::state(t) {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));

  if (1 /* wrong_order */) { // FIXME
    hal->set_motor(MOTOR_LEFT);
    new (this) b2_is_wrong_order(this->m_token);
  } else {
    hal->open_switch();
    new (this) b2_is_correct_order(this->m_token);
  }
}

b2_metal_detection::~b2_metal_detection() { }

// b2_is_wrong_order
b2_is_wrong_order::b2_is_wrong_order(token* t) : state::state(t) {
  // Beginne mit Lauschen auf geeignete Events
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(this->m_token, EVENT_SENSOR_ENTRANCE);
}

b2_is_wrong_order::~b2_is_wrong_order() { }

void b2_is_wrong_order::dispatched_event_sensor_entrance() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  // TODO 1. Define von Band 2 abfragen
  hal->set_motor(MOTOR_STOP);// 2. Motor von Band 2 anhalten
  // TODO: Entfernungs-Vorgang einleiten und dort Puck zu anonymous_token machen
  // new (this) err_remove_token(this->m_token);
}

// b2_is_correct_order
b2_is_correct_order::b2_is_correct_order(token* t) : state::state(t) {
    // Beginne mit Lauschen auf geeignete Events
    dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(this->m_token, EVENT_SENSOR_EXIT);
  }

b2_is_correct_order::~b2_is_correct_order() { }

void b2_is_correct_order::dispatched_event_sensor_exit() {
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  // TODO: 1. Define fuer Band 2 abfragen
  hal->set_motor(MOTOR_STOP);
  hal->close_switch();
  // new (this) token_finished(this->m_token); // Entfernungs-Vorgang einleiten und Puck zu anonymous_token machen
  std::cout << "TOKEN ID = " << m_token->get_id() << std::endl; //FIXME
  std::cout << "HOEHE 1 = " << m_token->get_height1() << std::endl; //FIXME
  std::cout << "HOEHE 2 = " << m_token->get_height2()  << std::endl; //FIXME

}
#endif

/******************************************************************************
 *                                ERROR FSM                                   *
 ******************************************************************************/

// class err_b1_...
