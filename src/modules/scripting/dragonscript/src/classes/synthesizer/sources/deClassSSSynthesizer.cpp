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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassSSSynthesizer.h"
#include "../deClassSynthesizer.h"
#include "../deClassSynthesizerSource.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sSSSynNatDat{
	deSynthesizer *synthesizer;
	deSynthesizerSourceSynthesizer *source;
};



// Native Functions
/////////////////////

// public func new()
deClassSSSynthesizer::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSSSyn,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSSynthesizer::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	// super call
	deClassSynthesizerSource * const baseClass = ( deClassSynthesizerSource* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create synthesizer source
	nd.source = new deSynthesizerSourceSynthesizer;
	baseClass->AssignSource( myself->GetRealObject(), nd.source );
}

// public func destructor()
deClassSSSynthesizer::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSSSyn,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSSynthesizer::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself ) );
	
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
	
	if( nd.source ){
		nd.source->FreeReference();
		nd.source = NULL;
	}
}



// public func void targetAddLink( SSSynthesizer target, int link )
deClassSSSynthesizer::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsSSSyn,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSSynthesizerTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassSSSynthesizer::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself ) );
	const deClassSSSynthesizer::eTargets target = ( deClassSSSynthesizer::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassSSSynthesizer::etBlendFactor:
		nd.source->GetTargetBlendFactor().AddLink( link );
		break;
		
	case deClassSSSynthesizer::etVolume:
		nd.source->GetTargetVolume().AddLink( link );
		break;
		
	case deClassSSSynthesizer::etPanning:
		nd.source->GetTargetPanning().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void targetRemoveAllLinks( SSSynthesizer target )
deClassSSSynthesizer::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsSSSyn,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSSynthesizerTarget ); // target
}
void deClassSSSynthesizer::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself ) );
	const deClassSSSynthesizer::eTargets target = ( deClassSSSynthesizer::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassSSSynthesizer::etBlendFactor:
		nd.source->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassSSSynthesizer::etVolume:
		nd.source->GetTargetVolume().RemoveAllLinks();
		break;
		
	case deClassSSSynthesizer::etPanning:
		nd.source->GetTargetPanning().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void setSynthesizer( Synthesizer synthesizer )
deClassSSSynthesizer::nfSetSynthesizer::nfSetSynthesizer( const sInitData &init ) : dsFunction( init.clsSSSyn,
"setSynthesizer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSyn ); // synthesizer
}
void deClassSSSynthesizer::nfSetSynthesizer::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassSSSynthesizer* )GetOwnerClass() )->GetDS();
	deSynthesizer * const synthesizer = ds.GetClassSynthesizer()->GetSynthesizer( rt->GetValue( 0 )->GetRealObject() );
	
	if( synthesizer == nd.source->GetSynthesizer() ){
		return;
	}
	
	nd.source->SetSynthesizer( synthesizer );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setConnectionAt( int target, int controller )
deClassSSSynthesizer::nfSetConnectionAt::nfSetConnectionAt( const sInitData &init ) : dsFunction( init.clsSSSyn,
"setConnectionAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // target
	p_AddParameter( init.clsInt ); // controller
}
void deClassSSSynthesizer::nfSetConnectionAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself ) );
	
	const int target = rt->GetValue( 0 )->GetInt();
	const int controller = rt->GetValue( 1 )->GetInt();
	
	if( target > 0 || target < nd.source->GetConnectionCount() ){
		if( target == nd.source->GetConnectionAt( controller ) ){
			return;
		}
		nd.source->SetConnectionAt( target, controller );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// Class deClassSSSynthesizer
///////////////////////////////

// Constructor
////////////////

deClassSSSynthesizer::deClassSSSynthesizer( deScriptingDragonScript &ds ) :
dsClass( "SSSynthesizer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "SynthesizerSource" );
	
	p_SetNativeDataSize( sizeof( sSSSynNatDat ) );
}

deClassSSSynthesizer::~deClassSSSynthesizer(){
}



// Management
///////////////

void deClassSSSynthesizer::CreateClassMembers( dsEngine *engine ){
	pClsSSSynthesizerTarget = engine->GetClass( "Dragengine.Scenery.SSSynthesizerTarget" );
	
	sInitData init;
	init.clsSSSyn = this;
	init.clsSyn = pDS.GetClassSynthesizer();
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSSSynthesizerTarget = pClsSSSynthesizerTarget;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	AddFunction( new nfSetSynthesizer( init ) );
	AddFunction( new nfSetConnectionAt( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerSourceSynthesizer *deClassSSSynthesizer::GetSource( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSSSynNatDat* )p_GetNativeData( myself->GetBuffer() ) )->source;
}

void deClassSSSynthesizer::AssignSynthesizer( dsRealObject *myself, deSynthesizer *synthesizer ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassSynthesizerSource()->AssignSynthesizer( myself, synthesizer );
	
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( synthesizer == nd.synthesizer ){
		return;
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
	}
	
	nd.synthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddReference();
	}
}

void deClassSSSynthesizer::PushSource( dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceSynthesizer *source ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! source ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassSynthesizerSource * const baseClass = ( deClassSynthesizerSource* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sSSSynNatDat &nd = *( ( sSSSynNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.synthesizer = synthesizer;
		if( synthesizer ){
			synthesizer->AddReference();
		}
		
		nd.source = source;
		source->AddReference();
		
		baseClass->AssignSource( rt->GetValue( 0 )->GetRealObject(), source );
		baseClass->AssignSynthesizer( rt->GetValue( 0 )->GetRealObject(), synthesizer );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
