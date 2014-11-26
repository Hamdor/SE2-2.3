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
#include "lib/dispatcher/abstract_dispatcher.hpp"
#include "lib/util/abstract_singleton.hpp"
#include "lib/util/HAWThread.hpp"

#include <queue>
#include <cstddef>

namespace se2 {
namespace unit_tests {
class dispatcher_test;
}
namespace util {
class singleton_mgr;
}
namespace dispatch {

struct dispatcher : public abstract_dispatcher
                  , public util::abstract_singleton
                  , public util::HAWThread {
  friend unit_tests::dispatcher_test;
  friend util::singleton_mgr;
  /**
   * Default Destuktor
   **/
  virtual ~dispatcher();

  /**
   * Registriert einen Listener fuer ein Event.
   * Der Listener erhaellt Nachricht fuer dieses Event solange er
   * Registriert ist.
   * @param listener Pointer auf den Status
   * @param event    auf welches gehoert werden soll
   * @return TRUE    nach erfolgreichen hinzufuegen
   *         FALSE   wenn bereits die maximale Anzahl an listenern fuer
   *                 dieses Event erreicht ist
   **/
  virtual bool register_listener(fsm::events* listener,
                                 hal::event_values event);

  /**
   * Ruft das event direkt auf, ohne das eine PulseMessage exsistiert
   * @param event welches ausgeloest werden soll
   *        da dann synchronisation zwischen dem Dispatcher Thread
   *        und dem aufrufendem Thread benoetigt wird.
   **/
  virtual void direct_call_event(hal::event_values event);

  /**
   * Mappt von `event_values` auf `dispatcher_events`
   * fuer zugriff auf Matrix
   * @param  val der Wert der in `dispatcher_events` abgebildet werden soll
   * @return gibt den Wert in `dispatcher_events` zurueck
   **/
  static dispatcher_events map_from_event_values(hal::event_values val);
 private:
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
};

} // namespace dispatch
} // namespace se2

#endif // SE2_DISPATCHER
