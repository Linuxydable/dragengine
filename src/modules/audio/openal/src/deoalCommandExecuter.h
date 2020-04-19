/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALCOMMANDEXECUTER_H_
#define _DEOALCOMMANDEXECUTER_H_

class deAudioOpenAL;
class decUnicodeArgumentList;
class decUnicodeString;



/**
 * \brief Command executer.
 */
class deoalCommandExecuter{
private:
	deAudioOpenAL &pOal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create command executer. */
	deoalCommandExecuter( deAudioOpenAL &audioThread );
	
	/** \brief Clean up command executer. */
	~deoalCommandExecuter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run command. */
	void ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	/** \brief Help message. */
	void CmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	/** \brief Display extensions. */
	void CmdExtensions( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
};

#endif
