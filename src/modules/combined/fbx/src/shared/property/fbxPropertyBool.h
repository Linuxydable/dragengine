/* 
 * FBX Modules
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

#ifndef _FBXPROPERTYBOOL_H_
#define _FBXPROPERTYBOOL_H_


#include "../fbxProperty.h"


/**
 * \brief FBX property bool.
 */
class fbxPropertyBool : public fbxProperty{
private:
	bool pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyBool();
	
	/** \brief Load property. */
	fbxPropertyBool( decBaseFileReader &reader );
	
protected:
	/** \brief Clean up property. */
	virtual ~fbxPropertyBool();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value. */
	inline bool GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( bool value );
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyBool &CastBool();
	
	/** \brief Get values as specific type if possible. */
	virtual bool GetValueAsBool();
	virtual int GetValueAsInt();
	virtual float GetValueAsFloat();
	virtual double GetValueAsDouble();
	
	
	
	/** \brief Save to file. */
	virtual void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure( deBaseModule &logger, const decString &prefix ) const;
	/*@}*/
};

#endif
