/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REBASEUNDOMOVE_H_
#define _REBASEUNDOMOVE_H_

#include <dragengine/common/math/decMath.h>
#include <deigde/undo/igdeUndo.h>



/**
 * \brief Base class for undo action moving things around.
 */
class reBaseUndoMove : public igdeUndo{
private:
	decVector pDist;
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reBaseUndoMove();
	
protected:
	virtual ~reBaseUndoMove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Distance. */
	inline decVector GetDistance() const{ return pDist; }
	
	/** \brief Set distance. */
	void SetDistance( const decVector &distance );
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
