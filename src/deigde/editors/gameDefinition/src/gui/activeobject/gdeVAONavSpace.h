/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEVAONAVSPACE_H_
#define _GDEVAONAVSPACE_H_

#include <dragengine/deObject.h>

class gdeViewActiveObject;
class gdeOCNavigationSpace;

class deDebugDrawer;
class igdeWDebugDrawerShape;



/**
 * \brief Active object view object class navigation space.
 */
class gdeVAONavSpace : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCNavigationSpace *pOCNavSpace;
	
	deDebugDrawer *pDDSpace;
	deDebugDrawer *pDDBlocker;
	igdeWDebugDrawerShape *pDDSSpace;
	igdeWDebugDrawerShape *pDDSBlocker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object component. */
	gdeVAONavSpace( gdeViewActiveObject &view, gdeOCNavigationSpace *occomponent );
	
protected:
	/**
	 * \brief Clean up active object component.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAONavSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class navigation space. */
	inline gdeOCNavigationSpace *GetOCNavSpace() const{ return pOCNavSpace; }
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	
	/** \brief Update debug drawer visibility. */
	void UpdateDDVisibility();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDebugDrawer();
	void pBuildDDSSpace();
	void pBuildDDSBlocker();
	void pUpdateDDShapeColor();
};

#endif
 
