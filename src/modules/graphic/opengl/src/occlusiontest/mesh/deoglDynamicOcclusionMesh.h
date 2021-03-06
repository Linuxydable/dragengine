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

#ifndef _DEOGLDYNAMICOCCLUSIONMESH_H_
#define _DEOGLDYNAMICOCCLUSIONMESH_H_

#include "../../deoglBasics.h"

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>

class deoglRComponent;
class deoglROcclusionMesh;
class deoglSharedVBOBlock;
class deoglRenderThread;
class deoglVAO;

class deComponent;


/**
 * @brief Occlusion Dynamic Mesh.
 */
class deoglDynamicOcclusionMesh{
public:
	deoglRenderThread &pRenderThread;
	deoglROcclusionMesh *pOcclusionMesh;
	deoglRComponent *pComponent;
	
	decIntList pBoneMappings;
	
	oglMatrix3x4 *pWeights;
	int pWeightCount;
	
	decVector *pVertices;
	
	GLuint pVBO;
	deoglVAO *pVAO;
	deoglVBOp *pVBOData;
	int pVBOVertexCount;
	int pVBOVertexSize;
	
	bool pDirtyOccMesh;
	bool pDirtyVBO;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dynamic occlusion mesh object. */
	deoglDynamicOcclusionMesh( deoglRenderThread &renderThread,
		deoglROcclusionMesh *occlusionMesh, deoglRComponent *component );
	
	/** Cleans up the peer. */
	~deoglDynamicOcclusionMesh();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	/** Retrieves the occlusion mesh. */
	inline deoglROcclusionMesh *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** Retrives the vbo. */
	inline GLuint GetVBO() const{ return pVBO; }
	/** Retrieves the VAO. */
	deoglVAO *GetVAO();
	/** Invalidates the VAO. */
	void InvalidateVAO();
	
	/** Component state changed. */
	void ComponentStateChanged();
	
	/** \brief Update bone mappings. */
	void UpdateBoneMappings( const deComponent &component );
	
	/** Prepare for rendering. */
	void Prepare();
	/*@}*/
	
private:
	void pCleanUp();
	void pBuildArrays();
	void pCalculateWeights();
	void pTransformVertices();
	void pWriteVBOData();
	void pBuildVBO();
	void pUpdateVAO();
};

#endif
