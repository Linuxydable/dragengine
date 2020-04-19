/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEPLAYBACKVARIABLELIST_H_
#define _CEPLAYBACKVARIABLELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class cePlaybackVariable;



/**
 * \brief Playback Variable List.
 */
class cePlaybackVariableList{
private:
	decObjectOrderedSet pVariables;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	cePlaybackVariableList();
	/** Cleans up the list. */
	~cePlaybackVariableList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of variables. */
	int GetCount() const;
	/** Retrieves the variable at the given position. */
	cePlaybackVariable *GetAt( int index ) const;
	/** Retrieves the named variable or NULL if not found. */
	cePlaybackVariable *GetNamed( const char *name ) const;
	/** Retrieves the index of a variable or -1 if not found. */
	int IndexOf( cePlaybackVariable *variable ) const;
	/** Retrieves the index of a named variable or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a variable exists. */
	bool Has( cePlaybackVariable *variable ) const;
	/** Determines if a named variable exists. */
	bool HasNamed( const char *name ) const;
	/** Sets a variable adding it if not existing. */
	void Set( const char *name, int value );
	/** Removes a variable. */
	void Remove( cePlaybackVariable *variable );
	/** Removes all variables. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	cePlaybackVariableList &operator=( const cePlaybackVariableList &list );
	/*@}*/
};

#endif
