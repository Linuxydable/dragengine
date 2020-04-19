/* 
 * Drag[en]gine Game Engine
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
#include "deSkyLayerBody.h"
#include "../skin/deSkin.h"
#include "../../common/exceptions.h"



// Class deSkyLayerBody
/////////////////////////

// Constructor, destructor
////////////////////////////

deSkyLayerBody::deSkyLayerBody(){
	pSize.Set( DEG2RAD * 5.0f, DEG2RAD * 5.0f );
	pColor.Set( 1.0f, 1.0f, 1.0f, 1.0f );
}

deSkyLayerBody::~deSkyLayerBody(){
}



// Management
///////////////

void deSkyLayerBody::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deSkyLayerBody::SetSize( const decVector2 &size ){
	pSize = size;
}

void deSkyLayerBody::SetColor( const decColor &color ){
	pColor = color;
}

void deSkyLayerBody::SetSkin( deSkin *skin ){
	pSkin = skin;
}
