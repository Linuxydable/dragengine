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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seClipboardDataSource.h"
#include "../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seClipboardDataSource
//////////////////////////////

const char * const seClipboardDataSource::TYPE_NAME = "source";

// Constructor, destructor
////////////////////////////

seClipboardDataSource::seClipboardDataSource( seSource *source ) :
igdeClipboardData( TYPE_NAME )
{
	seSource *copySource = NULL;
	
	try{
		copySource = source->CreateCopy();
		pSources.Add( copySource );
		copySource->FreeReference();
		
	}catch( const deException & ){
		if( copySource ){
			copySource->FreeReference();
		}
		throw;
	}
}

seClipboardDataSource::seClipboardDataSource( const seSourceList &sources ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = sources.GetCount();
	seSource *source = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			source = sources.GetAt( i )->CreateCopy();
			pSources.Add( source );
			source->FreeReference();
			source = NULL;
		}
		
	}catch( const deException & ){
		if( source ){
			source->FreeReference();
		}
		throw;
	}
}

seClipboardDataSource::~seClipboardDataSource(){
	pSources.RemoveAll();
}
