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

#ifndef _DEOGLRENDERLIGHTBASE_H_
#define _DEOGLRENDERLIGHTBASE_H_

#include "../deoglRenderBase.h"
#include "../../collidelist/deoglCollideList.h"

class deoglComponentList;
class deoglRenderPlan;



/**
 * \brief Base class for light renderer classes.
 */
class deoglRenderLightBase : public deoglRenderBase{
private:
	deoglCollideList pColList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderLightBase( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderLightBase();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Collider list. */
	inline deoglCollideList &GetCollideList(){ return pColList; }
	inline const deoglCollideList &GetCollideList() const{ return pColList; }
	
	/** \brief Populate collider list with components from a component list. */
	void AddComponentsToColliderList( const deoglComponentList &list );
	
	/**
	 * \brief Update VBOs for all components in the collide list.
	 * 
	 * \warning After this call the active FBO, OpengGL states and other rendering configuration
	 *          like viewport and scissoring can be undefined. Do not call after setting up
	 *          rendering parameters.
	 */
	void UpdateComponentVBO( const deoglCollideList &list );
	
	/**
	 * \brief Update renderables of all components in the collide list.
	 * 
	 * \warning After this call the active FBO, OpengGL states and other rendering configuration
	 *          like viewport and scissoring can be undefined. Do not call after setting up
	 *          rendering parameters.
	 */
	void UpdateComponentRenderables( deoglRenderPlan &plan, const deoglCollideList &list );
	
	/** \brief Transform color by matrix. */
	decColor TransformColor( const decMatrix &matrix, const decColor &color ) const;
	
	/** \brief Restore frame buffer configuration. */
	void RestoreFBO( deoglRenderPlan &plan );
	
	/** \brief Restore texture bindings from the deferred rendering pass using linear interpolation. */
	void RestoreDRTexturesSmooth();
	
	/** \brief Restore depth texture binding from the deferred rendering pass using linear interpolation. */
	void RestoreDRTextureDepthSmooth();
	
	/** \brief Restore render fbo including attachments. */
	void RestoreFBO();
	
	
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoSolid();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoTransparent();
	/*@}*/
};

#endif
