/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEUCAASPEAKEYESLASETPAUSE_H_
#define _CEUCAASPEAKEYESLASETPAUSE_H_

#include "../strip/ceUCAASpeakStripSetPause.h"



/**
 * \brief Undo action actor speak conversation action set eyeslookat pause.
 */
class ceUCAASpeakEyesLASetPause : public ceUCAASpeakStripSetPause{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCAASpeakEyesLASetPause( ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *eyesLookAt, float newPause );
	/** \brief Cleans up the undo object. */
	virtual ~ceUCAASpeakEyesLASetPause();
	/*@}*/
};

#endif
