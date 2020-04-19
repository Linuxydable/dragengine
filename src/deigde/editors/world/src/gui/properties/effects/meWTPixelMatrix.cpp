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

#if 0

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meWTPixelMatrix.h"
#include "dragengine/common/exceptions.h"



// Class meWTPixelMatrix
//////////////////////////

// Constructor, destructor
////////////////////////////

meWTPixelMatrix::meWTPixelMatrix( const char *name, const decTexMatrix &matrix ){
	pName = new char[ strlen( name ) + 1 ];
	if( ! pName ) DETHROW( deeOutOfMemory );
	strcpy( pName, name );
	
	pPixelMatrix = matrix;
}

meWTPixelMatrix::~meWTPixelMatrix(){
}



// Management
///////////////


void meWTPixelMatrix::SetPixelMatrix( const decTexMatrix &matrix ){
	pPixelMatrix = matrix;
}

#endif
