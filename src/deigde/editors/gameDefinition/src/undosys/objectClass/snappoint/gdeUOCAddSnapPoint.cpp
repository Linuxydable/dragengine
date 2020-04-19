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

#include "gdeUOCAddSnapPoint.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCAddSnapPoint
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCAddSnapPoint::gdeUOCAddSnapPoint( gdeObjectClass *objectClass, gdeOCSnapPoint *snapPoint ) :
pObjectClass( NULL ),
pSnapPoint( NULL )
{
	if( ! objectClass || ! snapPoint ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add snap point" );
	
	pSnapPoint = snapPoint;
	snapPoint->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCAddSnapPoint::~gdeUOCAddSnapPoint(){
	if( pSnapPoint ){
		pSnapPoint->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCAddSnapPoint::Undo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCSnapPoint() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSnapPoint ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCSnapPoint( NULL );
	}
	
	pObjectClass->GetSnapPoints().Remove( pSnapPoint );
	pObjectClass->NotifySnapPointsChanged();
}

void gdeUOCAddSnapPoint::Redo(){
	pObjectClass->GetSnapPoints().Add( pSnapPoint );
	pObjectClass->NotifySnapPointsChanged();
}
