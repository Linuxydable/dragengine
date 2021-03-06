/* 
 * Drag[en]gine IGDE
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

#include "igdeIconListBox.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeIconListBoxListener.h"
#include "menu/igdeMenuCascade.h"
#include "menu/igdeMenuCascadeReference.h"
#include "model/igdeListHeader.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemReference.h"
#include "model/igdeListItemSorter.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeIconListBox
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeIconListBox::igdeIconListBox( igdeEnvironment &environment, int rows, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pSelectionMode( esmSingle ),
pViewMode( evmList ),
pSelection( -1 ),
pRows( rows ),
pDescription( description )
{
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeIconListBox::~igdeIconListBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeIconListBox::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeIconListBox::SetRows( int rows ){
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( rows == pRows ){
		return;
	}
	
	pRows = rows;
	//OnRowsChanged();
}

void igdeIconListBox::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeIconListBox::SetViewMode( eViewMode mode ){
	if( mode == pViewMode ){
		return;
	}
	
	pViewMode = mode;
	OnViewModeChanged();
}

void igdeIconListBox::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->Focus();
	}
}



int igdeIconListBox::GetItemCount() const{
	return pItems.GetCount();
}

igdeListItem *igdeIconListBox::GetItemAt( int index ) const{
	return ( igdeListItem* )pItems.GetAt( index );
}

bool igdeIconListBox::HasItem( igdeListItem *item ) const{
	return pItems.Has( item );
}

bool igdeIconListBox::HasItem( const char *item ) const{
	return IndexOfItem( item ) != -1;
}

bool igdeIconListBox::HasItemWithData( void *data ) const{
	return IndexOfItemWithData( data ) != -1;
}

int igdeIconListBox::IndexOfItem( igdeListItem *item ) const{
	return pItems.IndexOf( item );
}

int igdeIconListBox::IndexOfItem( const char *item ) const{
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( const igdeListItem* )pItems.GetAt( i ) )->GetText() == item ){
			return i;
		}
	}
	
	return -1;
}

int igdeIconListBox::IndexOfItemWithData( void *data ) const{
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( const igdeListItem* )pItems.GetAt( i ) )->GetData() == data ){
			return i;
		}
	}
	
	return -1;
}

void igdeIconListBox::AddItem( igdeListItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	pItems.Add( item );
	OnItemAdded( pItems.GetCount() - 1 );
	
	if( pItems.GetCount() == 1 ){
		SetSelection( 0 );
	}
}

void igdeIconListBox::AddItem( const char *text, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	AddItem( item, text, icon, data );
}

void igdeIconListBox::AddItem( const char *text, const decStringList &details,
igdeIcon *icon, void *data ){
	igdeListItemReference item;
	AddItem( item, text, details, icon, data );
}

void igdeIconListBox::AddItem( igdeListItemReference &item, const char *text,
igdeIcon *icon, void *data ){
	item.TakeOver( new igdeListItem( text, icon, data ) );
	AddItem( item );
}

void igdeIconListBox::AddItem( igdeListItemReference &item, const char *text,
const decStringList &details, igdeIcon *icon, void *data ){
	item.TakeOver( new igdeListItem( text, icon, data ) );
	item->GetDetails() = details;
	AddItem( item );
}

void igdeIconListBox::InsertItem( int index, igdeListItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	igdeListItem * const selection = GetSelectedItem();
	
	pItems.Insert( item, index );
	
	if( selection ){
		pSelection = pItems.IndexOf( selection );
	}
	
	OnItemAdded( index );
	
	if( pItems.GetCount() == 1 ){
		SetSelection( 0 );
	}
}

void igdeIconListBox::InsertItem( int index, const char *text, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	InsertItem( item, index, text, icon, data );
}

void igdeIconListBox::InsertItem( int index, const char *text,
const decStringList &details, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	InsertItem( item, index, text, details, icon, data );
}

void igdeIconListBox::InsertItem( igdeListItemReference &item, int index, const char *text,
igdeIcon *icon, void *data ){
	item.TakeOver( new igdeListItem( text, icon, data ) );
	InsertItem( index, item );
}

void igdeIconListBox::InsertItem( igdeListItemReference &item, int index, const char *text,
const decStringList &details, igdeIcon *icon, void *data ){
	item.TakeOver( new igdeListItem( text, icon, data ) );
	item->GetDetails() = details;
	InsertItem( index, item );
}

void igdeIconListBox::MoveItem( int fromIndex, int toIndex ){
	igdeListItem * const selection = GetSelectedItem();
	
	pItems.Move( fromIndex, toIndex );
	
	if( selection ){
		pSelection = pItems.IndexOf( selection );
	}
	
	OnItemMoved( fromIndex, toIndex );
}

void igdeIconListBox::RemoveItem( int index ){
	pItems.RemoveFrom( index );
	OnItemRemoved( index );
	
	if( index == pSelection ){
		SetSelection( decMath::min( pSelection, pItems.GetCount() - 1 ) );
		
	}else if( index < pSelection ){
		pSelection--;
	}
}

void igdeIconListBox::RemoveAllItems(){
	if( pItems.GetCount() == 0 ){
		return;
	}
	
	if( pSelection != -1 ){
		SetSelection( -1 );
	}
	
	pItems.RemoveAll();
	
	OnAllItemsRemoved();
}

void igdeIconListBox::ItemChangedAt( int index ){
	if( index < 0 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	OnItemChanged( index );
}



void igdeIconListBox::SetSorter( igdeListItemSorter *sorter ){
	pSorter = sorter;
}

void igdeIconListBox::SetDefaultSorter(){
	pSorter.TakeOver( new igdeListItemSorter );
}

static void igdeIconListBox_Sort( decObjectList &items, igdeListItemSorter &sorter, int left, int right ){
	igdeListItemReference pivot = ( igdeListItem* )items.GetAt( left );
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && sorter.Precedes( pivot, *( ( igdeListItem* )items.GetAt( right ) ) ) ){
			right--;
		}
		if( left != right ){
			items.SetAt( left, items.GetAt( right ) );
			left++;
		}
		while( left < right && sorter.Precedes( *( ( igdeListItem* )items.GetAt( left ) ), pivot ) ){
			left++;
		}
		if( left != right ){
			items.SetAt( right, items.GetAt( left ) );
			right--;
		}
	}
	
	items.SetAt( left, ( igdeListItem* )pivot );
	if( l_hold < left ){
		igdeIconListBox_Sort( items, sorter, l_hold, left - 1 );
	}
	if( r_hold > left ){
		igdeIconListBox_Sort( items, sorter, left + 1, r_hold );
	}
}

void igdeIconListBox::SortItems(){
	if( ! pSorter ){
		return;
	}
	
	const int count = pItems.GetCount();
	if( count < 2 ){
		return;
	}
	
	igdeListItem * const selected = GetSelectedItem();
	
	igdeIconListBox_Sort( pItems, pSorter, 0, count - 1 );
	
	if( selected ){
		pSelection = pItems.IndexOf( selected );
	}
	
	OnItemsSorted();
}



void igdeIconListBox::SetSelectionMode( eSelectionMode mode ){
	if( mode == pSelectionMode ){
		return;
	}
	
	pSelectionMode = mode;
	OnSelectionModeChanged();
}

igdeListItem *igdeIconListBox::GetSelectedItem() const{
	if( pSelection != -1 ){
		return ( igdeListItem* )pItems.GetAt( pSelection );
		
	}else{
		return NULL;
	}
}

void igdeIconListBox::SetSelection( int selection ){
	if( selection < -1 || selection >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( selection == pSelection ){
		return;
	}
	
	pSelection = selection;
	OnSelectionChanged();
	NotifySelectionChanged();
}

void igdeIconListBox::SetSelectionWithData( void *data ){
	SetSelection( IndexOfItemWithData( data ) );
}

void igdeIconListBox::SelectItem( int index ){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	if( index < -1 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( index ) );
	if( item.GetSelected() ){
		return;
	}
	
	item.SetSelected( true );
	OnSelectionChanged();
}

void igdeIconListBox::SelectAllItems(){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( i ) );
		if( item.GetSelected() ){
			continue;
		}
		
		item.SetSelected( true );
		changed = true;
	}
	
	if( changed ){
		OnSelectionChanged();
	}
}

void igdeIconListBox::DeselectItem( int index ){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	if( index < -1 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( index ) );
	if( ! item.GetSelected() ){
		return;
	}
	
	item.SetSelected( false );
	OnSelectionChanged();
}

void igdeIconListBox::DeselectAllItems(){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( i ) );
		if( ! item.GetSelected() ){
			continue;
		}
		
		item.SetSelected( false );
		changed = true;
	}
	
	if( changed ){
		OnSelectionChanged();
	}
}

void igdeIconListBox::NotifyItemSelected( int index ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeIconListBoxListener* )listeners.GetAt( i ) )->OnItemSelected( this, index );
	}
}

void igdeIconListBox::NotifyItemDeselected( int index ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeIconListBoxListener* )listeners.GetAt( i ) )->OnItemDeselected( this, index );
	}
}



int igdeIconListBox::GetHeaderCount() const{
	return pHeaders.GetCount();
}

igdeListHeader *igdeIconListBox::GetHeaderAt( int index ) const{
	return ( igdeListHeader* )pHeaders.GetAt( index );
}

bool igdeIconListBox::HasHeader( igdeListHeader *header ) const{
	return pHeaders.Has( header );
}

void igdeIconListBox::AddHeader( igdeListHeader *header ){
	if( ! header || pHeaders.Has( header ) ){
		DETHROW( deeInvalidParam );
	}
	
	pHeaders.Add( header );
	OnHeaderChanged();
}

void igdeIconListBox::RemoveHeader( igdeListHeader *header ){
	pHeaders.Remove( header );
	OnHeaderChanged();
}

void igdeIconListBox::RemoveAllHeaders(){
	if( pHeaders.GetCount() == 0 ){
		return;
	}
	
	pHeaders.RemoveAll();
	OnHeaderChanged();
}

void igdeIconListBox::HeaderChangedAt( int index ){
	if( index < 0 || index >= pHeaders.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	OnHeaderChanged();
}

void igdeIconListBox::NotifyHeaderClicked( int index ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeIconListBoxListener* )listeners.GetAt( i ) )->OnHeaderClicked( this, index );
	}
}



void igdeIconListBox::EnsureItemVisible( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeIconListBox* )GetNativeWidget() )->MakeItemVisible( index );
}

void igdeIconListBox::EnsureSelectedItemVisible(){
	if( pSelection != -1 ){
		EnsureItemVisible( pSelection );
	}
}

void igdeIconListBox::ShowContextMenu( const decPoint &position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( GetEnvironment() ) );
	
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeIconListBoxListener* )pListeners.GetAt( i ) )->AddContextMenuEntries( this, menu );
	}
	
	if( menu->GetChildCount() > 0 ){
		menu->Popup( *this, position );
	}
}



void igdeIconListBox::AddListener( igdeIconListBoxListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeIconListBox::RemoveListener( igdeIconListBoxListener *listener ){
	pListeners.Remove( listener );
}

void igdeIconListBox::NotifySelectionChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeIconListBoxListener* )listeners.GetAt( i ) )->OnSelectionChanged( this );
	}
}

void igdeIconListBox::NotifyDoubleClickItem( int index ){
	if( index < 0 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeIconListBoxListener* )listeners.GetAt( i ) )->OnDoubleClickItem( this, index );
	}
}



void igdeIconListBox::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeIconListBox * const native = igdeNativeIconListBox::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeIconListBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeIconListBox* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeIconListBox::OnItemAdded( int index ){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->InsertItem(
			index, *( ( igdeListItem* )pItems.GetAt( index ) ) );
	}
}

void igdeIconListBox::OnItemRemoved( int index ){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->RemoveItem( index );
	}
}

void igdeIconListBox::OnAllItemsRemoved(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeIconListBox* )GetNativeWidget() )->RemoveAllItems();
}

void igdeIconListBox::OnItemChanged( int index ){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateItem( index );
	}
}

void igdeIconListBox::OnItemMoved( int fromIndex, int toIndex ){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->MoveItem( fromIndex, toIndex );
	}
}

void igdeIconListBox::OnItemsSorted(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->BuildList();
	}
}

void igdeIconListBox::OnSelectionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateSelection();
	}
}

void igdeIconListBox::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeIconListBox::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeIconListBox::OnViewModeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateStyles();
	}
}

void igdeIconListBox::OnSelectionModeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateStyles();
	}
}

void igdeIconListBox::OnHeaderChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeIconListBox* )GetNativeWidget() )->UpdateHeader();
	}
}
