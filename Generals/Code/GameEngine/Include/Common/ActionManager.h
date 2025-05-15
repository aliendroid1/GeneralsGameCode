/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: ActionManager.h //////////////////////////////////////////////////////////////////////////
// Author: Colin Day
// Desc:   TheActionManager is a convenient place for us to wrap up all sorts of logical 
//				 queries about what objects can do in the world and to other objects.  The purpose
//				 of having a central place for this logic assists us in making these logical kind
//				 of queries in the user interface and allows us to use the same code to validate
//				 commands as they come in over the network interface in order to do the 
//				 real action.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __ACTIONMANAGER_H_
#define __ACTIONMANAGER_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/SubsystemInterface.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Object;
class Player;
class SpecialPowerTemplate;
enum SpecialPowerType : Int;
enum WeaponSlotType : Int;
enum CommandSourceType : Int;
enum CanAttackResult : Int;

enum CanEnterType : Int
{
	CHECK_CAPACITY,
	DONT_CHECK_CAPACITY,
	COMBATDROP_INTO
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class ActionManager : public SubsystemInterface
{

public:

	ActionManager( void );
	virtual ~ActionManager( void );

	virtual void init( void ) { };							///< subsystem interface
	virtual void reset( void ) { };							///< subsystem interface
	virtual void update( void ) { };						///< subsystem interface

	//Single unit to unit check
	bool canGetRepairedAt( const Object *obj, const Object *repairDest, CommandSourceType commandSource );
	bool canTransferSuppliesAt( const Object *obj, const Object *transferDest );
	bool canDockAt( const Object *obj, const Object *dockDest, CommandSourceType commandSource );
	bool canGetHealedAt( const Object *obj, const Object *healDest, CommandSourceType commandSource );
	bool canRepairObject( const Object *obj, const Object *objectToRepair, CommandSourceType commandSource );
	bool canResumeConstructionOf( const Object *obj, const Object *objectBeingConstructed, CommandSourceType commandSource );
	bool canEnterObject( const Object *obj, const Object *objectToEnter, CommandSourceType commandSource, CanEnterType mode );
	CanAttackResult getCanAttackObject( const Object *obj, const Object *objectToAttack, CommandSourceType commandSource, AbleToAttackType attackType );
	bool canConvertObjectToCarBomb( const Object *obj, const Object *objectToConvert, CommandSourceType commandSource );
	bool canHijackVehicle( const Object *obj, const Object *ObjectToHijack, CommandSourceType commandSource ); // LORENZEN
	bool canCaptureBuilding( const Object *obj, const Object *objectToCapture, CommandSourceType commandSource );
	bool canDisableVehicleViaHacking( const Object *obj, const Object *objectToHack, CommandSourceType commandSource, bool checkSourceRequirements = true );
#ifdef ALLOW_SURRENDER
	bool canPickUpPrisoner( const Object *obj, const Object *prisoner, CommandSourceType commandSource );
#endif
	bool canStealCashViaHacking( const Object *obj, const Object *objectToHack, CommandSourceType commandSource );
	bool canSnipeVehicle( const Object *obj, const Object *objectToSnipe, CommandSourceType commandSource );
	bool canBribeUnit( const Object *obj, const Object *objectToBribe, CommandSourceType commandSource );
	bool canCutBuildingPower( const Object *obj, const Object *building, CommandSourceType commandSource );
	bool canDisableBuildingViaHacking( const Object *obj, const Object *objectToHack, CommandSourceType commandSource );
	bool canDoSpecialPowerAtLocation( const Object *obj, const Coord3D *loc, CommandSourceType commandSource, const SpecialPowerTemplate *spTemplate, const Object *objectInWay, UnsignedInt commandOptions, bool checkSourceRequirements = true );
	bool canDoSpecialPowerAtObject( const Object *obj, const Object *target, CommandSourceType commandSource, const SpecialPowerTemplate *spTemplate, UnsignedInt commandOptions, bool checkSourceRequirements = true);
	bool canDoSpecialPower( const Object *obj, const SpecialPowerTemplate *spTemplate, CommandSourceType commandSource, UnsignedInt commandOptions, bool checkSourceRequirements = true );
	bool canMakeObjectDefector( const Object *obj, const Object *objectToMakeDefector, CommandSourceType commandSource );
	bool canFireWeaponAtLocation( const Object *obj, const Coord3D *loc, CommandSourceType commandSource, const WeaponSlotType slot, const Object *objectInWay );
	bool canFireWeaponAtObject( const Object *obj, const Object *target, CommandSourceType commandSource, const WeaponSlotType slot );
	bool canFireWeapon( const Object *obj, const WeaponSlotType slot, CommandSourceType commandSource );
	bool canGarrison( const Object *obj, const Object *target, CommandSourceType commandSource );
	bool canOverrideSpecialPowerDestination( const Object *obj, const Coord3D *loc, SpecialPowerType spType, CommandSourceType commandSource );

	//Player to unit check
	bool canPlayerGarrison( const Player *player, const Object *target, CommandSourceType commandSource );

protected:

};

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////
extern ActionManager *TheActionManager;

#endif  // end __ACTIONMANAGER_H_
