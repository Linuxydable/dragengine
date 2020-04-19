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

#include "ceUCFileAdd.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"

#include <dragengine/common/exceptions.h>



// Class ceUCFileAdd
////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFileAdd::ceUCFileAdd( ceConversation *conversation, ceConversationFile *file ){
	if( ! conversation || ! file ) DETHROW( deeInvalidParam );
	
	pConversation = NULL;
	pFile = NULL;
	
	SetShortInfo( "Add File" );
	
	pConversation = conversation;
	conversation->AddReference();
	
	pFile = file;
	file->AddReference();
}

ceUCFileAdd::~ceUCFileAdd(){
	if( pFile ){
		pFile->FreeReference();
	}
	if( pConversation ){
		pConversation->FreeReference();
	}
}



// Management
///////////////

void ceUCFileAdd::Undo(){
	pConversation->RemoveFile( pFile );
}

void ceUCFileAdd::Redo(){
	pConversation->AddFile( pFile );
	pConversation->SetActiveFile( pFile );
}
