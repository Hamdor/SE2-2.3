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

namespace se2 {
namespace dispatcher {

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
   * Registriert ist.
   * @param listener Pointer auf den Status
   * @param event    auf welches gehoert werden soll
   * @return TRUE    nach erfolgreichen hinzufuegen
   *         FALSE   wenn bereits die maximale Anzahl an listenern fuer
   *                 dieses Event erreicht ist
   * TODO: Typen ersetzen
   **/
  virtual bool register_listener(void* listener, void* event) = 0;

  /**
   * Unregistriert einen Listener von einem Event.
   * @param listener Pointer auf den Status
   * @param event    welches nicht mehr gehoert werden soll
   * @return TRUE    nach erfoglreichen entfernen
   *         FALSE   listener hat nicht auf das event gehorcht
   * TODO: Typen ersetzen
   **/
  virtual bool unregister_listener(void* listener, void* event) = 0;

  /**
   * Unregistriert einen Listener von allen Events.
   * @param listener Pointer
   * TODO: Typen ersetzen
   **/
  virtual void unregister_from_all(void* listener) = 0;

  /**
   * Ruft das event direkt auf, ohne das eine PulseMessage exsistiert
   * @param Event welches ausgeloest werden soll
   * TODO: Benoetigt? Wuerde die Sache verkomplizieren,
   *       da dann synchronisation zwischen dem Dispatcher Thread
   *       und dem aufrufendem Thread benoetigt wird.
   **/
  virtual void direct_call_event(void* event) = 0;
};

} // namespace dispatcher
} // namespace se2

#endif // SE2_ABSTRACT_DISPATCHER
