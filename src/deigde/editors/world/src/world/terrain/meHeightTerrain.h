/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHEIGHTTERRAIN_H_
#define _MEHEIGHTTERRAIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class meHeightTerrainSector;
class meHTVegetationLayer;
class meObject;
class meWorld;

class deDebugDrawer;
class deHeightTerrain;

class igdeEnvironment;



/**
 * \brief Height terrain.
 */
class meHeightTerrain : public deObject{
private:
	meWorld &pWorld;
	deHeightTerrain *pEngHT;
	
	decString pPathHT;
	bool pChanged;
	bool pDepChanged;
	bool pSaved;
	
	float pSectorSize;
	int pSectorResolution;
	float pBaseHeight;
	float pHeightScaling;
	
	decObjectOrderedSet pSectors;
	meHeightTerrainSector *pActiveSector;
	
	decObjectOrderedSet pVLayers;
	meHTVegetationLayer *pActiveVLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meHeightTerrain( meWorld &world );
	
	
	
protected:
	/** \brief Clean up object. */
	virtual ~meHeightTerrain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Engine height terrain. */
	inline deHeightTerrain *GetEngineHeightTerrain() const{ return pEngHT; }
	
	
	
	/** \brief Height terrain file path. */
	inline const decString &GetPathHT() const{ return pPathHT; }
	
	/** \brief Set height terrain file path. */
	void SetPathHT( const char *path );
	
	/** \brief Base directory. */
	decString GetBaseDirectory() const;
	
	/** \brief World dependency has changed. */
	inline bool GetDepChanged() const{ return pDepChanged; }
	
	/** \brief Set if world dependency has changed. */
	void SetDepChanged( bool changed );
	
	/** \brief World has changed. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** \brief Set if world has changed. */
	void SetChanged( bool changed );
	
	/** \brief World has saved. */
	inline bool GetSaved() const{ return pSaved; }
	
	/** \brief Set if world has saved. */
	void SetSaved( bool saved );
	
	
	
	/** \brief Sector size. */
	inline float GetSectorSize() const{ return pSectorSize; }
	
	/** \brief Set sector size. */
	void SetSectorSize( float size );
	
	/** \brief Sector resolution. */
	inline int GetSectorResolution() const{ return pSectorResolution; }
	
	/** \brief Set sector resolution. */
	void SetSectorResolution( int resolution );
	
	/** \brief Base height. */
	inline float GetBaseHeight() const{ return pBaseHeight; }
	
	/** \brief Set base height. */
	void SetBaseHeight( float height );
	
	/** \brief Height scaling. */
	inline float GetHeightScaling() const{ return pHeightScaling; }
	
	/** \brief Set height scaling. */
	void SetHeightScaling( float scaling );
	
	
	
	/** \brief Set world changed. */
	void SetWorldChanged();
	
	
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	
	
	
	/** \brief Notify engine height values changed. */
	void NotifyHeightsChanged( const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid );
	
	/** \brief Notify engine height values changed. */
	void NotifyHeightsChanged( const decBoundary &areaSector, const decBoundary &areaGrid );
	
	/** \brief Invalidate heights. */
	void InvalidateHeights( const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid );
	
	/** \brief Invalidate all heights. */
	void InvalidateAllHeights();
	
	
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields( const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid );
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields( const decBoundary &areaSector, const decBoundary &areaGrid );
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields( meHeightTerrainSector *sector );
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields( meObject *object );
	
	/** \brief Invalidate all prop fields. */
	void InvalidateAllPropFields();
	
	
	
	/** \brief Init delegates. */
	void InitDelegates( igdeEnvironment *environment );
	
	
	
	/** \brief Update height terrain. */
	void Update();
	
	/** \brief Force update of vegetation. */
	void ForceUpdateVegetation( bool fullUpdate );
	
	/** \brief Clear vegetation. */
	void ClearVegetation();
	
	/** \brief Rebuild height terrain engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	/*@}*/
	
	
	
	/** \name Sectors */
	/*@{*/
	/** \brief Number of sectors. */
	int GetSectorCount() const;
	
	/** \brief Sector at index. */
	meHeightTerrainSector *GetSectorAt( int index ) const;
	
	/** \brief Sector at coordinates or \em NULL if absent. */
	meHeightTerrainSector *GetSectorWith( const decPoint &coordinates ) const;
	
	/** \brief Index of sector or -1 if absent. */
	int IndexOfSector( meHeightTerrainSector *sector ) const;
	
	/** \brief Add sector. */
	void AddSector( meHeightTerrainSector *sector );
	
	/** \brief Remove sector. */
	void RemoveSector( meHeightTerrainSector *sector );
	
	/** \brief Remove all sectors. */
	void RemoveAllSectors();
	
	/** \brief Active sector. */
	inline meHeightTerrainSector *GetActiveSector() const{ return pActiveSector; }
	
	/** \brief Set active sector. */
	void SetActiveSector( meHeightTerrainSector *sector );
	/*@}*/
	
	
	
	/** \name Vegetation */
	/*@{*/
	/** \brief Count of vegetation layers. */
	int GetVLayerCount() const;
	
	/** \brief Vegetation layer at index. */
	meHTVegetationLayer *GetVLayerAt( int index ) const;
	
	/** \brief Index of the vegetation layer or -1 if absent. */
	int IndexOfVLayer( meHTVegetationLayer *vlayer ) const;
	
	/** \brief Vegetation layer is present. */
	bool HasVLayer( meHTVegetationLayer *vlayer ) const;
	
	/** \brief Add vegetation layer. */
	void AddVLayer( meHTVegetationLayer *vlayer );
	
	/** \brief Inserts vegetation layer. */
	void InsertVLayer( int before, meHTVegetationLayer *vlayer );
	
	/** \brief Move vegetation layer to new position. */
	void MoveVLayer( meHTVegetationLayer *vlayer, int moveTo );
	
	/** \brief Remove vegetation layer. */
	void RemoveVLayer( meHTVegetationLayer *vlayer );
	
	/** \brief Remove all vegetation layers. */
	void RemoveAllVLayers();
	
	/** \brief Active vegetation layer or NULL. */
	inline meHTVegetationLayer *GetActiveVLayer() const{ return pActiveVLayer; }
	
	/** \brief Set active vegetation layer or NULL. */
	void SetActiveVLayer( meHTVegetationLayer *vlayer );
	
	/** \brief Notify world vegetation layer changed. */
	void NotifyVLayerChanged( meHTVegetationLayer *vlayer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateHeightTerrain();
};

#endif
