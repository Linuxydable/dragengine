/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeWConnection.h"
#include "../../igdeShapeBuilder.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeWConnection
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeWConnection::igdeWConnection() :
pColor( 0.0f, 0.0f, 0.0f ),
pArrowOnAnchor( false ),
pArrowOnTarget( true ),
pArrowSize( 0.05f ),
pArrowLength( 2.0f ),
pArrowRadius( 0.15f ),
pVisible( true )
{
	try{
		pDDSConnection.SetEdgeColor( pColor );
		pDDSConnection.SetFillColor( decColor( pColor.r, pColor.g, pColor.b, pColor.a * 0.2f ) );
		
		pDDSArrowAnchor.SetEdgeColor( pColor );
		pDDSArrowAnchor.SetFillColor( decColor( pColor.r, pColor.g, pColor.b, pColor.a * 0.2f ) );
		
		pDDSArrowTarget.SetEdgeColor( pColor );
		pDDSArrowTarget.SetFillColor( decColor( pColor.r, pColor.g, pColor.b, pColor.a * 0.2f ) );
		
		pRebuildArrow();
		pUpdateGeometries();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeWConnection::~igdeWConnection(){
	pCleanUp();
}



// Management
///////////////

void igdeWConnection::SetParentDebugDrawer( deDebugDrawer *debugDrawer ){
	pDDSConnection.SetParentDebugDrawer( debugDrawer );
	pDDSArrowAnchor.SetParentDebugDrawer( debugDrawer );
	pDDSArrowTarget.SetParentDebugDrawer( debugDrawer );
}



void igdeWConnection::SetAnchorPosition( const decVector &position ){
	if( position.IsEqualTo( pAnchorPosition ) ){
		return;
	}
	
	pAnchorPosition = position;
	pUpdateGeometries();
}

void igdeWConnection::SetTargetPosition( const decVector &position ){
	if( position.IsEqualTo( pTargetPosition ) ){
		return;
	}
	
	pTargetPosition = position;
	pUpdateGeometries();
}

void igdeWConnection::SetPositions( const decVector &anchorPosition, const decVector &targetPosition ){
	if( anchorPosition.IsEqualTo( pAnchorPosition ) && targetPosition.IsEqualTo( pTargetPosition ) ){
		return;
	}
	
	pAnchorPosition = anchorPosition;
	pTargetPosition = targetPosition;
	pUpdateGeometries();
}



void igdeWConnection::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	
	pDDSConnection.SetEdgeColor( color );
	pDDSConnection.SetFillColor( decColor( color.r, color.g, color.b, color.a * 0.2f ) );
	
	pDDSArrowAnchor.SetEdgeColor( color );
	pDDSArrowAnchor.SetFillColor( decColor( color.r, color.g, color.b, color.a * 0.2f ) );
	
	pDDSArrowTarget.SetEdgeColor( color );
	pDDSArrowTarget.SetFillColor( decColor( color.r, color.g, color.b, color.a * 0.2f ) );
}

void igdeWConnection::SetArrowOnAnchor( bool arrowOnAnchor ){
	if( arrowOnAnchor == pArrowOnAnchor ){
		return;
	}
	
	pArrowOnAnchor = arrowOnAnchor;
	pUpdateGeometries();
}

void igdeWConnection::SetArrowOnTarget( bool arrowOnTarget ){
	if( arrowOnTarget == pArrowOnTarget ){
		return;
	}
	
	pArrowOnTarget = arrowOnTarget;
	pUpdateGeometries();
}

void igdeWConnection::SetArrowSize( float size ){
	size = decMath::max( size, 0.001f );
	if( fabsf( size - pArrowSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pArrowSize = size;
	pRebuildArrow(); // because arrow radius is implicitely changed and this requires a rebuild
	pUpdateGeometries();
}

void igdeWConnection::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	pUpdateGeometries();
}



// Private Functions
//////////////////////

void igdeWConnection::pCleanUp(){
	SetParentDebugDrawer( NULL );
}

void igdeWConnection::pRebuildArrow(){
	igdeShapeBuilder builder;
	
	builder.CreateArrow( pDDSConnection, decVector( 0.0f, 0.0f, -0.5f ),
		decVector( 0.0f, 0.0f, 0.5f ), 0.5f, 1e-5f, 0.5f );
	
	builder.CreateArrowHead( pDDSArrowTarget, decVector( 0.0f, 0.0f, -1.0f ), decVector(), 0.5f );
	builder.CreateArrowHead( pDDSArrowAnchor, decVector( 0.0f, 0.0f, 1.0f ), decVector(), 0.5f );
}

void igdeWConnection::pUpdateGeometries(){
	const decVector direction = pTargetPosition - pAnchorPosition;
	float len = direction.Length();
	
	// calculate matrix
	decMatrix matrix;
	
	if( len < 1e-5f ){
		matrix.SetTranslation( pAnchorPosition );
		
	}else{
		const decVector dirNor = direction / len;
		
		if( fabsf( dirNor.y ) < 0.999f ){
			matrix.SetVU( dirNor, decVector( 0.0f, 1.0f, 0.0f ) );
			
		}else{
			matrix.SetVU( dirNor, decVector( 0.0f, 0.0f, 1.0f ) );
		}
	}
	
	// update geometry parameters
	const decQuaternion orientation = matrix.ToQuaternion();
	const float arrowLength = pArrowSize * pArrowLength * 0.5f;
	const float arrowSize = pArrowSize * pArrowRadius;
	
	pDDSArrowTarget.SetPosition( matrix.Transform( 0.0f, 0.0f, len ) );
	pDDSArrowTarget.SetOrientation( orientation );
	pDDSArrowTarget.SetScale( decVector( pArrowSize, pArrowSize, arrowLength ) );
	pDDSArrowTarget.SetVisible( pVisible && pArrowOnTarget );
	
	pDDSArrowAnchor.SetPosition( matrix.GetPosition() );
	pDDSArrowAnchor.SetOrientation( orientation );
	pDDSArrowAnchor.SetScale( decVector( pArrowSize, pArrowSize, arrowLength ) );
	pDDSArrowAnchor.SetVisible( pVisible && pArrowOnAnchor );
	
	float center = len * 0.5f;
	if( pArrowOnTarget ){
		len -= arrowLength;
		center -= arrowLength * 0.5f;
	}
	if( pArrowOnAnchor ){
		len -= arrowLength;
		center += arrowLength * 0.5f;
	}
	if( len < 1e-5f ){
		len = 1e-5f;
	}
	
	pDDSConnection.SetPosition( matrix.Transform( 0.0f, 0.0f, center ) );
	pDDSConnection.SetOrientation( orientation );
	pDDSConnection.SetScale( decVector( arrowSize, arrowSize, len ) );
	pDDSConnection.SetVisible( pVisible );
}
