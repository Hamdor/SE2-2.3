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

#include <time.h>

using namespace se2;
using namespace se2::fsm;
using namespace se2::hal;
using namespace se2::util;
using namespace se2::dispatch;
using namespace se2::serial_bus;
using namespace se2::timer;

anonymous_token::anonymous_token(token* t) : state::state(t) {
  LOG_TRACE("")
  m_token->reset();
#if defined(IS_CONVEYOR_1)
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_ENTRANCE);
#elif defined(IS_CONVEYOR_2)
  new (this) b2_receive_data(m_token);
#endif
}

void anonymous_token::dispatched_event_sensor_entrance() {
  LOG_TRACE("")
#if defined(IS_CONVEYOR_1)
  new (this) b1_realized_object(m_token);
#endif
}

/**
 * Werkstueck hat Eingangslichtschranke durchbrochen
 **/
b1_realized_object::b1_realized_object(token* t) : state::state(t) {
  LOG_TRACE("")
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(
      DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_HEIGHT);
  disp->register_listener(m_token, EVENT_SEG1_TOO_LATE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_existence();
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration too_late  = { SEGMENT1_SEC__TOO_LATE,
                               SEGMENT1_MSEC_TOO_LATE };
  size_t idx = hdl->register_timer(too_late,  EVENT_SEG1_TOO_LATE);
  m_token->add_timer_id(idx);
  m_token->init_internal_times(SEGMENT_1);
  if (mgr->is_motor_slow()) {
    // Motor laeuft langsam, Zeiten anpassen
    const timespec addspec = mgr->get_motor_slow_diff();
    m_token->add_internal_times(addspec.tv_sec, addspec.tv_nsec);
    const duration dur = { addspec.tv_sec, (size_t)addspec.tv_nsec / MILISEC_TO_NANOSEC };
    hdl->add_time(idx, dur);
  }
  if (mgr->is_motor_stopped()) {
    // motor ist gestoppt, timer pausieren...
    hdl->pause_timer(idx);
  }
}

/**
 * Good Case. Token ist zwischen dem Too late und dem Too early
 * Token in die Lichtschranke geraten
 * Betreffend Segment 1
 **/
void b1_realized_object::dispatched_event_sensor_height() {
  LOG_TRACE("")
  m_token->delete_timers();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->force_pop(m_token, EVENT_SEG2_TOO_LATE);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_slow_motor();
  if (m_token->check_internal_times(SEGMENT_1)) {
    new (this) b1_height_measurement(m_token);
  } else {
    new (this) err_runtime_too_short(m_token);
  }
}

/**
 * Laufzeit im Segment 1 war zu lang
 **/
void b1_realized_object::dispatched_event_seg1_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck ist im Bereich der Hoehenmessung
 * Moegliche Folgezustaende:
 * - b1_token_too_small
 * - b1_valid_height
 **/
b1_height_measurement::b1_height_measurement(token* t) : state::state(t) {
  LOG_TRACE("")
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
  LOG_TRACE("")
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SLIDE);
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
}

/**
 * Werkstueck hat die Hoehenmessung verlassen.
 **/
#define SLIDE_OFFSET 2
void b1_token_too_small::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  m_token->init_internal_times(SEGMENT_2);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SEG2_TOO_LATE);
  m_token->delete_timers();
  const duration too_late = { SEGMENT2_SEC__TOO_LATE + SLIDE_OFFSET,
                              SEGMENT2_MSEC_TOO_LATE };
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG2_TOO_LATE));
}

/**
 * Werkstueck hat zu frueh die Lichtschranke der Weiche durchbrochen
 **/
void b1_token_too_small::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  m_token->delete_timers();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->force_pop(m_token, EVENT_SEG2_TOO_LATE);
  if (!m_token->check_internal_times(SEGMENT_2)) {
    new (this) err_runtime_too_short(m_token);
  }
}

/**
 * Werkstueck ist zu klein und hat gerade die Lichtschranke der
 * Rutsche durchbrochen
 **/
void b1_token_too_small::dispatched_event_sensor_slide() {
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
 * Werkstueck hat Rutschenbereich wieder verlassen
 **/
void b1_token_too_small::dispatched_event_sensor_slide_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}

/**
 * Rutsche ist voll
 **/
void b1_token_too_small::dispatched_event_slide_full_timeout() {
  LOG_TRACE("")
  new (this) err_slide_full(m_token);
}

/**
 * Werkstueck hat die Lichtschranke der Rutsche nicht durchbrochen und
 * wurde (a) vom Band entfernt oder (b) haengt auf dem Band fest
 **/
void b1_token_too_small::dispatched_event_seg2_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck hat korrekte Hoehe, ist aber noch in der Hoehenmessung
 **/
b1_valid_height::b1_valid_height(token* t) : state::state(t) {
  LOG_TRACE("")
}

/**
 * Werkstueck hat die korrekte Hoehe
 **/
void b1_valid_height::dispatched_event_sensor_height_rising() {
  LOG_TRACE("")
  m_token->init_internal_times(SEGMENT_2);
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_fast_motor();
  dispatcher* disp = TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH);
  disp->register_listener(m_token, EVENT_SEG2_TOO_LATE);
  const duration too_late = { SEGMENT2_SEC__TOO_LATE,
                              SEGMENT2_MSEC_TOO_LATE };
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  m_token->add_timer_id(hdl->register_timer(too_late,  EVENT_SEG2_TOO_LATE));
}

/**
 * Good Case, Werkstueck ist nicht zu spaet im Weichensensor
 **/
void b1_valid_height::dispatched_event_sensor_switch() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_open_switch();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_SENSOR_SWITCH_R);
}

/**
 * Werkstueck hat den Sensor der Weiche verlassen
 **/
void b1_valid_height::dispatched_event_sensor_switch_rising() {
  LOG_TRACE("")
  m_token->delete_timers();
  if (!m_token->check_internal_times(SEGMENT_2)) {
    new (this) err_runtime_too_short(m_token);
    return;
  }
  m_token->init_internal_times(SEGMENT_3);
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->force_pop(m_token, EVENT_SEG2_TOO_LATE);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT);
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

void b1_valid_height::dispatched_event_close_switch_time() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_open_switch();
}

/**
 * Werkstueck hat das Ende des Bandes erreicht
 **/
void b1_valid_height::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  m_token->delete_timers();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->force_pop(m_token, EVENT_SEG3_TOO_LATE);
  if (m_token->check_internal_times(SEGMENT_3)) {
    new (this) b1_exit(m_token);
  } else {
    new (this) err_runtime_too_short(m_token);
  }
}

void b1_valid_height::dispatched_event_seg2_too_late() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

void b1_valid_height::dispatched_event_seg3_too_late() {
  LOG_TRACE("")
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
    TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, true);
  } else {
    new (this) b1_token_ready_for_b2(m_token);
  }
}

/**
 * Werkstueck noch immer am Ende des Bandes
 * (Uebergang von `b1_exit::b1_exit()`)
 **/
b1_token_upside_down::b1_token_upside_down(token* t) : state::state(t) {
  LOG_TRACE("")
  light_mgr* lmgr = TO_LIGHT(singleton_mgr::get_instance(LIGHT_PLUGIN));
  lmgr->set_state(TURN_TOKEN);
  token_mgr* mgr  = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->request_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_BUTTON_START);
  disp->register_prior_listener(m_token, EVENT_SENSOR_EXIT);
  disp->register_prior_listener(m_token, EVENT_SENSOR_EXIT_R);
  disp->register_listener(m_token, EVENT_REMOVE_TOKEN_TIMEOUT);
  // Timer fuer timeout zum entnehmen des Werkstueckes starten
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration dur = { TURNOVER_SEC__TIMEOUT, TURNOVER_MSEC_TIMEOUT };
  m_token->add_timer_id(hdl->register_timer(dur, EVENT_REMOVE_TOKEN_TIMEOUT));
}

/**
 * Werkstueck wurde bestaetigt und ist bereit fuer Uebergang
 * nach Band 2
 **/
void b1_token_upside_down::dispatched_event_button_start() {
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, false);
  LOG_TRACE("")
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
 * Token wurde hochgehoben und muss nun wieder auf das Band gelegt werden
 **/
void b1_token_upside_down::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
  new (this) b1_token_upside_down_lift_up(m_token);
}

/**
 * Werkstueck wurde nicht entfernt in der vorgegebenen Zeit vom
 * Band gehoben
 **/
void b1_token_upside_down::dispatched_event_remove_token_timeout() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START,false);
  m_token->delete_timers();
  new (this) err_token_not_removed_from_end(m_token);
}

/**
 * Werkstueck wurde vom Band entnommen (Uebergang von
 * `b1_token_upside_down::dispatched_event_sensor_exit_rising()`
 * oder `b1_token_upside_down_put_back::dispatched_event_sensor_exit_rising()`
 **/
b1_token_upside_down_lift_up::b1_token_upside_down_lift_up(token* t)
    : state::state(t) {
  LOG_TRACE("")
  // Timer fuer timeout zum Entnehmen des Werkstueckes starten
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration dur = { TURNOVER_SEC__TIMEOUT, TURNOVER_MSEC_TIMEOUT };
  m_token->add_timer_id(hdl->register_timer(dur, EVENT_TURN_TOKEN_TIMEOUT));
}

/**
 * Werkstueck wurde zurueck auf das Band gelegt
 **/
void b1_token_upside_down_lift_up::dispatched_event_sensor_exit() {
  LOG_TRACE("")
  new (this) b1_token_upside_down_put_back(m_token);
}

/**
 * Werkstueck wurde entfernt und nicht in der gegebenen Zeit gedreht
 **/
void b1_token_upside_down_lift_up::dispatched_event_turn_token_timeout() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START,false);
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_RESET, true);
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck wurde zurueck auf das Band gelegt (Uebergang von
 * `b1_token_upside_down_lift_up::dispatched_event_sensor_exit()`)
 **/
b1_token_upside_down_put_back::b1_token_upside_down_put_back(token* t)
    : state::state(t) {
  LOG_TRACE("")
}

/**
 * Werkstueck liegt auf dem Band und es wurde der Start-Taster betaetigt
 **/
void b1_token_upside_down_put_back::dispatched_event_button_start() {
  LOG_TRACE("")
  TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN))->set_led_state(LED_START, false);
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
 * Werkstueck wurde wieder vom Band entfernt
 **/
void b1_token_upside_down_put_back::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
  new (this) b1_token_upside_down_lift_up(m_token);
}

/**
 * Werkstueck ist bereit fuer Band 2
 **/
b1_token_ready_for_b2::b1_token_ready_for_b2(token* t) : state::state(t) {
  LOG_TRACE("")
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
    disp->register_listener(m_token, EVENT_TRANSFER_TIMEOUT);
    timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
    const duration dur = { TRANSFER_SEC__TIMEOUT, TRANSFER_MSEC_TIMEOUT };
    m_token->add_timer_id(hdl->register_timer(dur, EVENT_TRANSFER_TIMEOUT));
  }
}

/**
 * Band 2 ist wieder frei und kann ein weiteres Werkstueck bearbeiten
 **/
void b1_token_ready_for_b2::dispatched_event_serial_next_ok() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->unrequest_stop_motor();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  telegram tg(m_token);
  TO_SERIAL(singleton_mgr::get_instance(SERIAL_PLUGIN))->send_telegram(&tg);
  disp->register_listener(m_token, EVENT_SENSOR_EXIT_R);
}

/**
 * Werkstueck hat Band 1 verlassen
 **/
void b1_token_ready_for_b2::dispatched_event_sensor_exit_rising() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_token_trasition();
  dispatcher* disp =
      TO_DISPATCHER(singleton_mgr::get_instance(DISPATCHER_PLUGIN));
  disp->register_listener(m_token, EVENT_TRANSFER_TIMEOUT);
  timer_handler* hdl = TO_TIMER(singleton_mgr::get_instance(TIMER_PLUGIN));
  const duration dur = { TRANSFER_SEC__TIMEOUT, TRANSFER_MSEC_TIMEOUT };
  m_token->add_timer_id(hdl->register_timer(dur, EVENT_TRANSFER_TIMEOUT));
}

/**
 * Wenn dieses Event getriggert wird, ist das Werkstueck zwischen
 * Band 1 und Band 2 haengengeblieben
 **/
void b1_token_ready_for_b2::dispatched_event_tansfer_timeout() {
  LOG_TRACE("")
  new (this) err_runtime_too_long(m_token);
}

/**
 * Werkstueck wurde erfolgreich an Band 2 uebergeben
 **/
void b1_token_ready_for_b2::dispatched_event_serial_transfer_fin() {
  LOG_TRACE("")
  token_mgr* mgr = TO_TOKEN_MGR(singleton_mgr::get_instance(TOKEN_PLUGIN));
  mgr->notify_death();
  new (this) anonymous_token(m_token);
}
