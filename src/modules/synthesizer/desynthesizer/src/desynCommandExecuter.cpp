/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynCommandExecuter.h"
#include "desynBasics.h"
#include "deDESynthesizer.h"
#include "desynConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>



// Class desynCommandExecuter
///////////////////////////////

// Constructor, destructor
////////////////////////////

desynCommandExecuter::desynCommandExecuter( deDESynthesizer &module ) :
pModule( module ){
}

desynCommandExecuter::~desynCommandExecuter(){
}



// Management
///////////////

void desynCommandExecuter::ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 0 ){
		answer.SetFromUTF8( "No command provided." );
		
	}else{
		if( command.MatchesArgumentAt( 0, "help" ) ){
			CmdHelp( command, answer );
			
		}else{
			answer.SetFromUTF8( "Unknown command '" );
			answer += *command.GetArgumentAt( 0 );
			answer.AppendFromUTF8( "'." );
		}
	}
}



void desynCommandExecuter::CmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	answer.SetFromUTF8( "help => Displays this help screen.\n" );
}
