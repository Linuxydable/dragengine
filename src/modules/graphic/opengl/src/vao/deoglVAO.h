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

#ifndef _DEOGLVAO_H_
#define _DEOGLVAO_H_

#include "../deoglBasics.h"
#include "../vbo/deoglVBOLayout.h"

class deoglRenderThread;


/**
 * @brief OpenGL VAO.
 * A small wrapper for VAO objects to help with tracking VAO switches, storing indexed rendering
 * properties and speeding up render task creation.
 */
class deoglVAO{
public:
	deoglRenderThread &pRenderThread;
	
	GLuint pVAO;
	
	deoglVBOLayout::eIndexTypes pIndexType;
	int pIndexSize;
	int pIndexGLType;
	
	unsigned int pRenderTaskTrackingNumber;
	int pRenderTaskVAOIndex;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shared vbo. */
	deoglVAO( deoglRenderThread &renderThread );
	/** Cleans up the shared vbo. */
	virtual ~deoglVAO();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the VAO. */
	inline GLuint GetVAO() const{ return pVAO; }
	
	/** Retrieves the index type. */
	inline deoglVBOLayout::eIndexTypes GetIndexType() const{ return pIndexType; }
	/** Sets the index type. */
	void SetIndexType( deoglVBOLayout::eIndexTypes indexType );
	/** Retrieves the size in bytes of indices. */
	inline int GetIndexSize() const{ return pIndexSize; }
	/** Retrieves the opengl type of the indices. */
	inline int GetIndexGLType() const{ return pIndexGLType; }
	
	/** Retrieves the render task tracking number. */
	inline unsigned int GetRenderTaskTrackingNumber() const{ return pRenderTaskTrackingNumber; }
	/** Sets the render task tracking number. */
	void SetRenderTaskTrackingNumber( unsigned int trackingNumber );
	/** Retrieves the render task vao index. */
	inline int GetRenderTaskVAOIndex() const{ return pRenderTaskVAOIndex; }
	/** Sets the render task vao index. */
	void SetRenderTaskVAOIndex( int vaoIndex );
	/*@}*/
};

#endif
