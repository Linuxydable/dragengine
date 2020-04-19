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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cePlayback.h"
#include "cePlaybackActor.h"
#include "cePlaybackEvaluateCondition.h"
#include "command/cePlaybackCommandList.h"
#include "command/cePlaybackCommand.h"
#include "variable/cePlaybackVariableList.h"
#include "variable/cePlaybackVariable.h"
#include "../ceConversation.h"
#include "../condition/ceCConditionLogic.h"
#include "../condition/ceConversationCondition.h"
#include "../condition/ceCConditionHasActor.h"
#include "../condition/ceCConditionActorInConversation.h"
#include "../condition/ceCConditionVariable.h"
#include "../condition/ceCConditionActorParameter.h"
#include "../condition/ceCConditionActorCommand.h"
#include "../condition/ceCConditionGameCommand.h"
#include "../condition/ceCConditionTrigger.h"
#include "../actor/ceConversationActor.h"
#include "../actor/parameters/ceActorParameter.h"
#include "../file/ceConversationFile.h"
#include "../topic/ceConversationTopic.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>



// Class cePlaybackEvaluateCondition
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackEvaluateCondition::cePlaybackEvaluateCondition(){
}

cePlaybackEvaluateCondition::~cePlaybackEvaluateCondition(){
}



// Management
///////////////

bool cePlaybackEvaluateCondition::EvaluateCondition( ceConversation &conversation,
const ceConversationCondition &condition ){
	switch( condition.GetType() ){
	case ceConversationCondition::ectLogic:
		return EvaluateLogic( conversation,
			( const ceCConditionLogic & )condition );
		
	case ceConversationCondition::ectHasActor:
		return EvaluateHasActor( conversation,
			( const ceCConditionHasActor & )condition );
		
	case ceConversationCondition::ectActorInConversation:
		return EvaluateActorInConversation( conversation,
			( const ceCConditionActorInConversation & )condition );
		
	case ceConversationCondition::ectVariable:
		return EvaluateVariable( conversation,
			( const ceCConditionVariable & )condition );
		
	case ceConversationCondition::ectActorParameter:
		return EvaluateActorParameter( conversation,
			( const ceCConditionActorParameter & )condition );
		
	case ceConversationCondition::ectActorCommand:
		return EvaluateActorCommand( conversation,
			( const ceCConditionActorCommand & )condition );
		
	case ceConversationCondition::ectGameCommand:
		return EvaluateGameCommand( conversation,
			( const ceCConditionGameCommand & )condition );
		
	case ceConversationCondition::ectTrigger:
		return EvaluateTrigger( conversation,
			( const ceCConditionTrigger & )condition );
	}
	
	return false;
}



bool cePlaybackEvaluateCondition::EvaluateLogic( ceConversation &conversation,
const ceCConditionLogic &condition ){
	const ceConversationConditionList &list = condition.GetConditions();
	const int count = list.GetCount();
	int i;
	
	switch( condition.GetOperator() ){
	case ceCConditionLogic::eopNone:
		for( i=0; i<count; i++ ){
			if( EvaluateCondition( conversation, *list.GetAt( i ) ) ){
				return false;
			}
		}
		return true;
		
	case ceCConditionLogic::eopAny:
		for( i=0; i<count; i++ ){
			if( EvaluateCondition( conversation, *list.GetAt( i ) ) ){
				return true;
			}
		}
		return false;
		
	case ceCConditionLogic::eopAll:
		for( i=0; i<count; i++ ){
			if( ! EvaluateCondition( conversation, *list.GetAt( i ) ) ){
				return false;
			}
		}
		return count > 0;
	}
	
	return false;
}

bool cePlaybackEvaluateCondition::EvaluateHasActor( ceConversation &conversation,
const ceCConditionHasActor &condition ){
	const bool result = conversation.GetActorList().HasWithIDOrAliasID( condition.GetActor() );
	
	if( condition.GetNegate() ){
		return ! result;
		
	}else{
		return result;
	}
}

bool cePlaybackEvaluateCondition::EvaluateActorInConversation( ceConversation &conversation,
const ceCConditionActorInConversation &condition ){
	const bool result = conversation.GetActorList().HasWithIDOrAliasID( condition.GetActor() );
	
	if( condition.GetNegate() ){
		return ! result;
		
	}else{
		return result;
	}
}

bool cePlaybackEvaluateCondition::EvaluateVariable( ceConversation &conversation,
const ceCConditionVariable &condition ){
	cePlaybackVariableList &variableList = conversation.GetPlayback()->GetVariables();
	
	int testValue = condition.GetTestValue();
	if( ! condition.GetTestVariable().IsEmpty() ){
		const cePlaybackVariable * const variable = variableList.GetNamed( condition.GetTestVariable() );
		if( variable ){
			testValue = variable->GetValue();
		}
	}
	
	int variableValue = 0;
	const cePlaybackVariable * const variable = variableList.GetNamed( condition.GetVariable() );
	if( variable ){
		variableValue = variable->GetValue();
	}
	
	switch( condition.GetOperator() ){
	case ceCConditionVariable::eopEqual:
		return variableValue == testValue;
		
	case ceCConditionVariable::eopNotEqual:
		return variableValue != testValue;
		
	case ceCConditionVariable::eopLess:
		return variableValue < testValue;
		
	case ceCConditionVariable::eopLessEqual:
		return variableValue <= testValue;
		
	case ceCConditionVariable::eopGreater:
		return variableValue > testValue;
		
	case ceCConditionVariable::eopGreaterEqual:
		return variableValue >= testValue;
	}
	
	return false;
}

bool cePlaybackEvaluateCondition::EvaluateActorParameter( ceConversation &conversation,
const ceCConditionActorParameter &condition ){
	cePlayback &playback = *conversation.GetPlayback();
	const decString &actorID = condition.GetActor();
	const decString &name = condition.GetParameter();
	int paramValue = 0;
	
	int testValue = condition.GetTestValue();
	if( ! condition.GetTestVariable().IsEmpty() ){
		const cePlaybackVariable * const variable =
			playback.GetVariables().GetNamed( condition.GetTestVariable() );
		if( variable ){
			testValue = variable->GetValue();
		}
	}
	
	ceConversationActor const * conversationActor = conversation.GetActorList().GetWithIDOrAliasID( actorID );
	if( conversationActor ){
		ceActorParameter *parameter = conversationActor->GetParameter().GetNamed( name );
		
		if( parameter ){
			paramValue = parameter->GetValue();
		}
	}
	
	switch( condition.GetOperator() ){
	case ceCConditionActorParameter::eopEqual:
		return paramValue == testValue;
		
	case ceCConditionActorParameter::eopNotEqual:
		return paramValue != testValue;
		
	case ceCConditionActorParameter::eopLess:
		return paramValue < testValue;
		
	case ceCConditionActorParameter::eopLessEqual:
		return paramValue <= testValue;
		
	case ceCConditionActorParameter::eopGreater:
		return paramValue > testValue;
		
	case ceCConditionActorParameter::eopGreaterEqual:
		return paramValue >= testValue;
	}
	
	return false;
}

bool cePlaybackEvaluateCondition::EvaluateActorCommand( ceConversation &conversation,
const ceCConditionActorCommand &condition ){
	const decString &actorID = condition.GetActor();
	
	ceConversationActor const * conversationActor = conversation.GetActorList().GetWithIDOrAliasID( actorID );
	if( ! conversationActor ){
		return condition.GetNegate();
	}
	
	const cePlaybackCommandList &list = conversationActor->GetCommands();
	const cePlaybackCommand * const pcommand = list.GetWith( condition.GetCommand() );
	if( ! pcommand ){
		return condition.GetNegate();
	}
	
	if( condition.GetNegate() ){
		return ! pcommand->GetValue();
		
	}else{
		return pcommand->GetValue();
	}
}

bool cePlaybackEvaluateCondition::EvaluateGameCommand( ceConversation &conversation,
const ceCConditionGameCommand &condition ){
	const cePlaybackCommandList &list = conversation.GetPlayback()->GetCommands();
	const cePlaybackCommand * const pcommand = list.GetWith( condition.GetCommand() );
	bool result = false;
	
	if( pcommand ){
		result = pcommand->GetValue();
	}
	
	if( condition.GetNegate() ){
		return ! result;
		
	}else{
		return result;
	}
}

bool cePlaybackEvaluateCondition::EvaluateTrigger( ceConversation &conversation,
const ceCConditionTrigger &condition ){
	igdeTriggerTarget * const target = conversation.GetPlayback()->GetTriggerTable().GetNamed( condition.GetTrigger() );
	
	switch( condition.GetTestMode() ){
	case ceCConditionTrigger::etmFired:
		return target && target->GetFired();
		
	case ceCConditionTrigger::etmNotFired:
		return ! target || ! target->GetFired();
		
	case ceCConditionTrigger::etmEverFired:
		return target && target->GetHasFired();
		
	case ceCConditionTrigger::etmNeverFired:
		return ! target || ! target->GetHasFired();
	}
	
	return false;
}
