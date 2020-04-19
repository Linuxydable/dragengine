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

#ifndef _DEOGLCACHES_H_
#define _DEOGLCACHES_H_

#include <dragengine/threading/deMutex.h>

class deCacheHelper;
class deGraphicOpenGl;



/**
 * \brief Caches.
 */
class deoglCaches{
private:
	deGraphicOpenGl &pOgl;
	deMutex pMutex;
	
	deCacheHelper *pSkinTextures;
	deCacheHelper *pModels;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create caches. */
	deoglCaches( deGraphicOpenGl &ogl );
	
	/** \brief Cleans up caches. */
	~deoglCaches();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Lock caches. */
	void Lock();
	
	/** \brief Unlock caches. */
	void Unlock();
	
	/** \brief Skin textures cache. */
	inline deCacheHelper &GetSkinTextures() const{ return *pSkinTextures; }
	
	/** \brief Model cache. */
	inline deCacheHelper &GetModels() const{ return *pModels; }
	
	
	
private:
	void pCleanUp();
};

#endif
