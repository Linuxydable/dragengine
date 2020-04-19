/* 
 * Drag[en]gine Game Engine
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSkyControllerTarget.h"

#include "../../common/exceptions.h"



// Class deSkyControllerTarget
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkyControllerTarget::deSkyControllerTarget(){
}

deSkyControllerTarget::deSkyControllerTarget( const deSkyControllerTarget &target ) :
pLinks( target.pLinks ){
}

deSkyControllerTarget::~deSkyControllerTarget(){
}



// Management
///////////////

int deSkyControllerTarget::GetLinkCount() const{
	return pLinks.GetCount();
}

int deSkyControllerTarget::GetLinkAt( int index ) const{
	return pLinks.GetAt( index );
}

bool deSkyControllerTarget::HasLink( int link ) const{
	return pLinks.Has( link );
}

void deSkyControllerTarget::AddLink( int link ){
	pLinks.Add( link );
}

void deSkyControllerTarget::RemoveLink( int link ){
	const int index = pLinks.IndexOf( link );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pLinks.RemoveFrom( index );
}

void deSkyControllerTarget::RemoveAllLinks(){
	pLinks.RemoveAll();
}
