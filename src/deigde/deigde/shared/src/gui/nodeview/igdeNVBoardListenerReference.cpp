/* 
 * Drag[en]gine IGDE
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

#include "igdeNVBoardListener.h"
#include "igdeNVBoardListenerReference.h"



// Class igdeNVBoardListenerReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVBoardListenerReference::igdeNVBoardListenerReference(){
}

igdeNVBoardListenerReference::igdeNVBoardListenerReference( igdeNVBoardListener *listener ) :
deObjectReference( listener ){
}

igdeNVBoardListenerReference::igdeNVBoardListenerReference( const igdeNVBoardListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVBoardListenerReference::TakeOver( igdeNVBoardListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeNVBoardListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVBoardListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVBoardListenerReference::operator igdeNVBoardListener *() const{
	return ( igdeNVBoardListener* )operator deObject *();
}

igdeNVBoardListenerReference::operator igdeNVBoardListener &() const{
	return ( igdeNVBoardListener& )operator deObject &();
}

igdeNVBoardListener *igdeNVBoardListenerReference::operator->() const{
	return ( igdeNVBoardListener* )deObjectReference::operator->();
}

igdeNVBoardListenerReference &igdeNVBoardListenerReference::operator=( igdeNVBoardListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeNVBoardListenerReference &igdeNVBoardListenerReference::operator=( const igdeNVBoardListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVBoardListenerReference::operator==( igdeNVBoardListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeNVBoardListenerReference::operator!=( igdeNVBoardListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeNVBoardListenerReference::operator==( const igdeNVBoardListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVBoardListenerReference::operator!=( const igdeNVBoardListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
