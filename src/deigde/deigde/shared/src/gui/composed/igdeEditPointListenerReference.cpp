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

#include "igdeEditPointListener.h"
#include "igdeEditPointListenerReference.h"



// Class igdeEditPointListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPointListenerReference::igdeEditPointListenerReference(){
}

igdeEditPointListenerReference::igdeEditPointListenerReference( igdeEditPointListener *listener ) :
deObjectReference( listener ){
}

igdeEditPointListenerReference::igdeEditPointListenerReference( const igdeEditPointListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPointListenerReference::TakeOver( igdeEditPointListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPointListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPointListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPointListenerReference::operator igdeEditPointListener *() const{
	return ( igdeEditPointListener* )operator deObject *();
}

igdeEditPointListenerReference::operator igdeEditPointListener &() const{
	return ( igdeEditPointListener& )operator deObject &();
}

igdeEditPointListener *igdeEditPointListenerReference::operator->() const{
	return ( igdeEditPointListener* )deObjectReference::operator->();
}

igdeEditPointListenerReference &igdeEditPointListenerReference::operator=( igdeEditPointListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPointListenerReference &igdeEditPointListenerReference::operator=( const igdeEditPointListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPointListenerReference::operator==( igdeEditPointListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPointListenerReference::operator!=( igdeEditPointListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPointListenerReference::operator==( const igdeEditPointListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPointListenerReference::operator!=( const igdeEditPointListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
