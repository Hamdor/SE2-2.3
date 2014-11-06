/******************************************************************************
 *                                                                            *
 *    Hochschule für Angewandte Wissenschaften Hamburg                        *
 *                                                                            *
 *    WP                                                                      *
 *    Effiziente Datenstrukturen in C++ für verteilte Echtzeitprogrammierung  *
 *                                                                            *
 *    Prof. Dr. Stephan Pareigis                                              *
 *                                                                            *
 *    Thread Class                                                            *
 *                                                                            *
 ******************************************************************************
 * Gruppe 2.3:                                                                *
 * Bei dieser Datei wurde Namespace sowie Codingstyle angepasst.              *
 * Variablennamen und C-Style Casts wurden Refaktorisiert.                    *
 * Die Originalversion dieser Datei befindet sich im Pub.                     *
 ******************************************************************************/

#ifndef _HAW_THREAD_H_
#define _HAW_THREAD_H_

#include "config.h"

#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>

/**
 * HAWThread class.
 * Encapsulates the most important features of a thread.
 * This serves as a basis for further development.
 * For example, priorities could be passed as constructor 
 * argument.
 */

namespace se2 {
namespace util {

class HAWThread
{
 public:
  /**
   * Constructor. Initializes members
   */
  HAWThread();

  /**
   * Destructor. Calls ThreadDestroy. This should
   * shutdown the thread more carefully.
   * @warning needs some work!
   */
  virtual ~HAWThread();

  /**
   * Starts the Thread.
   * @warning start must be called always from the same context as stop().
   * @warning If Thread is already running, start() is a NOP.
   * @arg argument is stored locally as a member.
   */
  virtual void start(void* arg);

  /**
   * Sets the internal flag LOCAL_EXIT to true.
   */
  void stop();

  /**
   * Calls join on the thread.
   * @warning must be called from the same context as start.
   */
  void join() const;

  /**
   * This function holds (suspends) the thread. It makes a ThreadCtrl call.
   * It shall be used if the thread is not being used for a while but
   * may be used later
   */
  void hold();

  /**
   * This function continues (resumes) the thread. It makes a ThreadCtrl call.
   */
  void cont();

protected:
  /** This is called when the thread is started.
   * It calls execute an shutdown which are
   * the user functions.
   */
  void run(void * arg);

  static void* entryPoint(void*);
  /**
   * to be implemented in the derived class.
   * The application programmer has to write his own loop.
   * He can check bool isStopped() to see if the thread
   * should exit the loop.
   */
  virtual void execute(void*) = 0;

  /**
   * this function must be implemented
   * in the derived class.
   * The function is called once after
   * the thread has been stopped.
   */
  virtual void shutdown() = 0;

  /**
   * used internally to pass the argument.
   */
  inline void* Arg() const { return m_arg; }

  /**
   * used internally to set the arguement.
   */
  inline void Arg(void* a) { m_arg = a; }

  /**
   * returns the stop-status of the thread.
   * This function should be checked by
   * the user function execute regularly.
   */
  inline bool isStopped() const { return (s_exit || m_exit); }

	/**
	 * sets the GLOBAL_EXIT flag to true.
	 */
  inline void shutdownAll() { s_exit = true; }

 private:
  pthread_t            m_tid;     /**< thread id to be passed to pthread_create*/
  volatile bool        m_running; /**< flag to check if thread is running*/
  void *               m_arg;
  static volatile bool s_exit;    /**< close all threads */
  bool                 m_exit;    /**< stop this thread  */
};

} // namespace util
} // namespace se2

#endif
