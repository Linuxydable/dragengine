/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEAIDEVICEBUTTON_H_
#define _DEAIDEVICEBUTTON_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>

class deInputDeviceButton;



/**
 * \brief Android input device button.
 */
class deaiDeviceButton{
private:
	decString pID;
	decString pName;
	bool pPressed;
	
	int pAICode;
	int pAIChar;
	deInputEvent::eKeyCodes pKeyCode;
	int pMatchPriority;
	
	bool pDirtyValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device button. */
	deaiDeviceButton();
	
	/** \brief Clean up device button. */
	~deaiDeviceButton();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** \brief Set if button is presssed. */
	void SetPressed( bool pressed );
	
	
	
	/** \brief Android specific code. */
	inline int GetAICode() const{ return pAICode; }
	
	/** \brief Set Android specific code. */
	void SetAICode( int code );
	
	/** \brief Android specific character. */
	inline int GetAIChar() const{ return pAIChar; }
	
	/** \brief Set Android specific character. */
	void SetAIChar( int character );
	
	/** \brief Input event key code. */
	inline deInputEvent::eKeyCodes GetKeyCode() const{ return pKeyCode; }
	
	/** \brief Set input event key code. */
	void SetKeyCode( deInputEvent::eKeyCodes keyCode );
	
	/** \brief Match priority. */
	inline int GetMatchPriority() const{ return pMatchPriority; }
	
	/** \brief Set match priority. */
	void SetMatchPriority( int priority );
	
	
	
	/** \brief Update engine input device information button. */
	void GetInfo( deInputDeviceButton &info ) const;
	/*@}*/
};

#endif
