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

#ifndef _CEACTORGESTURE_H_
#define _CEACTORGESTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class igdeEnvironment;
class deAnimator;



/**
 * \brief Conversation actor gesture.
 */
class ceActorGesture : public deObject{
private:
	igdeEnvironment &pEnvironment;
	deAnimator *pEngAnimator;
	
	decString pName;
	decString pPathAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create actor gesture. */
	ceActorGesture( igdeEnvironment &environment, const char *name );
	
	/** \brief Create copy of actor gesture. */
	ceActorGesture( const ceActorGesture &gesture );
	
protected:
	/** \brief Clean up actor gesture. */
	virtual ~ceActorGesture();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Engine animator or \em NULL. */
	inline deAnimator *GetEngineAnimator() const{ return pEngAnimator; }
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Animator path. */
	inline const decString &GetPathAnimator() const{ return pPathAnimator; }
	
	/** \brief Set animator path. */
	void SetPathAnimator( const char *path );
	/*@}*/
	
	
	
private:
	void pLoadAnimator();
};

#endif
