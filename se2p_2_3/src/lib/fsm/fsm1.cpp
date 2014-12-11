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
 * @version 0.2
 *
 * Klasse fuer Zustandsautomaten
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

anonymous_token::anonymous_token(token* t) : state::state(t) {
  LOG_TRACE("anonymous_token::anonymous_token")
  m_token->reset();
#if defined(IS_CONVEYOR_1)
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
#elif defined(IS_CONVEYOR_2)
  new (this) b2_receive_data(m_token);
#endif
}

void anonymous_token::dispatched_event_sensor_entrance() {
  LOG_TRACE("anonymous_token::dispatched_event_sensor_entrance")
#if defined(IS_CONVEYOR_1)
  new (this) b1_realized_object(m_token);
#endif
}

/**
 * Werkstueck hat Eingangslichtschranke durchbrochen
 **/
b1_realized_object::b1_realized_object(token* t) : state::state(t) {
  LOG_TRACE("b1_realized_object::b1_realized_object")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(
      DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT);
  disp->register_listener(m_token, EVENT_SEG1_HAS_TO_EXPIRE);
  disp->register_listener(m_token, EVENT_SEG1_TOO_LATE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_existence();
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration to_expire = { SEGMENT1_SEC__HAS_TO_EXPIRE,
                               SEGMENT1_MSEC_HAS_TO_EXPIRE };
  const duration too_late  = { SEGMENT1_SEC__TOO_LATE,
                               SEGMENT1_MSEC_TOO_LATE };
  m_token->add_timer_id(hdl->register_timer(to_expire, EVENT_SEG1_HAS_TO_EXPIRE));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG1_TOO_LATE));
}

/**
 * EVENT_SEG1_HAS_TO_EXPIRE timer ist abgelaufen. Nun darf
 * der Token auch die Lichtschranke durchbrechen
 **/
void b1_realized_object::dispatched_event_seg1_has_to_expire() {
  LOG_TRACE("b1_realized_object::dispatched_event_seg1_has_to_expire");
  new (this) b1_realized_object_seg1_ok(m_token);
}

/**
 * EVENT_SENSOR_HEIGHT wurde zu frueh gefeuert, es wurde wahrscheinlich
 * ein neuer Token wild auf das Band gelegt.
 **/
void b1_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("b1_realized_object::dispatched_event_sensor_height");
  new (this) err_runtime_too_short(m_token);
}

/**
 * Erste Timer von Segment 1 ist abgelaufen.
 **/
b1_realized_object_seg1_ok::b1_realized_object_seg1_ok(token* t)
    : state::state(t) {
  LOG_TRACE("b1_realized_object_seg1_ok::b1_realized_object_seg1_ok")
}

/**
 * Good Case. Token ist zwischen dem Too late und dem Too early
 * Token in die Lichtschranke geraten
 * Betreffend Segment 1 (Hoehenmessung)
 **/
void b1_realized_object_seg1_ok::dispatched_event_sensor_height() {
  LOG_TRACE("b1_realized_object_seg1_ok::dispatched_event_sensor_height")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_slow_motor();
  new (this) b1_height_measurement(m_token);
}

/**
 * Token ist zu spaet und wurde wahrscheinlich vom Band entfernt.
 * Betreffend Segment 1 (Hoehenmessung)
 **/
void b1_realized_object_seg1_ok::dispatched_event_seg1_too_late() {
  LOG_TRACE("b1_realized_object_seg1_ok::dispatched_event_seg1_too_late");
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck ist im Bereich der Hoehenmessung
 * Moegliche Folgezustaende:
 * - b1_token_too_small
 * - b1_valid_height
 * TODO:
 * - Braucht dieser Zustand evtl. auch einen Handler fuer
 *   `dispatched_event_sensor_height_rising()` ? Grund:
 *   In seltenen Faellen bleibt der Motor auf Slow, ist der Grund
 *   das der Zustand noch nicht gewechselt wurde, aber die Hoehenmessung
 *   bereits verlassen wurde?
 **/
b1_height_measurement::b1_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("b1_height_measurement::b1_height_measurement")
  dispatcher* disp =
    TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT_R);
  hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  int height = hal->get_height_value();
  m_token->set_height1(height);
  if (TOO_SMALL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
      && height <= TOO_SMALL_HI + HEIGHT_SENSOR_TOLERANCE_MAX) {
    new (this) b1_token_too_small(m_token);
    return;
  } else if ((HOLE_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
     || (METAL_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    m_token->set_is_upside_down(false);
  } else if ((HOLE_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= HOLE_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)
          || (METAL_BOTTOM_UP_LOW - HEIGHT_SENSOR_TOLERANCE_MIN <= height
         && height <= METAL_BOTTOM_UP_HI + HEIGHT_SENSOR_TOLERANCE_MAX)) {
    m_token->set_is_upside_down(true);
  }
  new (this) b1_valid_height(m_token);
}

/**
 * Werkstueck ist zu klein und muss aussortiert werden.
 * Befindet sich aber noch in der Hoehenmessung
 **/
b1_token_too_small::b1_token_too_small(token* t) : state::state(t) {
  LOG_TRACE("b1_token_too_small::b1_token_too_small")
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE);
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

/**
 * Werkstueck hat die Hoehenmessung verlassen.
 * TODO:
 * - Ab hier einen Timer starten, der bis zur Lichtschranke
 *   der Weiche gilt.
 **/
void b1_token_too_small::dispatched_event_sensor_height_rising() {
  LOG_TRACE("b1_token_too_small::dispatched_event_sensor_height_rising")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SEG2_HAS_TO_EXPIRE);
  disp->register_listener(m_token, EVENT_SEG2_TOO_LATE);
  const duration to_expire = { SEGMENT2_SEC__HAS_TO_EXPIRE,
                              SEGMENT2_MSEC_HAS_TO_EXPIRE };
  const duration too_late = { SEGMENT2_SEC__TOO_LATE + 1,
                              SEGMENT2_MSEC_TOO_LATE };
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_token->add_timer_id(hdl->register_timer(to_expire, EVENT_SEG2_HAS_TO_EXPIRE));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG2_TOO_LATE));
}

/**
 * Werkstueck ist nicht zu frueh in den Sensor der Rutsche gekommen
 **/
void b1_token_too_small::dispatched_event_seg2_has_to_expire() {
  LOG_TRACE("b1_token_too_small::dispatched_event_seg2_has_to_expire")
  new (this) b1_token_too_small_seg2_ok(m_token);
}

/**
 * Werkstueck hat zu frueh die Lichtschranke der Weiche durchbrochen.
 **/
void b1_token_too_small::dispatched_event_sensor_switch() {
  LOG_TRACE("b1_token_too_small::dispatched_event_sensor_switch")
  new (this) err_runtime_too_short(m_token);
}

/**
 * Werkstueck ist zu klein und hat gerade ide Lichtschranke der
 * Rutsche durchbrochen.
 * TODO:
 * - Neuen Zustand der den Timeout der Lichtschranke beschreibt
 *   erstellen
 * - Wenn der Timer abgelaufen ist im Uebergang pruefen ob
 *   die Lichtschranke noch durchbrochen ist, wenn diese noch
 *   durchbrochen, dann in Fehlerbehandlung springen
 * - Token erst nach ablauf des Timers loeschen, bzw. erst nach
 *   erfolgreicher Fehlerbehandlung
 **/
void b1_token_too_small::dispatched_event_sensor_slide() {
  LOG_TRACE("b1_token_too_small::dispatched_event_sensor_slide")
  new (this) err_runtime_too_short(m_token);
}

/**
 * Uebergang von `b1_token_too_small::dispatched_event_seg2_has_to_expire()`
 * Werkstueck kam schonmal nicht zu spaet.
 **/
b1_token_too_small_seg2_ok::b1_token_too_small_seg2_ok(token* t)
    : state::state(t) {
  LOG_TRACE("b1_token_too_small_seg2_ok::b1_token_too_small_seg2_ok")
  m_token->m_seg2_ok = true;
}

/**
 * Good Case, Werkstueck war nicht zu spaet und nicht zu frueh.
 * Werkstueck muss jetzt die Rutsche runterrtuschen
 * TODO:
 * - Timer starten fuer timeout der Rutsche
 * - `dispatched_event_sensor_slide_r()` implementieren
 * - Token erst loeschen wenn Lichtschranke wieder ununterbrochen
 **/
void b1_token_too_small_seg2_ok::dispatched_event_sensor_slide() {
  LOG_TRACE("b1_token_too_small_seg2_ok::dispatched_event_sensor_slide")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

/**
 * Werkstueck hat die Lichtschranke der Rutsche nicht durchbrochen und
 * wurde (a) vom Band entfernt oder (b) haengt auf dem Band fest
 **/
void b1_token_too_small_seg2_ok::dispatched_event_seg2_too_late() {
  LOG_TRACE("b1_token_too_small_seg2_ok::dispatched_event_seg2_too_late")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck hat korrekte Hoehe, ist aber noch in der Hoehenmessung
 **/
b1_valid_height::b1_valid_height(token* t) : state::state(t) {
  LOG_TRACE("b1_valid_height::b1_valid_height")
}

/**
 * Werkstueck wurde von der Weiche entfernt (Segment 2)
 **/
void b1_valid_height::dispatched_event_seg2_too_late() {
  LOG_TRACE("b1_token_too_small_seg2_ok::dispatched_event_seg2_too_late")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck hat die korrekte Hoehe
 * TODO:
 * - Ab hier einen Timer starten, der bis zur Lichtschranke
 *   der Weiche gilt.
 **/
void b1_valid_height::dispatched_event_sensor_height_rising() {
  LOG_TRACE("b1_valid_height::dispatched_event_sensor_height_rising")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
  disp->register_listener(m_token, EVENT_SEG2_HAS_TO_EXPIRE);
  disp->register_listener(m_token, EVENT_SEG2_TOO_LATE);
  const duration to_expire = { SEGMENT2_SEC__HAS_TO_EXPIRE,
                              SEGMENT2_MSEC_HAS_TO_EXPIRE };
  const duration too_late = { SEGMENT2_SEC__TOO_LATE,
                              SEGMENT2_MSEC_TOO_LATE };
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_token->add_timer_id(hdl->register_timer(to_expire, EVENT_SEG2_HAS_TO_EXPIRE));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG2_TOO_LATE));
}

/**
 * Der erste Timer ist abgelaufen, Werkstueck nicht zu frueh.
 **/
void b1_valid_height::dispatched_event_seg2_has_to_expire() {
  LOG_TRACE("b1_valid_height::dispatched_event_seg2_has_to_expire")
  new (this) b1_valid_height_seg2_ok(m_token);
}

/**
 * Uebergang von `b1_valid_height`.
 **/
b1_valid_height_seg2_ok::b1_valid_height_seg2_ok(token* t) : state::state(t) {
  LOG_TRACE("b1_valid_height_seg2_ok::b1_valid_height_seg2_ok")
  m_token->m_seg2_ok = true;
}

/**
 * Good Case, Werkstueck ist nicht zu spaet im Weichen Sensor
 **/
void b1_valid_height_seg2_ok::dispatched_event_sensor_switch() {
  LOG_TRACE("b1_valid_height_seg2_ok::dispatched_event_sensor_switch")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_open_switch();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
}

/**
 * Werkstueck hat den Sensor der Weiche verlassen
 **/
void b1_valid_height_seg2_ok::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("b1_valid_height::dispatched_event_sensor_switch_rising")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_open_switch();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
  disp->register_listener(m_token, EVENT_SEG3_HAS_TO_EXPIRE);
  disp->register_listener(m_token, EVENT_SEG3_TOO_LATE);
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration to_expire = { SEGMENT3_SEC__HAS_TO_EXPIRE,
                               SEGMENT3_MSEC_HAS_TO_EXPIRE };
  const duration too_late  = { SEGMENT3_SEC__TOO_LATE,
                               SEGMENT3_MSEC_TOO_LATE };
  m_token->add_timer_id(hdl->register_timer(to_expire, EVENT_SEG3_HAS_TO_EXPIRE));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG3_TOO_LATE));
}

/**
 * Werkstueck hat die Lichtschranke der Weiche zu frueh durchbrochen.
 * In Fehlerbehandlung springen.
 **/
void b1_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("b1_valid_height::dispatched_event_sensor_switch")
  new (this) err_runtime_too_short(m_token);
}

/**
 * Werkstueck hat Lichtschranke der Weiche verlassen.
 * TODO:
 * - Evtl einen Timer erstellen der `unrequest_open_switch()` ausloest,
 *   auf manchen Baendern bleibt das Werkstueck in der Weiche haengen da
 *   entweder der Motor zu schwach ist, oder die Weiche zu stark.
 **/
void b1_valid_height::dispatched_event_sensor_switch_rising() {
  // TODO: In fehler.. Zu frueh oder zu spaet?!
}

/**
 * Werkstueck hat das Ende des Bandes erreicht.
 **/
void b1_valid_height::dispatched_event_sensor_exit() {
  LOG_TRACE("b1_valid_height::dispatched_event_sensor_exit")
  new (this) err_runtime_too_short(m_token);
}

/**
 * Werkstueck kam nicht zu frueh an.
 **/
void b1_valid_height_seg2_ok::dispatched_event_seg3_has_to_expire() {
  LOG_TRACE("b1_valid_height_seg2_ok::dispatched_event_seg3_has_to_expire")
  new (this) b1_valid_height_seg3_ok(m_token);
}

/**
 * Werkstueck sollte nun im letzten (3ten) Segment sein
 **/
b1_valid_height_seg3_ok::b1_valid_height_seg3_ok(token* t) : state::state(t) {
  LOG_TRACE("b1_valid_height_seg3_ok::b1_valid_height_seg3_ok")
}

/**
 * Token zu spaet, in Fehlerbehandlung springen...
 **/
void b1_valid_height_seg3_ok::dispatched_event_seg2_too_late() {
  LOG_TRACE("b1_valid_height_seg3_ok::dispatched_event_seg2_too_late")
  if (!m_token->m_seg2_ok) {
    new (this) err_runtime_too_long(m_token);
  }
}

/**
 * Werkstueck ist zur korrekten Zeit in die Exit-Lichtschranke gekommen.
 **/
void b1_valid_height_seg3_ok::dispatched_event_sensor_exit() {
  LOG_TRACE("b1_valid_height_seg3_ok::dispatched_event_sensor_exit")
  new (this) b1_exit(m_token);
}

/**
 * Werkstueck ist zu spaet. Wurde (a) vom Band entnommen, oder (b)
 * haengt auf dem Band fest.
 **/
void b1_valid_height_seg3_ok::dispatched_event_seg3_too_late() {
  LOG_TRACE("b1_valid_height_seg3_ok::dispatched_event_seg3_too_late")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck noch immer am Ende des Bandes
 * (Uebergang von `b1_valid_height::dispatched_event_sensor_exit()`)
 **/
b1_exit::b1_exit(token* t) : state::state(t) {
  LOG_TRACE("b1_exit::b1_exit")
  if (m_token->is_upside_down()) {
    new (this) b1_token_upside_down(m_token);
  } else {
    new (this) b1_token_ready_for_b2(m_token);
  }
}

/**
 * Werkstueck noch immer am Ende des Bandes
 * (Uebergang von `b1_exit::b1_exit()`)
 **/
b1_token_upside_down::b1_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("b1_token_upside_down::b1_token_upside_down")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(TURN_TOKEN);
  token_mgr* mgr  = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
  disp->register_prior_listener(m_token, EVENT_SENSOR_EXIT);
  disp->register_prior_listener(m_token, EVENT_SENSOR_EXIT_R);
}

/**
 * Werkstueck wurde bestaetigt und ist bereit fuer Uebergane
 * nach Band 2
 **/
void b1_token_upside_down::dispatched_event_button_start() {
  LOG_TRACE("b1_token_upside_down::dispatched_event_button_start")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(READY_TO_USE);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->unregister_prior_listener(m_token, EVENT_SENSOR_EXIT);
  disp->unregister_prior_listener(m_token, EVENT_SENSOR_EXIT_R);
  new (this) b1_token_ready_for_b2(m_token);
}

/**
 * Werkstueck ist bereit fuer Band 2.
 **/
b1_token_ready_for_b2::b1_token_ready_for_b2(token* t) : state::state(t) {
  LOG_TRACE("b1_token_ready_for_b2::b1_token_ready_for_b2")
  m_token->pretty_print();
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SERIAL_TRANSFER_FIN);
  if (!mgr->check_conveyor2_ready()) {
    mgr->request_stop_motor();
    dispatcher* disp =
        TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
    disp->register_listener(m_token, EVENT_SERIAL_NEXT_OK);
  } else {
    telegram tg(m_token);
    TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tg);
    mgr->notify_token_trasition();
  }
}

/**
 * Band 2 ist wieder frei und kann ein weiteres Werkstueck bearbeiten.
 **/
void b1_token_ready_for_b2::dispatched_event_serial_next_ok() {
  LOG_TRACE("b1_token_ready_for_b2::dispatched_event_serial_next_ok")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  telegram tg(m_token);
  TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tg);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
}

/**
 * Werkstueck hat Band 1 verlassen.
 * TODO:
 * - Hier Timer starten, falls das Werkstueck zwischen Band 1 und
 *   Band 2 haengen bleibt.
 * - Evtl Timer starten der erkennt ob ein Werkstueck dazwischen gelegt wurde?
 **/
void b1_token_ready_for_b2::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("b1_token_ready_for_b2::dispatched_event_sensor_exit_rising")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_token_trasition();
}

/**
 * Werkstueck wurde erfolgreich an Band 2 uebergeben
 **/
void b1_token_ready_for_b2::dispatched_event_serial_transfer_fin() {
  LOG_TRACE("b1_token_ready_for_b2::dispatched_event_serial_transfer_fin")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}
