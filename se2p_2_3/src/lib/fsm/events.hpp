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

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

namespace se2 {
namespace fsm {

class events {
 public:
  virtual ~events(void){ };
  virtual void dispatched_event_button_start(void)    = 0;
  virtual void dispatched_event_button_stop(void)     = 0;
  virtual void dispatched_event_button_reset(void)    = 0;
  virtual void dispatched_event_button_e_stop(void)   = 0;
  virtual void dispatched_event_sensor_entrance(void) = 0;
  virtual void dispatched_event_sensor_height(void)   = 0;
  virtual void dispatched_event_sensor_switch(void)   = 0;
  virtual void dispatched_event_sensor_slide(void)    = 0;
  virtual void dispatched_event_sensor_exit(void)     = 0;
  virtual void dispatched_event_serial_data(void)     = 0;
  virtual void dispatched_event_serial_msg(void)      = 0;
  virtual void dispatched_event_serial_err(void)      = 0;
  virtual void dispatched_event_serial_unk(void)      = 0;
  virtual void dispatched_event_seg1_exceeded(void)   = 0;
  virtual void dispatched_event_seg2_exceeded(void)   = 0;
  virtual void dispatched_event_seg3_exceeded(void)   = 0;
  virtual void dispatched_event_slide_full(void)      = 0;
  virtual void dispatched_event_open_switch(void)     = 0;
  virtual void dispatched_event_turn_token(void)      = 0;
  virtual void dispatched_event_remove_token(void)    = 0;
  virtual void dispatched_event_token_finished(void)  = 0;
  virtual void dispatched_event_max(void)             = 0;
};

} // namespace fsm
} // namespace se2

#endif /* EVENTS_HPP_ */
