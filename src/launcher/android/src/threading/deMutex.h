/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// include only once
#ifndef _DEMUTEX_H_
#define _DEMUTEX_H_

#include "../dragengine_configuration.h"

#if defined OS_UNIX || defined OS_BEOS
#	include <pthread.h>
#endif

#ifdef OS_W32
#	include "../app/include_windows.h"
#endif


/**
 * @brief Mutex.
 *
 * Provides mutual exclusive access support using a thin wrapper around
 * the operating system specific threading mutex.
 */
class deMutex{
private:
#if defined OS_UNIX || defined OS_BEOS
	pthread_mutex_t pMutex;
#endif
#ifdef OS_W32
	CRITICAL_SECTION pCritSec;
#endif
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a mutex. */
	deMutex();
	/** Cleans the mutex. */
	~deMutex();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Locks the mutex. If the mutex is already locked this call blocks
	 * until the original called released it.
	 */
	void Lock();
	/**
	 * Locks the mutex. If the mutex is already locked this call returns
	 * false and no lock is taken. If the mutex is not locked this call
	 * acquires the lock and return true.
	 */
	bool TryLock();
	/**
	 * Unlocks the mutex. An exception is thrown if this thread is not
	 * holding the mutex.
	 */
	void Unlock();
	/*@}*/
};

// end of include only once
#endif
