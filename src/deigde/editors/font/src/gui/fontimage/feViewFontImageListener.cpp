/* 
 * Drag[en]gine IGDE Font Editor
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

#include <stdlib.h>
#include <string.h>

#include "feViewFontImage.h"
#include "feViewFontImageListener.h"
#include "../../font/feFont.h"

#include <dragengine/common/exceptions.h>



// Class feViewFontImageListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

feViewFontImageListener::feViewFontImageListener( feViewFontImage &panel ) :
pPanel( panel ){
}

feViewFontImageListener::~feViewFontImageListener(){
}



// Management
///////////////


void feViewFontImageListener::FontChanged( feFont *font ){
	pPanel.UpdateScrollbarRanges();
	pPanel.UpdateImageViewCanvas();
}

void feViewFontImageListener::ImageChanged( feFont *font, feFontImage *image ){
    pPanel.UpdateScrollbarRanges();
	pPanel.UpdateImageViewCanvas();
}

void feViewFontImageListener::GlyphStructureChanged( feFont *font ){
	pPanel.UpdateImageViewCanvas();
}

void feViewFontImageListener::GlyphChanged( feFont *font, feFontGlyph *glyph ){
	pPanel.UpdateImageViewCanvas();
}

void feViewFontImageListener::GlyphSelectionChanged( feFont *font ){
	pPanel.UpdateImageViewCanvas();
}

void feViewFontImageListener::ActiveGlyphChanged( feFont *font ){
	pPanel.UpdateImageViewCanvas();
}
