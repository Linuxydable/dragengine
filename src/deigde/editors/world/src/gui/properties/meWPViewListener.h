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

#ifndef _MEWPVIEWLISTENER_H_
#define _MEWPVIEWLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWPView;



/**
 * \brief View Panel Listener
 */
class meWPViewListener : public meWorldNotifier{
private:
	meWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPViewListener( meWPView &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPViewListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sky changed. */
	virtual void SkyChanged( meWorld *world );
	
	/** \brief Editing parameters changed. */
	virtual void EditingChanged( meWorld *world );
	
	/** \brief Trigger table changed. */
	virtual void TriggerTableChanged( meWorld *world );
	
	/** \brief Class (partial) hide tags changed. */
	virtual void ClassHideTagsChanged( meWorld *world );
	
	/** \brief Object changed. */
	virtual void ObjectChanged( meWorld *world, meObject *object );
	
	/** \brief Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	
	/** \brief Active camera changed. */
	virtual void ActiveCameraChanged( meWorld *world );
	
	/** \brief Camera changed. */
	virtual void CameraChanged( meWorld *world, meCamera *camera );
	/*@}*/
};

#endif
