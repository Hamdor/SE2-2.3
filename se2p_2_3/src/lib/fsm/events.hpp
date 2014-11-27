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
 * @file    events.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse fuer Events
 **/

#ifndef SE2_EVENTS_HPP
#define SE2_EVENTS_HPP

namespace se2 {
namespace fsm {

class events {
 public:
  virtual ~events(){
    // nop
  }

  virtual void dispatched_event_button_start()    = 0;
  virtual void dispatched_event_button_stop()     = 0;
  virtual void dispatched_event_button_reset()    = 0;
  virtual void dispatched_event_button_e_stop()   = 0;
  virtual void dispatched_event_sensor_entrance() = 0;
  virtual void dispatched_event_sensor_height()   = 0;      // fallende flanke
  virtual void dispatched_event_sensor_height_rising() = 0; // steigende flanke
  virtual void dispatched_event_sensor_switch()   = 0;      // fallende flanke
  virtual void dispatched_event_sensor_switch_rising() = 0; // steigende flanke
  virtual void dispatched_event_sensor_slide()    = 0;      // fallende flanke
  virtual void dispatched_event_sensor_slide_rising() = 0;  // steigende flanke
  virtual void dispatched_event_sensor_exit()     = 0;      // fallende Flanke
  virtual void dispatched_event_sensor_exit_rising() = 0;   // steigende Flanke
  virtual void dispatched_event_serial_data()     = 0;
  virtual void dispatched_event_serial_msg()      = 0;
  virtual void dispatched_event_serial_err()      = 0;
  virtual void dispatched_event_serial_unk()      = 0;
  virtual void dispatched_event_seg1_exceeded()   = 0;
  virtual void dispatched_event_seg2_exceeded()   = 0;
  virtual void dispatched_event_seg3_exceeded()   = 0;
  virtual void dispatched_event_slide_full()      = 0;
  virtual void dispatched_event_open_switch()     = 0;
  virtual void dispatched_event_turn_token()      = 0;
  virtual void dispatched_event_remove_token()    = 0;
  virtual void dispatched_event_token_finished()  = 0;
};

} // namespace fsm
} // namespace se2

#endif // SE2_EVENTS_HPP
