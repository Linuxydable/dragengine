/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seUPropertyNodeSetCombineMode.h"
#include "../../../skin/property/node/sePropertyNode.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodeSetCombineMode
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeSetCombineMode::seUPropertyNodeSetCombineMode(
	sePropertyNode *node, deSkinPropertyNode::eCombineModes newValue ) :
pNode( NULL ),
pNewValue( newValue )
{
	if( ! node || ! node->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Node set combine mode" );
	
	pOldValue = node->GetCombineMode();
	
	pNode = node;
	node->AddReference();
}

seUPropertyNodeSetCombineMode::~seUPropertyNodeSetCombineMode(){
	if( pNode ){
		pNode->FreeReference();
	}
}



// Management
///////////////

void seUPropertyNodeSetCombineMode::Undo(){
	pNode->SetCombineMode( pOldValue );
}

void seUPropertyNodeSetCombineMode::Redo(){
	pNode->SetCombineMode( pNewValue );
}
