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
 * @file    state.hpp
 * @version 0.1
 *
 * Interface/Abstrakte Klasse fuer Zustandsautomaten
 **/

#ifndef SE2_STATE_HPP
#define SE2_STATE_HPP

#include "lib/token.hpp"
#include "lib/fsm/events.hpp"
#include "lib/util/singleton_mgr.hpp"

namespace se2 {
namespace fsm {

class state : public events {
 protected:
  token* m_token;

 public:
  state() : m_token(NULL) {
    // nop
  }
  state(token* t) : m_token(t) {
    // nop
  }

  ~state() {
    // nop
  }

  virtual void dispatched_event_button_start() {
    // nop
  }
  virtual void dispatched_event_button_stop() {
    // nop
  }
  virtual void dispatched_event_button_reset() {
    // nop
  }
  virtual void dispatched_event_button_e_stop() {
    // nop
  }
  virtual void dispatched_event_sensor_entrance() {
    // nop
  }
  virtual void dispatched_event_sensor_entrance_rising() {
    // nop
  }
  virtual void dispatched_event_sensor_height() {
    // nop
  }
  virtual void dispatched_event_sensor_height_rising() {
    // nop
  }
  virtual void dispatched_event_sensor_switch() {
    // nop
  }
  virtual void dispatched_event_sensor_switch_rising() {
    // nop
  }
  virtual void dispatched_event_sensor_slide() {
    // nop
  }
  virtual void dispatched_event_sensor_slide_rising() {
    // nop
  }
  virtual void dispatched_event_sensor_exit() {
    // nop
  }
  virtual void dispatched_event_sensor_exit_rising() {
    // nop
  }
  virtual void dispatched_event_serial_data() {
    // nop
  }
  virtual void dispatched_event_serial_msg() {
    // nop
  }
  virtual void dispatched_event_serial_err() {
    // nop
  }
  virtual void dispatched_event_serial_next_ok() {
    // nop
  }
  virtual void dispatched_event_serial_transfer_fin() {
    // nop
  }
  virtual void dispatched_event_serial_e_stopp() {
    // nop
  }
  virtual void dispatched_event_serial_unk() {
    // nop
  }
  virtual void dispatched_event_seg1_has_to_expire() {
    // nop
  }
  virtual void dispatched_event_seg2_has_to_expire() {
    // nop
  }
  virtual void dispatched_event_seg3_has_to_expire() {
    // nop
  }
  virtual void dispatched_event_seg1_too_late() {
    // nop
  }
  virtual void dispatched_event_seg2_too_late() {
    // nop
  }
  virtual void dispatched_event_seg3_too_late() {
    // nop
  }
  virtual void dispatched_event_slide_full_timeout() {
    // nop
  }
  virtual void dispatched_event_turn_token_timeout() {
    // nop
  }
  virtual void dispatched_event_remove_token_timeout() {
    // nop
  }
  virtual void dispatched_event_token_finished_timeout() {
    // nop
  }
};

class anonymous_token : public state {
 public:
  anonymous_token(token* t);
  virtual ~anonymous_token() {
    // nop
  }

  virtual void dispatched_event_sensor_entrance();
};


/******************************************************************************
 *                                BAND 1 FSM                                  *
 ******************************************************************************/

class b1_realized_object : public state {
 public:
  b1_realized_object(token* t);
  virtual ~b1_realized_object() {
    // nop
  }

  virtual void dispatched_event_seg1_has_to_expire();
  virtual void dispatched_event_sensor_height();
};

class b1_realized_object_seg1_ok : public state {
 public:
  b1_realized_object_seg1_ok(token* t);
  virtual ~b1_realized_object_seg1_ok() {
    // nop
  }

  virtual void dispatched_event_sensor_height();
  virtual void dispatched_event_seg1_too_late();
};

class b1_height_measurement : public state {
 public:
  b1_height_measurement(token* t);
  virtual~b1_height_measurement() {
    // nop
  }
};

class b1_token_too_small : public state {
 public:
  b1_token_too_small(token* t);
  virtual ~b1_token_too_small() {
    // nop
  }
  virtual void dispatched_event_sensor_slide();
  virtual void dispatched_event_sensor_switch();
  virtual void dispatched_event_sensor_height_rising();
  virtual void dispatched_event_seg2_has_to_expire();
};

class b1_token_too_small_seg2_ok : public state {
 public:
  b1_token_too_small_seg2_ok(token* t);
  virtual ~b1_token_too_small_seg2_ok() {
    // nop
  }

  virtual void dispatched_event_sensor_slide();
  virtual void dispatched_event_seg2_too_late();
  virtual void dispatched_event_sensor_slide_rising();
  virtual void dispatched_event_slide_full_timeout();
};

class b1_valid_height : public state {
 public:
  b1_valid_height(token* t);
  virtual ~b1_valid_height() {
    // nop
  }

  virtual void dispatched_event_sensor_height_rising();
  virtual void dispatched_event_sensor_switch();
  virtual void dispatched_event_sensor_switch_rising();
  virtual void dispatched_event_sensor_exit();
  virtual void dispatched_event_seg2_has_to_expire();
  virtual void dispatched_event_seg2_too_late();
};

class b1_valid_height_seg2_ok : public state {
 public:
  b1_valid_height_seg2_ok(token* t);
  virtual ~b1_valid_height_seg2_ok() {
    // nop
  }

  virtual void dispatched_event_sensor_switch();
  virtual void dispatched_event_sensor_switch_rising();
  virtual void dispatched_event_seg3_has_to_expire(); // verschieben von `b1_valid_height`
};

class b1_valid_height_seg3_ok : public state {
 public:
  b1_valid_height_seg3_ok(token* t);
  virtual ~b1_valid_height_seg3_ok() {
    // nop
  }

  virtual void dispatched_event_seg2_too_late();
  virtual void dispatched_event_sensor_exit();
  virtual void dispatched_event_seg3_too_late();
};

class b1_exit : public state {
 public:
  b1_exit(token* t);
  virtual ~b1_exit() {
    // nop
  }
};

class b1_token_upside_down : public state {
 public:
  b1_token_upside_down(token* t);
  virtual ~b1_token_upside_down() {
    // nop
  }

  virtual void dispatched_event_button_start();
  virtual void dispatched_event_sensor_exit_rising();
  virtual void dispatched_event_remove_token_timeout();
};

class b1_token_upside_down_lift_up : public state {
 public:
  b1_token_upside_down_lift_up(token* t);
  virtual ~b1_token_upside_down_lift_up() {
    // nop
  }

  virtual void dispatched_event_sensor_exit();
  virtual void dispatched_event_turn_token_timeout();
};

class b1_token_upside_down_put_back : public state {
 public:
  b1_token_upside_down_put_back(token* t);
  virtual ~b1_token_upside_down_put_back() {
    // nop
  }

  virtual void dispatched_event_button_start();
  virtual void dispatched_event_sensor_exit_rising();
};

class b1_token_ready_for_b2 : public state {
 public:
  b1_token_ready_for_b2(token* t);
  virtual ~b1_token_ready_for_b2() {
    // nop
  }

  virtual void dispatched_event_serial_next_ok();
  virtual void dispatched_event_sensor_exit_rising();
  virtual void dispatched_event_serial_transfer_fin();
};


/******************************************************************************
 *                                BAND 2 FSM                                  *
 ******************************************************************************/

class b2_receive_data : public state {
 public:
  b2_receive_data(token* t);
  virtual ~b2_receive_data() {
    // nop
  }

#ifdef CONVEYOR_2_SINGLEMOD
  virtual void dispatched_event_sensor_entrance();
#endif
  virtual void dispatched_event_serial_data();
};

class b2_received_object : public state {
 public:
  b2_received_object(token* t);
  virtual ~b2_received_object() {
    // nop
  }

  virtual void dispatched_event_sensor_entrance();
};

class b2_realized_object : public state {
 public:
  b2_realized_object(token* t);
  virtual ~b2_realized_object() {
    // nop
  }

  virtual void dispatched_event_seg1_has_to_expire();
  virtual void dispatched_event_sensor_height();
};

class b2_realized_object_seg1_ok : public state {
 public:
  b2_realized_object_seg1_ok(token* t);
  virtual ~b2_realized_object_seg1_ok() {
    // nop
  }

  virtual void dispatched_event_sensor_height();
  virtual void dispatched_event_seg1_too_late();
};

class b2_height_measurement : public state {
 public:
  b2_height_measurement(token* t);
  virtual ~b2_height_measurement() {
    // nop
  }
};

class b2_token_upside_down : public state {
 public:
  b2_token_upside_down(token* t);
  virtual ~b2_token_upside_down() {
    // nop
  }

  virtual void dispatched_event_sensor_height_rising();
  virtual void dispatched_event_sensor_slide();
};

class b2_valid_height : public state {
 public:
  b2_valid_height(token* t);
  virtual ~b2_valid_height() {
    // nop
  }

  virtual void dispatched_event_sensor_height_rising();
  virtual void dispatched_event_sensor_switch();
};

class b2_metal_detection : public state {
 public:
  b2_metal_detection(token* t);
  virtual ~b2_metal_detection() {
    // nop
  }
};

class b2_is_wrong_order : public state {
 public:
  b2_is_wrong_order(token* t);
  virtual ~b2_is_wrong_order() {
    // nop
  }

  virtual void dispatched_event_sensor_entrance();
  virtual void dispatched_event_sensor_entrance_rising();
};

class b2_is_correct_order : public state {
 public:
  b2_is_correct_order(token* t);
  virtual ~b2_is_correct_order() {
    // nop
  }

  virtual void dispatched_event_sensor_switch_rising();
  virtual void dispatched_event_sensor_exit();
  virtual void dispatched_event_sensor_exit_rising();
};

} // namespace fsm
} // namespace se2

#endif // SE2_STATE_HPP
