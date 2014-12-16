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
 * @file    light_mgr.cpp
 * @version 0.1
 *
 * Der `light_mgr` soll die Ansteuerung der Ampelanlage uebernehmen
 **/

#include "lib/util/light_mgr.hpp"
#include "lib/util/logging.hpp"
#include "lib/hal/HWaccess.hpp"

#include <cstring>

using namespace se2::hal;
using namespace se2::util;
using namespace se2::timer;

#define TIMER_TICK_MSG 42

light_mgr* light_mgr::instance = 0;

light_mgr::light_mgr() : m_chid(0), m_tick(0), m_timer(NULL)
                       , m_state(NO_LIGHTS) {
  // nop
}

light_mgr::~light_mgr() {
  instance = 0;
  delete m_timer;
  ChannelDestroy(m_chid);
}

void light_mgr::initialize() {
  m_chid = ChannelCreate(0);
  if (m_chid == -1) {
    LOG_ERROR("ChannelCreate() failed!")
    return;
  }
  set_state(NO_LIGHTS);
  start(NULL);
}

void light_mgr::destroy() {
  // nop
}

void light_mgr::update_light() {
  hal::hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
  switch(m_state) {
    case NO_LIGHTS: {
      hal->set_light(GREEN, false);
      hal->set_light(YELLOW, false);
      hal->set_light(RED, false);
    } break;
    case READY_TO_USE: {
      hal->set_light(GREEN, true);
      hal->set_light(YELLOW, false);
      hal->set_light(RED, false);
    } break;
    case TURN_TOKEN:
    case REMOVE_TOKEN: {
      if (m_tick) {
        hal->set_light(GREEN, false);
        hal->set_light(YELLOW, false);
        hal->set_light(RED, false);
      } else {
        hal->set_light(GREEN, false);
        hal->set_light(YELLOW, true);
        hal->set_light(RED, false);
      }
    } break;
    case ERROR_GONE:
    case ERROR_NOT_RESOLVED: {
      if (m_tick) {
        hal->set_light(GREEN, false);
        hal->set_light(YELLOW, false);
        hal->set_light(RED, false);
      } else {
        hal->set_light(GREEN, false);
        hal->set_light(YELLOW, false);
        hal->set_light(RED, true);
      }
    } break;
    case ERROR_RESOLVED: {
      hal->set_light(GREEN, false);
      hal->set_light(YELLOW, false);
      hal->set_light(RED, true);
    } break;
  }
}

void light_mgr::execute(void*) {
  struct _pulse buffer;
  while(!isStopped()) {
    std::memset(&buffer, 0, sizeof(buffer));
    MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
    update_light();
    m_tick = !m_tick;
    m_timer->reset_timer();
    m_timer->start_timer();
  }
  update_light();
}

void light_mgr::shutdown() {
  // nop
}

void light_mgr::set_state(light_states state) {
  m_state = state;
  // unnoetiger `size_t` cast damit GCC 4.7 auch happy ist. :@)
  delete m_timer;
  duration dur = { state == ERROR_GONE ? (size_t)2 : (size_t)1, 0 };
  m_timer = new timer_wrapper(dur, TIMER_TICK_MSG, m_chid);
  update_light();
}
