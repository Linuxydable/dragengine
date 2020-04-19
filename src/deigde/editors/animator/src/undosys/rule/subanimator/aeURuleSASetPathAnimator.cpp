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

#include "aeURuleSASetPathAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleSubAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleSASetPathAnimator
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleSASetPathAnimator::aeURuleSASetPathAnimator( aeRuleSubAnimator *rule, const char *newPath ) :
pRule( NULL ),
pNewPath( newPath )
{
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pOldPath = rule->GetPathSubAnimator();
	
	const int count = rule->GetConnectionCount();
	int i;
	for( i=0; i<count; i++ ){
		pOldConnections.Add( rule->GetControllerAt( i ) );
	}
	
	SetShortInfo( "Sub-Animator: Set animator path" );
	
	pRule = rule;
	pRule->AddReference();
}

aeURuleSASetPathAnimator::~aeURuleSASetPathAnimator(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeURuleSASetPathAnimator::Undo(){
	pRule->SetPathSubAnimator( pOldPath );
	pRule->LoadSubAnimator();
	
	const int count = pOldConnections.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pRule->SetControllerAt( i, ( aeController* )pOldConnections.GetAt( i ) );
	}
}

void aeURuleSASetPathAnimator::Redo(){
	pRule->SetPathSubAnimator( pNewPath );
	pRule->LoadSubAnimator();
}
