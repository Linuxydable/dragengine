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

#include "gdeUOCNavBlockerSetBoneName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCNavBlockerSetBoneName
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCNavBlockerSetBoneName::gdeUOCNavBlockerSetBoneName( gdeObjectClass *objectClass,
gdeOCNavigationBlocker *navBlocker, const char *newValue ) :
pObjectClass( NULL ),
pNavBlocker( NULL )
{
	if( ! objectClass || ! navBlocker ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "NavBlocker set bone name" );
	
	pOldValue = navBlocker->GetBoneName();
	pNewValue = newValue;
	
	pNavBlocker = navBlocker;
	navBlocker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCNavBlockerSetBoneName::~gdeUOCNavBlockerSetBoneName(){
	if( pNavBlocker ){
		pNavBlocker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCNavBlockerSetBoneName::Undo(){
	pNavBlocker->SetBoneName( pOldValue );
	pObjectClass->NotifyNavigationBlockerChanged( pNavBlocker );
}

void gdeUOCNavBlockerSetBoneName::Redo(){
	pNavBlocker->SetBoneName( pNewValue );
	pObjectClass->NotifyNavigationBlockerChanged( pNavBlocker );
}
