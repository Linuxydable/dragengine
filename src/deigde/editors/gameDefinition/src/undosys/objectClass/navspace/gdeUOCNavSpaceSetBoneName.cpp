/* 
 * Drag[en]gine IGDE Game Definition Editor
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
#include <string.h>
#include <stdlib.h>

#include "gdeUOCNavSpaceSetBoneName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCNavSpaceSetBoneName
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCNavSpaceSetBoneName::gdeUOCNavSpaceSetBoneName( gdeObjectClass *objectClass,
gdeOCNavigationSpace *navSpace, const char *newValue ) :
pObjectClass( NULL ),
pNavSpace( NULL )
{
	if( ! objectClass || ! navSpace ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "NavSpace set bone name" );
	
	pOldValue = navSpace->GetBoneName();
	pNewValue = newValue;
	
	pNavSpace = navSpace;
	navSpace->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCNavSpaceSetBoneName::~gdeUOCNavSpaceSetBoneName(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCNavSpaceSetBoneName::Undo(){
	pNavSpace->SetBoneName( pOldValue );
	pObjectClass->NotifyNavigationSpaceChanged( pNavSpace );
}

void gdeUOCNavSpaceSetBoneName::Redo(){
	pNavSpace->SetBoneName( pNewValue );
	pObjectClass->NotifyNavigationSpaceChanged( pNavSpace );
}
