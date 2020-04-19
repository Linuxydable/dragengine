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

#include "igdeActionContextMenu.h"
#include "igdeActionContextMenuReference.h"



// Class igdeActionContextMenuReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionContextMenuReference::igdeActionContextMenuReference(){
}

igdeActionContextMenuReference::igdeActionContextMenuReference( igdeActionContextMenu *action ) :
deObjectReference( action ){
}

igdeActionContextMenuReference::igdeActionContextMenuReference( const igdeActionContextMenuReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionContextMenuReference::TakeOver( igdeActionContextMenu *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionContextMenuReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionContextMenuReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionContextMenuReference::operator igdeActionContextMenu *() const{
	return ( igdeActionContextMenu* )operator deObject *();
}

igdeActionContextMenuReference::operator igdeActionContextMenu &() const{
	return ( igdeActionContextMenu& )operator deObject &();
}

igdeActionContextMenu *igdeActionContextMenuReference::operator->() const{
	return ( igdeActionContextMenu* )deObjectReference::operator->();
}

igdeActionContextMenuReference &igdeActionContextMenuReference::operator=( igdeActionContextMenu *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionContextMenuReference &igdeActionContextMenuReference::operator=( const igdeActionContextMenuReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionContextMenuReference::operator==( igdeActionContextMenu *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionContextMenuReference::operator!=( igdeActionContextMenu *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionContextMenuReference::operator==( const igdeActionContextMenuReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionContextMenuReference::operator!=( const igdeActionContextMenuReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
