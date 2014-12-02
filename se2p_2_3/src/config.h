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
 * @version 0.3
 *
 * Globaler Config Header
 * Muss in jeder Datei inkludiert sein
 **/

#ifndef SE2_CONFIG_H
#define SE2_CONFIG_H

#define NUM_OF_TOKENS 20   // Anzahl an vordefinierten Tokens
                           // Die Logik kann niemals mehr als diese Anzahl an
                           // Tokens verwalten, da keine dynamische Allokierung
                           // stattfindet

//#define IS_CONVEYOR_1      // Foerderband 1 FSM aktivieren
//#define IS_CONVEYOR_2      // Foerderband 2 FSM aktivieren

#if (defined(IS_CONVEYOR_1) && defined(IS_CONVEYOR_2)) \
     || (!defined(IS_CONVEYOR_1) && !defined(IS_CONVEYOR_2))
#error Entweder IS_CONVEYOR_1 oder IS_CONVEYOR_2 definieren
#endif

//#define CONVEYOR_2_SINGLEMOD // Wenn definiert wartet Conveyor 2 nicht nur
                               // auf ein Ereignis der Seriellen Schnittstelle
                               // sondern auch auf Sensor Entrance.

//#define UNIT_TEST_SENDER // Aktiviert Senderseite im Unit-Test fuer
                           // die Serielle chnittstelle

//#define SIMULATION   // Aktiviert die Verbindung zur Simulation, hat zur
                       // Folge dass `ioaccess.h` in `abstract_inout.hpp`
                       // inkludiert wird. Dadurch koennen die Original
                       // `in8()`, `out8` sowie `AttachInterrupt()`,
                       // `DetachInterrupt()` nicht verwendet werden.
                       // Stattaedeassen werden diese Funktionen auf eigene fuer
                       // die Simulation ueberschrieben.

#if defined(SIMULATION)
#warning SIMULATION ist definiert, richtige Hardware wird nicht funktionieren
#endif

//#define PRINT_TRANSITIONS_UNIT_TEST // Aktiviert die Ausgabe der Uebergaenge
                                      // im Dispatcher Unit Test

/**
 * ----------------------------------------------------------------------------
 * Logging Spezifische Defines
 **/
#define DISABLE_LOGGING // Deaktiviert das schreiben auf das File System.
                        // Es wird nur eine Ausgabe in der Console erzeugt.
#define LOG_FILE_PATH "/root/logs/" // Gibt den Pfad zum Logfile Ordner an
                                    // Das generierte Logfile hat einen prefix
                                    // Der Name des generieten Logfiles setzt
                                    // sich wie folgt zusammen:
                                    // "log_{prozess ID}_{UNIX_TIME}.log"
/**
 * ----------------------------------------------------------------------------
 * Serielle Schnittstelle Spezifische Defines
 **/
#define HAS_SERIAL_INTERFACE        // Aktiviert die Serielle Schnittstelle
#define SERIAL_CHAR_DEV "/dev/ser1" // Pfad zum Char Device der Seriellen
                                    // Schnittstelle. In der VM NICHT verfügbar
/**
 * ----------------------------------------------------------------------------
 * Festo Spezifische Defines
 **/
//#define FESTO_3      // Aktiviert spezifisch gemessene Werte fuer Festo 3
                       // Unter anderem Daten fuer Hoehenmessung
                       // definiert in `constants.hpp` (height_values)

//#define FESTO_9      // Aktiviert spezifisch gemessene Werte fuer Festo 9
                       // Unter anderem Daten fuer Hoehenmessung
                       // definiert in `constants.hpp` (height_values)

//#define FESTO_7      // Aktiviert spezifisch gemessene Werte fuer Festo 7
                       // Unter anderem Daten fuer Hoehenmessung
                       // definiert in `constants.hpp` (height_values)

//#define FESTO_1      // Aktiviert spezifisch gemessene Werte fuer Festo 1
                       // Unter anderem Daten fuer Hoehenmessung
                       // definiert in `constants.hpp` (height_values)

//#define FESTO_6      // Aktiviert spezifisch gemessene Werte fuer Festo 6
                       // Unter anderem Daten fuer Hoehenmessung
                       // definiert in `constants.hpp` (height_values)

//#define FESTO_5      // Aktiviert spezifisch gemessene Werte fuer Festo 5
                       // Unter anderem Daten fuer Hoehenmessung
                       // definiert in `constants.hpp` (height_values)

/**
 * ----------------------------------------------------------------------------
 **/

#endif // SE2_CONFIG_H
