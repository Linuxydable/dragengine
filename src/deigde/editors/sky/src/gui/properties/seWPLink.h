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

#ifndef _SEWPLINK_H_
#define _SEWPLINK_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/event/igdeActionReference.h>

class seSky;
class seLink;
class seWindowProperties;
class seWPLinkListener;

class igdeViewCurveBezier;


/**
 * \brief Link Panel.
 */
class seWPLink : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seWPLinkListener *pListener;
	
	seSky *pSky;
	
	igdeActionReference pActionLinkAdd;
	igdeActionReference pActionLinkRemove;
	
	igdeListBoxReference pListLinks;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBController;
	igdeViewCurveBezierReference pEditCurve;
	igdeTextFieldReference pEditRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPLink( seWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky( seSky *sky );
	
	/** \brief Active link or \em NULL. */
	seLink *GetLink() const;
	
	/** \brief Update link list. */
	void UpdateLinkList();
	
	/** \brief Change selection to active link. */
	void SelectActiveLink();
	
	/** \brief Update link parameters. */
	void UpdateLink();
	
	/** \brief Update list of controller. */
	void UpdateControllerList();
	
	/** \brief Actions. */
	inline igdeAction *GetActionLinkAdd() const{ return pActionLinkAdd; }
	inline igdeAction *GetActionLinkRemove() const{ return pActionLinkRemove; }
	/*@}*/
};

#endif
