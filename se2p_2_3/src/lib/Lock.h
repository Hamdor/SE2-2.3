/**
 * @file    Lock.h
 * @author  Simon Brummer
 * @version 0.1
 *
 * Headerfile zur Klasse Lock.
 * Lock ist eine Variante des "Scoped Locking" Pattern.
 */

#ifndef LOCK_H_
#define LOCK_H_
#include <pthread.h>

class Lock {
    private:
        pthread_mutex_t* pMtx_;   ///< Mutex der von diesem Lock belegt wird.

    public:
        Lock(pthread_mutex_t* pMtx);
        ~Lock();
    
    private:
        Lock(const Lock& l);
	    Lock& operator=(Lock& l);
};

#endif /* LOCK_H_ */
