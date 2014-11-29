/******************************************************************************
 *                                                                            *
 *    Hochschule für Angewandte Wissenschaften Hamburg                        *
 *                                                                            *
 *    WP                                                                      *
 *    Effiziente Datenstrukturen in C++ für verteilte Echtzeitprogrammierung  *
 *                                                                            *
 *    Prof. Dr. Stephan Pareigis                                              *
 *                                                                            *
 *    Thread Class Implementation                                             *
 *                                                                            *
 ******************************************************************************
 * Gruppe 2.3:                                                                *
 * Bei dieser Datei wurde Namespace sowie Codingstyle angepasst.              *
 * Variablennamen und C-Style Casts wurden Refaktorisiert.                    *
 * Die Originalversion dieser Datei befindet sich im Pub.                     *
 ******************************************************************************/

#include "lib/util/HAWThread.hpp"

using namespace se2::util;

volatile bool HAWThread::s_exit = false;

HAWThread::HAWThread() : m_running(false), m_arg(NULL), m_exit(false) {
  // nop
}

HAWThread::~HAWThread() {
	ThreadDestroy(m_tid , 0 , 0);
}

void HAWThread::start(void * arg) {
	if (m_running) {
	  return;
	}
	m_running = true;
  Arg(arg); // store user data
  int rc = pthread_create(&m_tid, NULL, (HAWThread::entryPoint), this);
  if (rc != 0) {
    std::cout << "Thread could not be started. Error: "
              << rc << std::endl;
  }
}

void HAWThread::stop() {
	m_exit = true;
}

void HAWThread::hold() {
	ThreadCtl(_NTO_TCTL_ONE_THREAD_HOLD, static_cast<void*>(&m_tid));
}

void HAWThread::cont() {
	ThreadCtl(_NTO_TCTL_ONE_THREAD_CONT, static_cast<void*>(&m_tid));
}

void HAWThread::run(void* arg) {
   execute( arg );
   shutdown();
}

void* HAWThread::entryPoint(void* pthis) {
  HAWThread* pt = static_cast<HAWThread*>(pthis);
  pt->run(pt->Arg());
  return NULL;
}

void HAWThread::join() const {
	pthread_join(m_tid, NULL);
}
