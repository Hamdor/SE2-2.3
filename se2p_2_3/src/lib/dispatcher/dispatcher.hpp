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
 * @file    dispatcher.hpp
 * @version 0.1
 *
 * Dispatcher
 **/

#ifndef SE2_DISPATCHER
#define SE2_DISPATCHER

#include "config.h"

#include "lib/constants.hpp"
#include "lib/util/HAWThread.hpp"
#include "lib/util/abstract_singleton.hpp"
#include "lib/dispatcher/abstract_dispatcher.hpp"

#include <map>
#include <queue>
#include <cstddef>

namespace se2 {
namespace util {
class singleton_mgr;
}
namespace unit_tests {
class dispatcher_test;
}
namespace dispatch {

struct dispatcher : public abstract_dispatcher
                  , public util::abstract_singleton
                  , public util::HAWThread {
  friend util::singleton_mgr;
  friend unit_tests::dispatcher_test;
  /**
   * Default Destuktor
   **/
  virtual ~dispatcher();

  virtual bool register_listener(fsm::events* listener,
                                 hal::event_values event);

  /**
   * Mappt von `event_values` auf `dispatcher_events`
   * fuer zugriff auf Matrix
   * @param  map Referenz auf zu durchsuchende map
   * @param  val der Wert der in `dispatcher_events` abgebildet werden soll
   * @return gibt den Wert in `dispatcher_events` zurueck
   **/
  static dispatcher_events map_from_event_values(
      const std::map<hal::event_values, dispatcher_events>& map,
      hal::event_values val);
 private:

  virtual void direct_call_event(hal::event_values event);

  /**
   * Default Konstruktor (private)
   **/
  dispatcher();

  static dispatcher* instance;

  /**
   * Initialisierung des Singletons
   **/
  virtual void initialize();

  /**
   * Zerstoerung des Singleton
   **/
  virtual void destroy();

  /**
   * Diese Funktion behandelt eventuelle Sonderfaelle der einzelnen
   * Signale. Intern durch ein Switch/Case abgedeckt.
   * @param buffer eine Referenz auf eine Pulse Message
   **/
  void special_case_handling(const _pulse& buffer);

  virtual void execute(void*);

  virtual void shutdown();

  std::queue<fsm::events*> m_listeners[DISPATCHED_EVENT_MAX];
  func_t                   m_functions[DISPATCHED_EVENT_MAX];
  std::map<hal::event_values, dispatcher_events> m_mapping;
};

} // namespace dispatch
} // namespace se2

#endif // SE2_DISPATCHER
