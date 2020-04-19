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

#ifndef _CECATRIGGER_H_
#define _CECATRIGGER_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Trigger Conversation Action.
 */
class ceCATrigger : public ceConversationAction{
public:
	/** \brief Action. */
	enum eActions{
		/** \brief Fire trigger. */
		eaFire,
		/** \brief Reset trigger. */
		eaReset,
		/** \brief Fire then immediately reset trigger. */
		eaPulse,
		/** \brief Full reset trigger. */
		eaFullReset,
		/** \brief Dummy, count of actions. */
		EA_COUNT
	};
	
private:
	decString pName;
	eActions pAction;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new trigger conversation action. */
	ceCATrigger();
	/** \brief Creates a new trigger conversation action. */
	ceCATrigger( const ceCATrigger &action );
	/** \brief Cleans up the trigger conversation action. */
	virtual ~ceCATrigger();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the name of the trigger. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Sets the name of the trigger. */
	void SetName( const char *name );
	/** \brief Retrieves the action. */
	inline eActions GetAction() const{ return pAction; }
	/** \brief Sets the action. */
	void SetAction( eActions action );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
