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

#ifndef _SEWPVIEW_H_
#define _SEWPVIEW_H_

#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCameraReference.h>
#include <deigde/gui/properties/igdeWPWObjectReference.h>

class seSky;
class seWindowProperties;
class seWPViewListener;



/**
 * \brief View panel.
 */
class seWPView : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seSky *pSky;
	seWPViewListener *pListener;
	
	igdeWPWObjectReference pWPEnvObject;
	igdeWPCameraReference pWPCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig. */
	seWPView( seWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up rig. */
	virtual ~seWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky( seSky *sky );
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
