/* 
 * Drag[en]gine Bullet Physics Module
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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "debpDCollisionVolume.h"



// Collision Detection Volume
///////////////////////////////

// Constructors, destructors
//////////////////////////////

debpDCollisionVolume::debpDCollisionVolume(){
}

debpDCollisionVolume::~debpDCollisionVolume(){
}



// Miscelanous Functions
//////////////////////////

decDVector debpDCollisionVolume::NormalAtPoint( const decDVector &point ){
	return decDVector( 1.0f, 0.0f, 0.0f );
}

bool debpDCollisionVolume::RayHitsVolume( const decDVector &rayOrigin, const decDVector &rayDirection, double &hitDistance ){
	return false;
}
