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
#include "lib/hal/hwaccess.hpp"

#include <cstring>

using namespace se2::util;

light_mgr* light_mgr::instance = 0;

light_mgr::light_mgr() : m_chid(0), m_tick(0), m_timer(NULL) {
  // nop
}

light_mgr::~light_mgr() {
  delete m_timer;
  instance = 0;
}

void light_mgr::initialize() {
  m_chid = ChannelCreate(0);
  if (m_chid == -1) {
    LOG_ERROR("ChannelCreate() failed!")
    return;
  }
}

void light_mgr::destroy() {
  // nop
}

void light_mgr::execute(void*) {
  struct _pulse buffer;
  while(!isStopped()) {
    hal::hwaccess* hal = TO_HAL(singleton_mgr::get_instance(HAL_PLUGIN));
    std::memset(&buffer, 0, sizeof(buffer));
    MsgReceivePulse(m_chid, &buffer, sizeof(_pulse), NULL);
    if (m_tick) {
      switch(m_state) {
      case NO_LIGHTS: {

      } break;
      case READY_TO_USE:
        break;
      case TURN_TOKEN:
        break;
      case REMOVE_TOKEN:
        break;
      case ERROR_NOT_RESOLVED:
        break;
      case ERROR_GONE:
        break;
      case ERROR_RESOLVED:
        break;
      }
    }
  }
}

void light_mgr::shutdown() {
  // nop
}

void light_mgr::set_state(light_states state) {

}
