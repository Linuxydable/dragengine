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
#ifndef _MEUHTTEXSETTYPENUM_H_
#define _MEUHTTEXSETTYPENUM_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meHeightTerrainTexture;



/**
 * \brief Undo Set Height Terrain Texture Type Number.
 *
 * Undo action to set the type number of a height terrain texture.
 */
class meUHTTexSetTypeNum : public igdeUndo{
private:
	meHeightTerrainTexture *pTexture;
	
	int pOldTypeNumber;
	int pNewTypeNumber;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTTexSetTypeNum( meHeightTerrainTexture *texture, int newTypeNumber );
	
protected:
	/** \brief Clean up object. */
	virtual ~meUHTTexSetTypeNum();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif
