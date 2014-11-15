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

namespace se2 {
namespace util {
class singleton_mgr;
}
namespace dispatch {

struct dispatcher : public abstract_dispatcher
                  , public util::abstract_singleton {
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
   * TODO: Typen ersetzen
   **/
  virtual bool register_listener(void* listener, hal::event_values event);

  /**
   * Unregistriert einen Listener von einem Event.
   * @param listener Pointer auf den Status
   * @param event    welches nicht mehr gehoert werden soll
   * @return TRUE    nach erfoglreichen entfernen
   *         FALSE   listener hat nicht auf das event gehorcht
   * TODO: Typen ersetzen
   **/
  virtual bool unregister_listener(void* listener, hal::event_values event);

  /**
   * Unregistriert einen Listener von allen Events.
   * @param listener Pointer
   * TODO: Typen ersetzen
   **/
  virtual void unregister_from_all(void* listener);

  /**
   * Ruft das event direkt auf, ohne das eine PulseMessage exsistiert
   * @param Event welches ausgeloest werden soll
   *       da dann synchronisation zwischen dem Dispatcher Thread
   *       und dem aufrufendem Thread benoetigt wird.
   **/
  virtual void direct_call_event(hal::event_values event);
 private:
  /**
   * Default Konstruktor (private)
   **/
  dispatcher();

  static dispatcher* instance;
  /**
   * Mappt von `event_values` auf `dispatcher_events`
   * fuer zugriff auf Matrix
   **/
  static dispatcher_events map_from_event_values(hal::event_values val);

  /**
   * Initialisierung des Singletons
   **/
  virtual void initialize();

  /**
   * Zerstoerung des Singleton
   **/
  virtual void destroy();
};

} // namespace dispatch
} // namespace se2

#endif // SE2_DISPATCHER
