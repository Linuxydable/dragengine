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

#include "gdeUOCRemoveSpeaker.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCRemoveSpeaker
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCRemoveSpeaker::gdeUOCRemoveSpeaker( gdeObjectClass *objectClass, gdeOCSpeaker *speaker ) :
pObjectClass( NULL ),
pSpeaker( NULL )
{
	if( ! objectClass || ! speaker ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! objectClass->GetSpeakers().Has( speaker ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove speaker" );
	
	pSpeaker = speaker;
	speaker->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCRemoveSpeaker::~gdeUOCRemoveSpeaker(){
	if( pSpeaker ){
		pSpeaker->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCRemoveSpeaker::Undo(){
	pObjectClass->GetSpeakers().Add( pSpeaker );
	pObjectClass->NotifySpeakersChanged();
}

void gdeUOCRemoveSpeaker::Redo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCSpeaker() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSpeaker ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCSpeaker( NULL );
	}
	
	pObjectClass->GetSpeakers().Remove( pSpeaker );
	pObjectClass->NotifySpeakersChanged();
}
