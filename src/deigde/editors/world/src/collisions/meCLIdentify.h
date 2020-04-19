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

// include only once
#ifndef _MECLIDENTIFY_H_
#define _MECLIDENTIFY_H_

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>



// predefinitions
class meWorld;
class meObject;
class meDecal;
class deCollider;



/**
 * Class for identifying an object.
 */
class meCLIdentify : public deBaseScriptingCollider{
private:
	meWorld *pWorld;
	meObject *pObject;
	meDecal *pDecal;
	
public:
	// constructor, destructor
	meCLIdentify( meWorld *world );
	virtual ~meCLIdentify();
	
	// management
	void Reset();
	inline meObject *GetObject() const{ return pObject; }
	inline meDecal *GetDecal() const{ return pDecal; }
	bool HasObject() const;
	bool HasDecal() const;
	
	// notifications
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	virtual void ColliderChanged( deCollider *owner );
	
private:
	meObject *pGetObjectForCollider( deCollider *collider ) const;
};

// end of include only once
#endif
