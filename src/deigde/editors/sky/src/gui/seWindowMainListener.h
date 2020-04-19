/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEWINDOWMAINLISTENER_H_
#define _SEWINDOWMAINLISTENER_H_

#include "../sky/seSkyListener.h"

class seWindowMain;



/**
 * \brief Window main listener.
 */
class seWindowMainListener : public seSkyListener{
private:
	seWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWindowMainListener( seWindowMain &window );
	
	/** \brief Clean up listener. */
	virtual ~seWindowMainListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( seSky *sky );
	
	/** \brief Undo changed. */
	virtual void UndoChanged( seSky *sky );
	
	/** \brief The view changed. */
	virtual void ViewChanged( seSky *sky );
	/*@}*/
};

#endif
