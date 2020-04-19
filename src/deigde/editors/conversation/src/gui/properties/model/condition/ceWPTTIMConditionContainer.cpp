/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceWPTTIMCondition.h"
#include "ceWPTTIMConditions.h"
#include "ceWPTTIMConditionContainer.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/condition/ceConversationCondition.h"
#include "../../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMConditionContainer::ceWPTTIMConditionContainer(
ceWindowMain &windowMain, ceConversation &conversation, ceConversationAction &action, eTypes type ) :
ceWPTTreeItemModel( windowMain, conversation, type ),
pAction( &action ),
pChildCondition( NULL ){
}

ceWPTTIMConditionContainer::~ceWPTTIMConditionContainer(){
}



// Management
///////////////

void ceWPTTIMConditionContainer::SetCondition( ceConversationCondition *condition ){
	if( pCondition == condition ){
		return;
	}
	
	if( pChildCondition ){
		RemoveChild( pChildCondition );
		pChildCondition = NULL;
	}
	
	pCondition = condition;
	
	if( ! condition ){
		return;
	}
	
	ceWPTTIMCondition *child = NULL;
	try{
		child = ceWPTTIMConditions::CreateConditionModel( GetWindowMain(),
			GetConversation(), GetAction(), condition );
		AddChild( child );
		pChildCondition = child;
		child->FreeReference(); // held by superclass child list
		
	}catch( const deException & ){
		if( child ){
			child->FreeReference();
		}
		throw;
	}
}

void ceWPTTIMConditionContainer::SetOrUpdateCondition( ceConversationCondition *condition ){
	if( pCondition == condition ){
		if( pChildCondition ){
			pChildCondition->Update();
		}
		
	}else{
		SetCondition( condition );
	}
}



ceWPTTIMCondition *ceWPTTIMConditionContainer::DeepFindCondition( ceConversationCondition *condition ){
	if( pChildCondition ){
		return pChildCondition->DeepFindCondition( condition );
		
	}else{
		return NULL;
	}
}
