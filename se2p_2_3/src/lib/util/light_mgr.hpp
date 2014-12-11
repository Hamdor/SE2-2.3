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
 * @file    light_mgr.hpp
 * @version 0.1
 *
 * Der `light_mgr` soll die Ansteuerung der Ampelanlage uebernehmen
 **/

#ifndef SE2_LIGHT_MGR_HPP
#define SE2_LIGHT_MGR_HPP

#include "lib/util/abstract_singleton.hpp"
#include "lib/util/HAWThread.hpp"
#include "lib/timer/timer_wrapper.hpp"
#include "lib/constants.hpp"

namespace se2 {
namespace util {
class singleton_mgr;
class light_mgr : public abstract_singleton
                , public HAWThread {
  friend class singleton_mgr;
  /**
   * Default Konstruktor
   **/
  light_mgr();
  /**
   * Default Destruktor
   **/
  virtual ~light_mgr();

  virtual void initialize();
  virtual void destroy();

  void update_light();

  virtual void execute(void*);
  virtual void shutdown();

  static light_mgr* instance;

  int                   m_chid;
  bool                  m_tick; // tick/tock
  timer::timer_wrapper* m_timer;
  light_states          m_state;

 public:
  /**
   * Aendert das licht der Ampelanlage
   * @param state beschreibt den State in dem sich die Ampelanlage
   *        befinden soll
   **/
  void set_state(light_states state);
};

} // namespace util
} // namespace se2

#endif // SE2_LIGHT_MGR_HPP
