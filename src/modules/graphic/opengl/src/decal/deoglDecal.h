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

#ifndef _DEOGLDECAL_H_
#define _DEOGLDECAL_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicDecal.h>

class deoglDynamicSkin;
class deoglRDecal;
class deoglComponent;

class deGraphicOpenGl;
class deDecal;



/**
 * \brief Decal peer.
 */
class deoglDecal : public deBaseGraphicDecal{
public:
	deGraphicOpenGl &pOgl;
	const deDecal &pDecal;
	
	deoglRDecal *pRDecal;
	
	deoglDynamicSkin *pDynamicSkin;
	
	bool pDirtyVBO;
	bool pDirtyGeometry;
	bool pDirtyTransform;
	bool pDirtySkin;
	bool pDirtyDynamicSkin;
	bool pDirtyVisibility;
	bool pDirtyParamBlocks;
	
	bool pDynamicSkinRequiresSync;
	
	deoglComponent *pParentComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglDecal( deGraphicOpenGl &ogl, const deDecal &decal );
	
	/** \brief Clean up peer. */
	virtual ~deoglDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Decal engine resource. */
	inline const deDecal &GetDecal() const{ return pDecal; }
	
	
	
	/** \brief Render decal. */
	inline deoglRDecal *GetRDecal() const{ return pRDecal; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Parent component or \em NULL. */
	inline deoglComponent *GetParentComponent() const{ return pParentComponent; }
	
	/** \brief Set parent component or \em NULL. */
	void SetParentComponent( deoglComponent *component );
	
	
	
	/** \brief Dynamic skin needs sync. */
	void DynamicSkinRequiresSync();
	
	/** \brief Drop dynamic skin because it is about to be deleted. */
	void DropDynamicSkin();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position, orientation or size changed. */
	virtual void GeometryChanged();
	
	/** \brief Texture coordinates transformation changed. */
	virtual void TransformChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** \brief Visible changed. */
	virtual void VisibleChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pSyncSkin();
	void pSyncDynamicSkin();
	
	void pRequiresSync();
};

#endif
