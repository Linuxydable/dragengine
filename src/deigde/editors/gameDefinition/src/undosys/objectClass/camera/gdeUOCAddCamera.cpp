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

#include "gdeUOCAddCamera.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/camera/gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCAddCamera
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCAddCamera::gdeUOCAddCamera( gdeObjectClass *objectClass, gdeOCCamera *camera ) :
pObjectClass( NULL ),
pCamera( NULL )
{
	if( ! objectClass || ! camera ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add camera" );
	
	pCamera = camera;
	camera->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCAddCamera::~gdeUOCAddCamera(){
	if( pCamera ){
		pCamera->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCAddCamera::Undo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCCamera() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCCamera ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCCamera( NULL );
	}
	
	pObjectClass->GetCameras().Remove( pCamera );
	pObjectClass->NotifyCamerasChanged();
}

void gdeUOCAddCamera::Redo(){
	pObjectClass->GetCameras().Add( pCamera );
	pObjectClass->NotifyCamerasChanged();
}
