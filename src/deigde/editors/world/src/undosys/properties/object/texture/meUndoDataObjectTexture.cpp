/* 
 * Drag[en]gine IGDE World Editor
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

#include "meUndoDataObjectTexture.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUndoDataObjectTexture
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUndoDataObjectTexture::meUndoDataObjectTexture( meObject *object ){
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	pOldTexture = NULL;
	pNewTexture = NULL;
	
	pObject = object;
	object->AddReference();
}

meUndoDataObjectTexture::~meUndoDataObjectTexture(){
	pNewTextureList.RemoveAllTextures();
	pOldTextureList.RemoveAllTextures();
	if( pNewTexture ){
		pNewTexture->FreeReference();
	}
	if( pOldTexture ){
		pOldTexture->FreeReference();
	}
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUndoDataObjectTexture::SetOldTexture( meObjectTexture *texture ){
	if( texture != pOldTexture ){
		if( pOldTexture ){
			pOldTexture->FreeReference();
		}
		pOldTexture = texture;
		if( texture ){
			texture->AddReference();
		}
	}
}

void meUndoDataObjectTexture::SetNewTexture( meObjectTexture *texture ){
	if( texture != pNewTexture ){
		if( pNewTexture ){
			pNewTexture->FreeReference();
		}
		pNewTexture = texture;
		if( texture ){
			texture->AddReference();
		}
	}
}
