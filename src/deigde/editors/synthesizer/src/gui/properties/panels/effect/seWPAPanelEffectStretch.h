/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEWPAPANELEFFECTSTRETCH_H_
#define _SEWPAPANELEFFECTSTRETCH_H_

#include "seWPAPanelEffect.h"



/**
 * \brief Stretch time/pitch effect Panel.
 */
class seWPAPanelEffectStretch : public seWPAPanelEffect{
private:
	igdeTextFieldReference pEditMinTime;
	igdeTextFieldReference pEditMaxTime;
	igdeTextFieldReference pEditMinPitch;
	igdeTextFieldReference pEditMaxPitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creat panel. */
	seWPAPanelEffectStretch( seWPEffect &wpEffect );
	
protected:
	/** \brief Clean up panel. */
	virtual ~seWPAPanelEffectStretch();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update effect. */
	virtual void UpdateEffect();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	/*@}*/
};

#endif
