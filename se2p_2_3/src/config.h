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
 * @file    config.h
 * @version 0.1
 *
 * Globaler Config Header
 * Sollte in jeder Datei includiert sein
 **/

#ifndef SE2_CONFIG_H
#define SE2_CONFIG_H

//#define UNIT_TESTS_STUB   // Unit Tests der HAL aktivieren
//#define UNIT_TESTS_IRQ    // Unit Tests der IRQ aktivieren
//#define REAL_HW           // Deaktiviert das LOG file
//#define SIMULATION        // Aktiviert die Verbindung zur Simulation

#ifdef UNIT_TESTS_STUB
// Es kann zur Zeit nur mit einem Test kompiliert werden,
// das liegt daran, dass die HAL Unit Tests stubs brauchen.
// Diese können jedoch keine interrupts auslösen.
// TODO: Besseres verhalten
  #undef UNIT_TESTS_IRQ
#warning "UNIT_TESTS_IRQ nicht möglich mit UNIT_TESTS_STUB"
#endif

#endif // SE2_CONFIG_H
