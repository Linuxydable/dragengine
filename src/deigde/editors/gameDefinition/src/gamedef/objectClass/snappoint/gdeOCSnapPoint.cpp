/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCSnapPoint.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCSnapPoint
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSnapPoint::gdeOCSnapPoint() :
pSnapDistance( 0.1f ),
pSnapToRotation( true ){
}

gdeOCSnapPoint::gdeOCSnapPoint( const gdeOCSnapPoint &snapPoint ) :
pName( snapPoint.pName ),

pPosition( snapPoint.pPosition ),
pRotation( snapPoint.pRotation ),

pSnapDistance( snapPoint.pSnapDistance ),
pSnapToRotation( snapPoint.pSnapToRotation ){
}

gdeOCSnapPoint::~gdeOCSnapPoint(){
}



// Management
///////////////

void gdeOCSnapPoint::SetName( const char *name ){
	pName = name;
}



void gdeOCSnapPoint::SetPosition( const decDVector &position ){
	pPosition = position;
}

void gdeOCSnapPoint::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}



void gdeOCSnapPoint::SetSnapDistance( float distance ){
	pSnapDistance = decMath::max( distance, 0.0f );
}

void gdeOCSnapPoint::SetSnapToRotation( bool snapToRotation ){
	pSnapToRotation = snapToRotation;
}
