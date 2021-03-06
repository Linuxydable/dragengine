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
#include <string.h>
#include <stdlib.h>

#include "ceUCAPChoiceActionAdd.h"
#include "../ceUActionHelpers.h"
#include "../../../conversation/action/ceCAPlayerChoice.h"
#include "../../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAPChoiceActionAdd
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAPChoiceActionAdd::ceUCAPChoiceActionAdd( ceConversationTopic *topic, ceCAPlayerChoice *playerChoice,
ceCAPlayerChoiceOption *option, ceConversationAction *action, int index ){
	if( ! topic || ! playerChoice || ! action ){
		DETHROW( deeInvalidParam );
	}
	
	if( option ){
		if( index < 0 || index > option->GetActions().GetCount() ){
			DETHROW( deeInvalidParam );
		}
		
	}else{
		if( index < 0 || index > playerChoice->GetActions().GetCount() ){
			DETHROW( deeInvalidParam );
		}
	}
	
	pTopic = NULL;
	pPlayerChoice = NULL;
	pOption = NULL;
	pAction = NULL;
	pIndex = index;
	
	SetShortInfo( "Player Choice Add Action" );
	
	pTopic = topic;
	topic->AddReference();
	
	pPlayerChoice = playerChoice;
	playerChoice->AddReference();
	
	if( option ){
		pOption = option;
		option->AddReference();
	}
	
	pAction = action;
	action->AddReference();
}

ceUCAPChoiceActionAdd::~ceUCAPChoiceActionAdd(){
	if( pAction ){
		pAction->FreeReference();
	}
	if( pOption ){
		pOption->FreeReference();
	}
	if( pPlayerChoice ){
		pPlayerChoice->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAPChoiceActionAdd::Undo(){
	ceConversationAction *activateAction = NULL;
	
	if( pOption ){
		activateAction = ceUActionHelpers::ActivateActionAfterRemove( pOption->GetActions(), pAction );
		pOption->GetActions().Remove( pAction );
		
	}else{
		activateAction = ceUActionHelpers::ActivateActionAfterRemove( pPlayerChoice->GetActions(), pAction );
		pPlayerChoice->GetActions().Remove( pAction );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	
	pTopic->SetActive( activateAction ? activateAction : pPlayerChoice, NULL );
}

void ceUCAPChoiceActionAdd::Redo(){
	if( pOption ){
		pOption->GetActions().InsertAt( pAction, pIndex );
		
	}else{
		pPlayerChoice->GetActions().InsertAt( pAction, pIndex );
	}
	
	pTopic->NotifyActionStructureChanged( pPlayerChoice );
	pTopic->SetActive( pAction, NULL );
}
