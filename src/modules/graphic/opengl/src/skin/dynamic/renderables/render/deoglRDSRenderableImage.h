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

#ifndef _DEOGLRRDSRENDERABLEIMAGE_H_
#define _DEOGLRRDSRENDERABLEIMAGE_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"

class deoglRImage;



/**
 * \brief Render dynamic skin image renderable.
 */
class deoglRDSRenderableImage : public deoglRDSRenderable{
private:
	deoglRImage *pImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render dynamic skin image renderable. */
	deoglRDSRenderableImage( deoglRDynamicSkin &dynamicSkin );
	
	/** \brief Clean up render dynamic skin image renderable. */
	virtual ~deoglRDSRenderableImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set image. */
	void SetImage( deoglRImage *image );
	
	/** \brief Prepare for render. */
	virtual void PrepareForRender();
	
	/**
	 * \brief Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
