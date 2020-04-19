/* 
 * Drag[en]gine DragonScript Script Module
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

// include only once
#ifndef _DEDSXMLPARSER_H_
#define _DEDSXMLPARSER_H_

// includes
#include "dragengine/common/xmlparser/decXmlParser.h"

// predefintions
class deLogger;



// xml parser extension with logging
class dedsXmlParser : public decXmlParser{
private:
	char *pLog;
	int pLogLen;
	
public:
	// constructor, destructor
	dedsXmlParser( deLogger *logger );
	virtual ~dedsXmlParser();
	// management
	inline const char *GetParseLog() const{ return ( const char * )pLog; }
	virtual void UnexpectedEOF( int line, int pos );
	virtual void UnexpectedToken( int line, int pos, const char *token );
};

// end of include only once
#endif
