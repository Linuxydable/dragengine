/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeURuleLimitSetPosMax.h"
#include "../../../animator/rule/aeRuleLimit.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleLimitSetPosMax
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleLimitSetPosMax::aeURuleLimitSetPosMax( aeRuleLimit *rule, const decVector &newMax ){
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldMax = rule->GetMaximumPosition();
		pNewMax = newMax;
		
		SetShortInfo( "Limit Set maximum translation" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeURuleLimitSetPosMax::~aeURuleLimitSetPosMax(){
	pCleanUp();
}



// Management
///////////////

void aeURuleLimitSetPosMax::Undo(){
	pRule->SetMaximumPosition( pOldMax );
}

void aeURuleLimitSetPosMax::Redo(){
	pRule->SetMaximumPosition( pNewMax );
}



// Private Functions
//////////////////////

void aeURuleLimitSetPosMax::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
