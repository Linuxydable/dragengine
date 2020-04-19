/* 
 * Drag[en]gine Live Launcher
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dellGPDisableModuleVersion.h"

#include <dragengine/common/exceptions.h>



// Class dellGPDisableModuleVersion
/////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGPDisableModuleVersion::dellGPDisableModuleVersion(){
}

dellGPDisableModuleVersion::dellGPDisableModuleVersion( const char *name, const char *version ) :
pName( name ),
pVersion( version ){
}

dellGPDisableModuleVersion::dellGPDisableModuleVersion( const dellGPDisableModuleVersion &copy ) :
pName( copy.pName ),
pVersion( copy.pVersion ){
}

dellGPDisableModuleVersion::~dellGPDisableModuleVersion(){
}



// Management
///////////////

void dellGPDisableModuleVersion::SetName( const char *name ){
	pName = name;
}

void dellGPDisableModuleVersion::SetVersion( const char *version ){
	pVersion = version;
}



// Operators
//////////////

bool dellGPDisableModuleVersion::operator==( const dellGPDisableModuleVersion &other ) const{
	return pName == other.pName && pVersion == other.pVersion;
}

bool dellGPDisableModuleVersion::operator!=( const dellGPDisableModuleVersion &other ) const{
	return pName != other.pName || pVersion != other.pVersion;
}

dellGPDisableModuleVersion &dellGPDisableModuleVersion::operator=( const dellGPDisableModuleVersion &other ){
	pName = other.pName;
	pVersion = other.pVersion;
	return *this;
}
