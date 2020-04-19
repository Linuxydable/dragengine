/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CECACOORDSYSTEMREMOVE_H_
#define _CECACOORDSYSTEMREMOVE_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Remove coordinate system conversation action.
 * \details Removes a coordinate system from the conversation if existing.
 */
class ceCACoordSystemRemove : public ceConversationAction{
private:
	decString pCoordSystemID;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCACoordSystemRemove();
	/** \brief Creates a new conversation action. */
	ceCACoordSystemRemove( const ceCACoordSystemRemove &action );
	/** \brief Cleans up the conversation action. */
	virtual ~ceCACoordSystemRemove();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the coordinate system id. */
	inline const decString &GetCoordSystemID() const{ return pCoordSystemID; }
	/** \brief Sets the coordinate system id. */
	void SetCoordSystemID( const char *id );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
