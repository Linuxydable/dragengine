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
#ifndef _MEUHTVRULECOMPSETVECTOR_H_
#define _MEUHTVRULECOMPSETVECTOR_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRuleComponents;



/**
 * \brief Undo Height Terrain Vegetation Rule Components Set Vector.
 *
 * Undo action to set the vector of a height terrain vegetation rule components.
 */
class meUHTVRuleCompSetVector : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleComponents *pRule;
	
	decVector pOldVector;
	decVector pNewVector;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleCompSetVector( meHTVegetationLayer *vlayer, meHTVRuleComponents *rule, const decVector &nvector );
	/** \brief Clean up object. */
	virtual ~meUHTVRuleCompSetVector();
	/*@}*/
	
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
