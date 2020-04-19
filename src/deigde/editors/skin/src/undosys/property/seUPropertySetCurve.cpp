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
#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "seUPropertySetCurve.h"
#include "../../sky/layer/seSkyLayer.h"
#include "../../sky/property/seProperty.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertySetCurve
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertySetCurve::seUPropertySetCurve( seSkyLayer *layer, seProperty *property ){
	if( ! layer || ! property ) DETHROW( deeInvalidParam );
	
	pLayer = NULL;
	pProperty = NULL;
	
	SetShortInfo( "Property Set Curve" );
	
	pOldCurve = property->GetCurve();
	pNewCurve = pOldCurve;
	
	pLayer = layer;
	layer->AddReference();
	pProperty = property;
}

seUPropertySetCurve::~seUPropertySetCurve(){
	if( pLayer ) pLayer->FreeReference();
}



// Management
///////////////

void seUPropertySetCurve::SetNewCurve( const decCurveBezier &curve ){
	pNewCurve = curve;
}



void seUPropertySetCurve::Undo(){
	pProperty->GetCurve() = pOldCurve;
	pProperty->NotifyPropertyChanged();
}

void seUPropertySetCurve::Redo(){
	pProperty->GetCurve() = pNewCurve;
	pProperty->NotifyPropertyChanged();
}
#endif