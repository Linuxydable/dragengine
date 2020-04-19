/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stdlib.h>
#include <stdint.h>

#include "deoglConvexHull2D.h"

#include <dragengine/common/exceptions.h>



// Class deoglConvexHull2D
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexHull2D::deoglConvexHull2D(){
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
	
	pHullPoints = NULL;
	pHullPointCount = 0;
	pHullPointSize = 0;
}

deoglConvexHull2D::~deoglConvexHull2D(){
	if( pHullPoints ){
		delete [] pHullPoints;
	}
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector2 &deoglConvexHull2D::GetPointAt( int index ) const{
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPoints[ index ];
}

void deoglConvexHull2D::AddPoint( const decVector2 &point ){
	if( pPointCount == pPointSize ){
		const int newSize = pPointSize + 10;
		decVector2 * const newArray = new decVector2[ newSize ];
		
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( decVector2 ) * pPointCount );
			delete [] pPoints;
		}
		pPoints = newArray;
		
		pPointSize = newSize;
	}
	
	pPoints[ pPointCount++ ] = point;
}

void deoglConvexHull2D::AddPoint( float x, float y ){
	if( pPointCount == pPointSize ){
		const int newSize = pPointSize + 10;
		decVector2 * const newArray = new decVector2[ newSize ];
		
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( decVector2 ) * pPointCount );
			delete [] pPoints;
		}
		pPoints = newArray;
		
		pPointSize = newSize;
	}
	
	pPoints[ pPointCount++ ].Set( x, y );
}

void deoglConvexHull2D::RemoveAllPoints(){
	pPointCount = 0;
}



int deoglConvexHull2D::GetHullPointAt( int index ) const{
	if( index < 0 || index >= pHullPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pHullPoints[ index ];
}

const decVector2 &deoglConvexHull2D::GetHullPointVectorAt( int index ) const{
	if( index < 0 || index >= pHullPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPoints[ pHullPoints[ index ] ];
}



void deoglConvexHull2D::CalculateHull(){
	if( pHullPointSize < pPointSize ){
		int * const newArray = new int[ pPointSize ];
		
		if( pHullPoints ){
			delete [] pHullPoints;
		}
		pHullPoints = newArray;
		
		pHullPointSize = pPointSize;
		
	}
	
	pHullPointCount = 0;
	
	if( pPointCount > 2 ){
		int exchange, testIndex;
		int i, j, bestIndex;
		decVector2 edge;
		float bestValue;
		
		// init the hull points list
		for( i=0; i<pPointCount; i++ ){
			pHullPoints[ i ] = i;
		}
		
		// find the starting point. this is the point with the smallest x coordinate. this point is
		// added to the convex hull and moved to the front position to avoid testing it again
		bestValue = pPoints[ 0 ].x;
		bestIndex = 0;
		
		for( i=1; i<pPointCount; i++ ){
			if( pPoints[ i ].x < bestValue ){
				bestValue = pPoints[ i ].x;
				bestIndex = i;
			}
		}
		
		// find the rest of the hull points. for each loop the point is searched for which all other
		// points are on the right side of the line running from the last found point to the point in
		// question. for testing the sideness of points the triangle area calculation is
		// used. every found point is added to the hull points and moved to the location of the first
		// remaining point. this way they are not tested again in the coming loops. the test stops
		// if there is no more point with all points on the right side. this is the case for the last
		// point in the hull before closing up with the first point
		
		// the loop looks a bit strange the but idea is the following. if the loop variable i is equal
		// to pPointCount then we found no point anymore having all other points on the right side.
		// instead of using an if-condition to either update the hull list or to break from the loop at
		// the end of the main loop we can move this all at the beginning and into the for-condition.
		// to get this working the variable i is set to bestIndex for the first run of the loop. this
		// way the moving of the initial point to the start of the list is done before the first
		// testing is conducted. furthermore exchanges happen only if in the previous loop a new point
		// has been found. it is a sort of delayed loop
		for( pHullPointCount=0, i=bestIndex; i<pPointCount; pHullPointCount++ ){
			exchange = pHullPoints[ pHullPointCount ];
			pHullPoints[ pHullPointCount ] = pHullPoints[ i ];
			pHullPoints[ i ] = exchange;
			
			const decVector2 &lineStart = pPoints[ pHullPoints[ pHullPointCount ] ];
			
			for( i=pHullPointCount+1; i<pPointCount; i++ ){
				// for the calculation of the triangle area the following fomula can be used:
				// 0.5 * ( ( bx - ax )*( cy - ay ) - ( cx - ax )*( by - ay ) ) . for testing the sideness
				// the scaling by 0.5 is not important. this gives the simpler version:
				// ( bx - ax )*( cy - ay ) - ( cx - ax )*( by - ay ). this can be further converted to a
				// ( direct if-condition test (a point on the left side is positive, fails the test):
				// ( bx - ax )*( cy - ay ) > ( cx - ax )*( by - ay ). furthermore the lineStart and lineEnd
				// relative only value can be pre-computed for each loop
				
				testIndex = pHullPoints[ i ];
				
				const decVector2 &lineEnd = pPoints[ testIndex ];
				
				edge = lineEnd - lineStart;
				
				for( j=0; j<pPointCount; j++ ){
					if( j == testIndex ){
						continue;
					}
					
					const decVector2 &lineTest = pPoints[ j ];
					
					if( ( lineTest.y - lineStart.y ) * edge.x > ( lineTest.x - lineStart.x ) * edge.y ){
						break;
					}
				}
				
				if( j == pPointCount ){
					break;
				}
			}
		}
	}
}
