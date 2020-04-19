/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DECLASSOCCLUSIONMESHBUILDER_H_
#define _DECLASSOCCLUSIONMESHBUILDER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief OcclusionMesh builder script class.
 */
class deClassOcclusionMeshBuilder : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassOcclusionMeshBuilder( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassOcclusionMeshBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsOcclusionMeshBuilder;
		dsClass *clsVoid;
		dsClass *clsBoolean;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsOcclusionMesh;
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsPoint;
		dsClass *clsVector2;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfBuild );
	DEF_NATFUNC( nfBuildOcclusionMesh );
	DEF_NATFUNC( nfSetBoneCount );
	DEF_NATFUNC( nfSetBoneAt );
	DEF_NATFUNC( nfSetWeightCount );
	DEF_NATFUNC( nfSetWeightAt );
	DEF_NATFUNC( nfSetWeightGroupCount );
	DEF_NATFUNC( nfSetWeightGroupAt );
	DEF_NATFUNC( nfSetVertexCount );
	DEF_NATFUNC( nfSetVertexAt );
	DEF_NATFUNC( nfSetCornerCount );
	DEF_NATFUNC( nfSetCornerAt );
	DEF_NATFUNC( nfSetFaceCount );
	DEF_NATFUNC( nfSetFaceAt );
	DEF_NATFUNC( nfSetDoubleSidedFaceCount );
#undef DEF_NATFUNC
};

#endif
