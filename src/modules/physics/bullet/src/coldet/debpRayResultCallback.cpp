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

#include <stdio.h>
#include <stdlib.h>

#include "debpRayResultCallback.h"
#include "../debpCollisionObject.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpRayResultCallback
////////////////////////////////

// Constructors
/////////////////

debpRayResultCallback::debpRayResultCallback( deCollisionInfo *colinfo ){
	if( ! colinfo ){
		DETHROW( deeInvalidParam );
	}
	
	pColInfo = colinfo;
	
	pCollisionFilter = NULL;
	pListener = NULL;
}



// Management
///////////////

void debpRayResultCallback::Reset(){
}



void debpRayResultCallback::SetTestRay( const decDVector &rayOrigin, const decDVector &rayDirection,
	const decCollisionFilter *collisionFilter, deBaseScriptingCollider *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pRayOrigin = rayOrigin;
	pRayDirection = rayDirection;
	pCollisionFilter = collisionFilter;
	pListener = listener;
}



// Bullet
///////////

bool debpRayResultCallback::needsCollision( btBroadphaseProxy *proxy0 ) const{
	// basic bullet filtering
	if( ! RayResultCallback::needsCollision( proxy0 ) ){
		return false;
	}
	
	// determine the collision partner using the custom pointer
	const btCollisionObject &collisionObject = *( ( btCollisionObject* )proxy0->m_clientObject );
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )collisionObject.getUserPointer() );
	
	// test against a collider
	if( colObj.IsOwnerCollider() ){
		debpCollider * const collider = colObj.GetOwnerCollider();
		
		// check if a collision is possible according to layer mask
		deCollider * const engCollider = &collider->GetCollider();
		if( pCollisionFilter && pCollisionFilter->CollidesNot( engCollider->GetCollisionFilter() ) ){
			return false;
		}
		
		// check if a collision is possible according to the collider listener
		if( ! pListener->CanHitCollider( NULL, engCollider ) ){
			return false;
		}
		//printf( "needsCollision %p\n", colObj.GetOwnerCollider() );
		return true;
		
	// test against a height terrain sector
	}else if( colObj.IsOwnerHTSector() ){
		if( pCollisionFilter && pCollisionFilter->CollidesNot( colObj.GetOwnerHTSector()->
		GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter() ) ){
			return false;
		}
		
		return true;
	}
	
	// all other combinations score no collision
	return false;
}

btScalar debpRayResultCallback::addSingleResult( btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace ){
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )rayResult.m_collisionObject->getUserPointer() );
	bool callListener = false;
	
	if( colObj.IsOwnerCollider() ){
		pColInfo->SetCollider( &colObj.GetOwnerCollider()->GetCollider(), colObj.GetOwnerBone(),
			( int )( intptr_t )rayResult.m_collisionShape->getUserPointer() - 1,
			-1 /* convexResult.m_localShapeInfo->m_triangleIndex; // problem... bullet index not our index */ );
		callListener = true;
		
	}else if( colObj.IsOwnerHTSector() ){
		const debpHTSector &htsector = *colObj.GetOwnerHTSector();
		pColInfo->SetHTSector( htsector.GetHeightTerrain()->GetHeightTerrain(), htsector.GetSector() );
		callListener = true;
	}
	
	if( callListener ){
		btVector3 hitNormal;
		if( normalInWorldSpace ){
			hitNormal = rayResult.m_hitNormalLocal;
			
		}else{
			hitNormal = rayResult.m_collisionObject->getWorldTransform().getBasis() * rayResult.m_hitNormalLocal;
		}
		
		pColInfo->SetDistance( ( float )rayResult.m_hitFraction );
		pColInfo->SetNormal( decVector( ( float )hitNormal.x(), ( float )hitNormal.y(), ( float )hitNormal.z() ) );
		
		pListener->CollisionResponse( NULL, pColInfo );
	}
	
	// bullet uses m_closestHitFraction after addSingleResult returns to determine if
	// further collision tests are required. so we hijack the variable for the purpose
	// of stopping collision testing if the game requests.
	if( pColInfo->GetStopTesting() ){
		m_closestHitFraction = ( btScalar )0.0f;
	}
	
	return m_closestHitFraction; // no idea what the return value does. other implementations use m_closestHitFraction
}
