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
 * @file    fsm2.cpp
 * @version 0.2
 *
 * Klasse fuer Zustandsautomaten (FSM2)
 **/

#include "config.h"

#include "lib/fsm/state.hpp"
#include "lib/constants.hpp"
#include "lib/util/logging.hpp"
#include "lib/util/light_mgr.hpp"
#include "lib/hal/HWaccess.hpp"
#include "lib/fsm/error_states.hpp"

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::serial_bus;
using namespace se2::timer;

/**
 * Werkstueck wurde noch nicht uebergeben und es wurden noch
 * keine Daten uebertragen
 **/
b2_receive_data::b2_receive_data(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(
      singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SERIAL_DATA);
#ifdef CONVEYOR_2_SINGLEMOD
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
#endif
}

#ifdef CONVEYOR_2_SINGLEMOD
/**
 * Bei diesem Zustand funktioniert falsche Reihenfolge nicht,
 * da das Werkstueck zum anfang gefahren wird, jedoch andere Tokens
 * beim Dispatcher angemeldet sind
 **/
void b2_receive_data::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_existence();
  new (this) b2_realized_object(m_token);
}
#endif

/**
 * Werkstueck wurde noch nicht uebergeben, aber es wurden Daten
 * ueber das Werkstueck empfangen
 **/
void b2_receive_data::dispatched_event_serial_data() {
  LOG_TRACE("")
  serial_channel* serial = TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN));
  m_token->set_vals_from_telegram(serial->get_telegram());
  new (this) b2_received_object(m_token);
}

/**
 * Daten ueber das Werkstueck wurden empfangen
 * (Uebergang von `b2_receive_data::dispatched_event_serial_data()`
 **/
b2_received_object::b2_received_object(token* t) : state::state(t) {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_motor(MOTOR_RIGHT);
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
  disp->register_listener(m_token, EVENT_TRANSFER_TIMEOUT);
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration dur = { TRANSFER_SEC__TIMEOUT, TRANSFER_MSEC_TIMEOUT };
  m_token->add_timer_id(hdl->register_timer(dur, EVENT_TRANSFER_TIMEOUT));
}

/**
 * Werkstueck hat es nicht in der Zeit durch die Lichtschranke geschafft,
 * Uebergabe des Werkstueckes ist gescheitert...
 **/
void b2_received_object::dispatched_event_tansfer_timeout() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck hat Band 2 erreicht (Lichtscnrake des Eingangs durchbrochen)
 **/
void b2_received_object::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_existence();
  new (this) b2_realized_object(m_token);
}

/**
 * Uebergang von `b2_received_object::dispatched_event_sensor_entrance()`
 **/
b2_realized_object::b2_realized_object(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->init_internal_times(SEGMENT_1);
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT);
  disp->register_listener(m_token, EVENT_SEG1_TOO_LATE);
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration too_late  = { SEGMENT1_SEC__TOO_LATE,
                               SEGMENT1_MSEC_TOO_LATE };
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG1_TOO_LATE));
}

/**
 * EVENT_SENSOR_HEIGHT wurde zu frueh gefeuert, es wurde wahrscheinlich
 * ein neuer Token auf das Band gelegt
 **/
void b2_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_slow_motor();
  if (m_token->check_internal_times(SEGMENT_1)) {
    new (this) b2_height_measurement(m_token);
  } else {
    new (this) err_runtime_too_short(m_token);
  }
}

/**
 * Token ist zu spaet und wurde wahrscheinlich vom Band entfernt
 * Betreffend Segment 1
 **/
void b2_realized_object::dispatched_event_seg1_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Uebergang von `b2_realized_object::dispatched_event_sensor_height()`
 **/
b2_height_measurement::b2_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int height = hal->get_height_value();
  m_token->set_height2(height);
  if (TOO_SMALL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
      && height <= TOO_SMALL_HI + HEIGHT_SENSOR_TOLERANCE_MAX) {
    // darf nicht passieren
    // TODO: Eigenen Fehlerzustand ?
    new (this) err_unexpected_token(m_token);
  } else if ((HOLE_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
     || (METAL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    m_token->set_is_upside_down(false);
  } else if ((HOLE_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
          || (METAL_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    new (this) b2_token_upside_down(m_token);
    return;
  }
  new (this) b2_valid_height(m_token);
}

/**
 * Uebergang von`b2_height_measurement::b2_height_measuremet`
 **/
b2_token_upside_down::b2_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE);
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

/**
 * Werkstueck ist verkehrt herum und verlaesst gerade die Hoehenmessung
 **/
void b2_token_upside_down::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
}

/**
 * Werkstueck wird gerade aussortiert und hat die Lichtschranke der
 * Rutsche durchbrochen
 **/
void b2_token_upside_down::dispatched_event_sensor_slide() {
  LOG_TRACE("")
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SLIDE_FULL_TIMEOUT);
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE_R);
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration dur = { SLIDE_SEC__TIMEOUT, SLIDE_MSEC_TIMEOUT };
  m_token->add_timer_id(hdl->register_timer(dur, EVENT_SLIDE_FULL_TIMEOUT));
}

/**
 * Werkstueck hat die Lichtschranke der Weiche wieder freigemancht,
 * Rutsche war also nicht voll
 **/
void b2_token_upside_down::dispatched_event_sensor_slide_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

/**
 * Timeout ist angekommen, Rutsche ist voll
 **/
void b2_token_upside_down::dispatched_event_slide_full_timeout() {
  LOG_TRACE("")
  new (this) err_slide_full(m_token);
}

/**
 * Werkstueck wurde zwischen Hoehenmessung und Weiche entnommen (Segment 2)
 **/
void b2_token_upside_down::dispatched_event_seg2_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Uebergang von `b2_height_measurement::b2_height_measurement`
 **/
b2_valid_height::b2_valid_height(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

/**
 * Werkstueck hat gueltige Hoehe und hat gerade die Hoehenmessung verlassen
 **/
void b2_valid_height::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  m_token->init_internal_times(SEGMENT_2);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SEG2_TOO_LATE);
  const duration too_late = { SEGMENT2_SEC__TOO_LATE,
                              SEGMENT2_MSEC_TOO_LATE };
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG2_TOO_LATE));
}

/**
 * Werkstueck hat gueltige Hoehe und ist gerade in den Bereich der Weiche
 * gekommen, hat also die Lichtschranke der Weiche durchbrochen
 **/
void b2_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  if (m_token->check_internal_times(SEGMENT_2)) {
    new (this) b2_metal_detection(m_token);
  } else {
    new (this) err_runtime_too_short(m_token);
  }
}

/**
 * Werkstueck hat gueltige Hoehe, wurde jedoch zwischen
 * Hoehenmessung und Weiche entnommen (Segment 2)
 **/
void b2_valid_height::dispatched_event_seg2_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck befindet sich unter dem Metall-detektor
 * Uebergang von `b2_valid_height::dispatched_event_sensor_switch`
 **/
b2_metal_detection::b2_metal_detection(token* t) : state::state(t) {
  LOG_TRACE("")
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  m_token->set_is_metal(hal->obj_has_metal());
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  if (mgr->check_order(m_token->is_metal())) {
    new (this) b2_is_correct_order(m_token);
  } else {
    new (this) b2_is_wrong_order(m_token);
  }
}

/**
 * Werkstueck ist vom falschen Typen (falsche Reihenfolge)
 **/
b2_is_wrong_order::b2_is_wrong_order(token* t) : state::state(t) {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_left_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
  // Auch hier wieder sensetiv, damit kein unexpected token ausgeloest wird
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT);
}

/**
 * Werkstueck hat falsche Reihenfolge und ist wieder am Anfang des Bandes
 **/
void b2_is_wrong_order::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(REMOVE_TOKEN);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE_R);
}

/**
 * Werkstueck hat falsche Reihenfolge und wurde vom Band entnommen
 * (Am Anfang des Bandes)
 **/
void b2_is_wrong_order::dispatched_event_sensor_entrance_rising() {
  // Token wurde vom Band entfernt
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_left_motor(false);
  mgr->unrequest_stop_motor(false);
  mgr->notify_death();
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  new (this) anonymous_token(m_token);
}

/**
 * Werkstueck hat korrekte Reihenfolge
 * (Uebergang von `b2_metal_detection::b2_metal_detection`)
 **/
b2_is_correct_order::b2_is_correct_order(token* t) : state::state(t) {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_open_switch();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
}

/**
 * Werkstueck hat korrekte Reihenfolge und hat gerade die Weiche verlassen
 **/
void b2_is_correct_order::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("")
  m_token->init_internal_times(SEGMENT_3);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SEG3_TOO_LATE);
  disp->register_listener(m_token, EVENT_CLOSE_SWITCH_TIME);
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration too_late  = { SEGMENT3_SEC__TOO_LATE,
                               SEGMENT3_MSEC_TOO_LATE };
  const duration switch_timeout = { CLOSE_SWITCH_TIME__SEC,
                                    CLOSE_SWITCH_TIME_MSEC };
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG3_TOO_LATE));
  m_token->add_timer_id(hdl->register_timer(switch_timeout,
      EVENT_CLOSE_SWITCH_TIME));
}

void b2_is_correct_order::dispatched_event_close_switch_time() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_open_switch();
}

/**
 * Werkstueck hat korrekte Reihenfolge und hat gerade die End Lichtschranke
 * durchbrochen
 **/
void b2_is_correct_order::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(REMOVE_TOKEN);
  if (!m_token->check_internal_times(SEGMENT_3)) {
    new (this) err_runtime_too_short(m_token);
  }
  m_token->pretty_print();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_REMOVE_TOKEN_TIMEOUT);
  // Timer fuer timeout zum entnehmen des Werkstueckes starten
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration dur = { LIFT_UP_SEC__TIMEOUT, LIFT_UP_MSEC_TIMEOUT };
  m_token->add_timer_id(hdl->register_timer(dur, EVENT_REMOVE_TOKEN_TIMEOUT));
}

void b2_is_correct_order::dispatched_event_remove_token_timeout() {
  LOG_TRACE("")
  new (this) err_token_not_removed_from_end(m_token);
}

/**
 * Werkstueck hat korrekte Reihenfolge und wurde vom Ende des Bandes entnommen
 **/
void b2_is_correct_order::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor(false);
  mgr->notify_death();
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  new (this) anonymous_token(m_token);
}

/**
 * Werkstueck hat korrekte Reihenfolge, wurde aber von Segment 3 entnommen
 **/
void b2_is_correct_order::dispatched_event_seg3_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}
