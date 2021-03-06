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
#include <stdlib.h>
#include <string.h>

#include "gdeBaseMAOCSubObject.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeBaseMAOCSubObject
///////////////////////////////

// Constructor
////////////////

gdeBaseMAOCSubObject::gdeBaseMAOCSubObject( gdeWindowMain &windowMain,
	const char *text, igdeIcon *icon, const char *description ) :
gdeBaseAction( windowMain, text, icon, description ){
}



// Management
///////////////

igdeUndo *gdeBaseMAOCSubObject::OnAction( gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const objectClass = gameDefinition.GetActiveObjectClass();
	if( ! objectClass ){
		return NULL;
	}
	
	return OnActionSubObject( gameDefinition, *objectClass );
}

void gdeBaseMAOCSubObject::Update(){
	SetEnabled( GetActiveObjectClass() != NULL );
}

gdeObjectClass *gdeBaseMAOCSubObject::GetActiveObjectClass() const{
	const gdeGameDefinition * const gameDefinition = pWindowMain.GetActiveGameDefinition();
	if( ! gameDefinition ){
		return NULL;
	}
	
	switch( gameDefinition->GetSelectedObjectType() ){
	case gdeGameDefinition::eotObjectClass:
	case gdeGameDefinition::eotOCBillboard:
	case gdeGameDefinition::eotOCCamera:
	case gdeGameDefinition::eotOCComponent:
	case gdeGameDefinition::eotOCEnvMapProbe:
	case gdeGameDefinition::eotOCLight:
	case gdeGameDefinition::eotOCNavigationBlocker:
	case gdeGameDefinition::eotOCNavigationSpace:
	case gdeGameDefinition::eotOCParticleEmitter:
	case gdeGameDefinition::eotOCForceField:
	case gdeGameDefinition::eotOCSnapPoint:
	case gdeGameDefinition::eotOCSpeaker:
		return gameDefinition->GetActiveObjectClass();
		
	default:
		return NULL;
	}
}
