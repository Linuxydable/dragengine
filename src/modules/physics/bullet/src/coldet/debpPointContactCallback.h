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

#ifndef _DEBPPOINTCONTACTCALLBACK_H_
#define _DEBPPOINTCONTACTCALLBACK_H_

#include "debpCollisionDetection.h"

#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class decCollisionFilter;
class deCollisionInfo;
class deBaseScriptingCollider;
class debpShape;
class debpCollider;



/**
 * \brief Point contact callback.
 */
class debpPointContactCallback : public btBroadphaseAabbCallback{
private:
	const btVector3 pPoint;
	const decCollisionFilter &pCollisionFilter;
	deBaseScriptingCollider &pListener;
	debpCollisionDetection &pColDet;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new callback. */
	debpPointContactCallback( const btVector3 &point, const decCollisionFilter &collisionFilter,
		deBaseScriptingCollider &listener, debpCollisionDetection &colDet );
	/*@}*/
	
	
	
	/** \name Bullet */
	/*@{*/
	/** \brief Process a collision proxy. */
	virtual bool process( const btBroadphaseProxy *proxy );
	/*@}*/
};

#endif
