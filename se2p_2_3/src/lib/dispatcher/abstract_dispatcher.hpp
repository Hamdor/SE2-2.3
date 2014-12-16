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
 * @file    abstract_dispatcher.hpp
 * @version 0.1
 *
 * Dispatcher Interface
 **/

#ifndef SE2_ABSTRACT_DISPATCHER
#define SE2_ABSTRACT_DISPATCHER

#include "config.h"
#include "lib/constants.hpp"

namespace se2 {
namespace fsm {
class events;
}
namespace dispatch {

typedef void (fsm::events::*func_t)();

/**
 * Dispatcher Interface
 **/
struct abstract_dispatcher {
  /**
   * Default Destruktor
   **/
  virtual ~abstract_dispatcher() {
    // nop
  }

  /**
   * Registriert einen Listener fuer ein Event.
   * Der Listener erhaellt Nachricht fuer dieses Event solange er
   * Registriert ist
   * @param listener Pointer auf den Status
   * @param event    Auf welches gehoert werden soll
   * @return TRUE    Nach erfolgreichen hinzufuegen
   *         FALSE   Wenn bereits die maximale Anzahl an Listenern fuer
   *                 dieses Event erreicht ist
   **/
  virtual bool register_listener(fsm::events* listener,
                                 hal::event_values event) = 0;

  /**
   * Registriert einen Listener fuer ein Event.
   * Dieser Listener bekommt alle Nachrichten fuer dieses
   * Event solange er angemeldet ist. Normal angemeldete Zustaende
   * bekommen diese Events nicht
   * @param listener Pointer auf den Status
   * @param event    Auf welches gehoert werden soll
   * @return TRUE    Nach erfolgreichen hinzufuegen
   *         FALSE   Wenn bereits die maximale Anzahl an Listenern fuer
   *                 dieses Event erreicht ist
   **/
  virtual bool register_prior_listener(fsm::events* listener,
                                       hal::event_values event) = 0;

  /**
   * Unregistriert einen priorisierten Listener von einem Event.
   * Normal registrierte Listener bekommen wieder Event Nachrichten.
   * @param listener Pointer auf den Status
   * @param event    Auf welches gehoert werden soll
   * @return TRUE    Nach erfolgreichen hinzufuegen
   *         FALSE   Wenn bereits die maximale Anzahl an Listenern fuer
   *                 dieses Event erreicht ist
   **/
  virtual bool unregister_prior_listener(fsm::events* listener,
                                         hal::event_values event) = 0;

  /**
   * Entfernt den ersten Listener aus der Queue wenn es der Uebergebene
   * Listener ist
   * @param listener Der zu entfernende Listener
   * @param event Das Event von welchem geloescht werden soll
   **/
  virtual void force_pop(fsm::events* listener, hal::event_values event) = 0;

  /**
   * Entfernt einen Listener von allen Events
   * @param listener Listener der entfernt werden soll
   **/
  virtual void remove_from_all(fsm::events* listener) = 0;

 private:
  /**
   * Ruft das Event direkt auf, ohne das eine PulseMessage exsistiert
   * @param event Welches ausgeloest werden soll
   * @warning Diese Funktion wird eventuell geloescht. Sollte deshalb
   *          nicht verwendet werden
   **/
  virtual void direct_call_event(hal::event_values event) = 0;

  /**
   * Entfernt einen einzelnen Listener aus einer Queue
   **/
  virtual void single_remove(fsm::events* listener, size_t idx) = 0;
};

} // namespace dispatch
} // namespace se2

#endif // SE2_ABSTRACT_DISPATCHER
