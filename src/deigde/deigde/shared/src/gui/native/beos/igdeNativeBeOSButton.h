/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENATIVEBEOSBUTTON_H_
#define _IGDENATIVEBEOSBUTTON_H_

#include "beostoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeButton;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * BeOS Native button.
 */
class igdeNativeBeOSButton : public BButton{
private:
	enum eEvents{
		eeClicked = 'clic'
	};
	
	igdeButton *pOwner;
	igdeFontReference pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeOSButton( igdeButton &owner, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeBeOSButton();
	
	/** \brief Create native widget. */
	static igdeNativeBeOSButton* CreateNativeWidget( igdeButton &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void Focus();
	virtual void UpdateStyle();
	virtual void UpdateText();
	virtual void UpdateDescription();
	virtual void UpdateIcon();
	virtual void UpdateEnabled();
	
	virtual void MessageReceived( BMessage *message );
	void UpdateAction();
	
	static const char *ButtonText( const igdeButton &owner );
	static BBitmap *ButtonIcon( const igdeButton &owner );
	static igdeFont *ButtonFont( const igdeButton &owner, const igdeGuiTheme &guitheme );
	static int ButtonPadLeft( const igdeGuiTheme &guitheme );
	static int ButtonPadRight( const igdeGuiTheme &guitheme );
	static int ButtonPadTop( const igdeGuiTheme &guitheme );
	static int ButtonPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
};

typedef igdeNativeBeOSButton igdeNativeButton;

#endif
