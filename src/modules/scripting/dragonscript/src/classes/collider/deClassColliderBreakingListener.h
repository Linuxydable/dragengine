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

#ifndef _DECLASSCOLLIDERBREAKINGLISTENER_H_
#define _DECLASSCOLLIDERBREAKINGLISTENER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Collider breaking listener script class.
 */
class deClassColliderBreakingListener : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassColliderBreakingListener( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassColliderBreakingListener();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** \brief Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Function index colliderConstraintBroke(). */
	inline int GetFuncIndexColliderConstraintBroke() const{ return pFuncIndexColliderConstraintBroke; }
	
	/** \brief Function index rigConstraintBroke(). */
	inline int GetFuncIndexRigConstraintBroke() const{ return pFuncIndexRigConstraintBroke; }
	/*@}*/
	
private:
	int pFuncIndexColliderConstraintBroke;
	int pFuncIndexRigConstraintBroke;
	
	struct sInitData{
		dsClass *clsCBL, *clsVoid, *clsBool, *clsFlt, *clsInt, *clsCol;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfColliderConstraintBroke );
	DEF_NATFUNC( nfRigConstraintBroke );
#undef DEF_NATFUNC
};

#endif
