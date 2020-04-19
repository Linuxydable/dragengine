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

#ifndef _GDEWPSSKINLISTENER_H_
#define _GDEWPSSKINLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSSkin;



/**
 * \brief Skin panel listener.
 */
class gdeWPSSkinListener : public gdeGameDefinitionListener{
private:
	gdeWPSSkin &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSSkinListener( gdeWPSSkin &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSSkinListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object class categories changed. */
	virtual void SkinCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class changed. */
	virtual void SkinChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin );
	
	/** \brief Active object class changed. */
	virtual void ActiveSkinChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
