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

#include "meUObjectShapeReplace.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>

#include <deigde/codec/igdeCodecPropertyString.h>



// Class meUObjectShapeReplace
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeReplace::meUObjectShapeReplace( meObject *object, const char *property, int shapeIndex, const decShape &shape ){
	if( ! object || ! property ){
		DETHROW( deeInvalidParam );
	}
	if( ! object->GetWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeCodecPropertyString codec;
	decShape *copyShape = NULL;
	decShapeList shapeList;
	
	pObject = NULL;
	
	SetShortInfo( "Object-Shape replace" );
	SetLongInfo( "Object-Shape replace" );
	
	pPropertyExists = object->GetProperties().Has( property );
	if( pPropertyExists ){
		pOldValue = object->GetProperties().GetAt( property );
	}
	
	codec.DecodeShapeList( pOldValue.GetString(), shapeList );
	
	try{
		copyShape = shape.Copy();
		shapeList.SetAt( shapeIndex, copyShape );
		
	}catch( const deException & ){
		if( copyShape ){
			delete copyShape;
		}
		throw;
	}
	
	codec.EncodeShapeList( shapeList, pNewValue );
	
	pProperty = property;
	pObject = object;
	object->AddReference();
}

meUObjectShapeReplace::~meUObjectShapeReplace(){
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectShapeReplace::Undo(){
	if( pPropertyExists ){
		pObject->SetProperty( pProperty.GetString(), pOldValue.GetString() );
		
	}else{
		pObject->RemoveProperty( pProperty.GetString() );
	}
}

void meUObjectShapeReplace::Redo(){
	pObject->SetProperty( pProperty.GetString(), pNewValue.GetString() );
}
