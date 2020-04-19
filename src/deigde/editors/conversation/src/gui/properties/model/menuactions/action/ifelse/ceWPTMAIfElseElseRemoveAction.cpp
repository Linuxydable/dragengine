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

#include "ceWPTMAIfElseElseRemoveAction.h"
#include "../../../../ceWPTopic.h"
#include "../../../../ceWindowProperties.h"
#include "../../../../../ceWindowMain.h"
#include "../../../../../../conversation/ceConversation.h"
#include "../../../../../../conversation/action/ceCAIfElse.h"
#include "../../../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../../../undosys/action/ifElse/ceUCAIfElseRemove.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTMAIfElseElseRemoveAction::ceWPTMAIfElseElseRemoveAction( ceWindowMain &windowMain,
ceConversation &conversation, ceConversationTopic &topic,
ceCAIfElse &ifElse, ceConversationAction *action ) :
ceWPTMenuAction( windowMain, "Remove Action",
	windowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ) ),
pConversation( &conversation ),
pTopic( &topic ),
pIfElse( &ifElse ),
pAction( action )
{
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
}



// Management
///////////////

void ceWPTMAIfElseElseRemoveAction::OnAction(){
	igdeUndoReference undo;
	undo.TakeOver( new ceUCAIfElseRemove( pTopic, pIfElse, NULL, pAction ) );
	pConversation->GetUndoSystem()->Add( undo );
}
