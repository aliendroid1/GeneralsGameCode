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

// JetAIUpdate.h //////////
// Author: Steven Johnson, June 2002
 
#pragma once

#ifndef _JET_AI_UPDATE_H_
#define _JET_AI_UPDATE_H_

#include "Common/STLTypedefs.h"
#include "Common/GameMemory.h"
#include "GameLogic/AIStateMachine.h"
#include "GameLogic/Module/AIUpdate.h"

#ifdef RTS_INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
class JetAIUpdateModuleData : public AIUpdateModuleData
{
public:
	Real										m_outOfAmmoDamagePerSecond;				/**< amount of damage to take per SEC (not per frame) when out of ammo
																																	note that it's expressed as a percent of max health, not an absolute */
	Real										m_takeoffSpeedForMaxLift;					///< percent of speed that gives us max lift
	Real										m_minHeight;											///< how far off the ground to lift the drawable when taxiing
	Real										m_parkingOffset;									///< tweaking the park loc
	Real										m_sneakyOffsetWhenAttacking;			///< our sneaky offset when attacking (or zero)
	bool										m_keepsParkingSpaceWhenAirborne;	///< if t, keeps its parking space reservation even when airborne
	bool										m_needsRunway;										///< if t, needs runways to takeoff/land
	UnsignedInt							m_takeoffPause;										///< pre-takeoff pause
	LocomotorSetType				m_attackingLoco;									///< custom attacking loco
	LocomotorSetType				m_returningLoco;									///< custom return-for-ammo loco
	UnsignedInt							m_attackLocoPersistTime;					///< if we have a custom attack loco, it persists for this long after attack is done
	UnsignedInt							m_attackersMissPersistTime;				///< how long after our attack we continue immunity
	UnsignedInt							m_lockonTime;											///< time it takes for someone to lock-on to us.
	AsciiString							m_lockonCursor;										///< template used for lockon.
	Real										m_lockonInitialDist;							///< how far away the lockon cursor starts.
	Real										m_lockonFreq;											
	Real										m_lockonAngleSpin;								///< how many times to spin around it
	Real										m_lockonBlinky;								
	UnsignedInt							m_returnToBaseIdleTime;						///< if we're idle for this long, return to base

	JetAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse& p);
};

//-------------------------------------------------------------------------------------------------
class JetAIUpdate : public AIUpdateInterface
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( JetAIUpdate, "JetAIUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( JetAIUpdate, JetAIUpdateModuleData )

	virtual UpdateSleepTime update();

public:

	JetAIUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	virtual void onObjectCreated();
	virtual void onDelete();

 	virtual void aiDoCommand(const AICommandParms* parms);
	virtual bool chooseLocomotorSet(LocomotorSetType wst);
	virtual void setLocomotorGoalNone();
	virtual bool isIdle() const;

	virtual bool isAllowedToMoveAwayFromUnit() const;
	virtual bool getSneakyTargetingOffset(Coord3D* offset) const;
	virtual void addTargeter(ObjectID id, bool add);
	virtual bool isTemporarilyPreventingAimSuccess() const;
	virtual bool isDoingGroundMovement() const;
	virtual void notifyVictimIsDead();

	const Coord3D* friend_getProducerLocation() const { return &m_producerLocation; }
	Real friend_getOutOfAmmoDamagePerSecond() const { return getJetAIUpdateModuleData()->m_outOfAmmoDamagePerSecond; }
	bool friend_keepsParkingSpaceWhenAirborne() const { return getJetAIUpdateModuleData()->m_keepsParkingSpaceWhenAirborne; }
	bool friend_needsRunway() const { return getJetAIUpdateModuleData()->m_needsRunway; }
	Real friend_getTakeoffSpeedForMaxLift() const { return getJetAIUpdateModuleData()->m_takeoffSpeedForMaxLift; }
	Real friend_getMinHeight() const { return getJetAIUpdateModuleData()->m_minHeight; }
	Real friend_getParkingOffset() const { return getJetAIUpdateModuleData()->m_parkingOffset; }
	UnsignedInt friend_getTakeoffPause() const { return getJetAIUpdateModuleData()->m_takeoffPause; }
	void friend_setGoalPath( const std::vector<Coord3D>* path ) { getStateMachine()->setGoalPath(path); }
	void friend_setTakeoffInProgress(bool v) { setFlag(TAKEOFF_IN_PROGRESS, v); }
	void friend_setLandingInProgress(bool v) { setFlag(LANDING_IN_PROGRESS, v); }
	void friend_setTaxiInProgress(bool v) { setFlag(TAXI_IN_PROGRESS, v); }
	void friend_setUseSpecialReturnLoco(bool v) { setFlag(USE_SPECIAL_RETURN_LOCO, v); }
	void friend_setAllowCircling(bool v) { setFlag(ALLOW_CIRCLING, v); }
	const Coord3D& friend_getLandingPosForHelipadStuff() const { return m_landingPosForHelipadStuff; }
	void friend_enableAfterburners(bool v);
	void friend_setAllowAirLoco(bool a);
	bool friend_isTakeoffOrLandingInProgress() const
	{
		return (getFlag(TAKEOFF_IN_PROGRESS) || getFlag(LANDING_IN_PROGRESS));
	}

protected:

	virtual AIStateMachine* makeStateMachine();

	virtual void privateFollowPath( const std::vector<Coord3D>* path, Object *ignoreObject, CommandSourceType cmdSource, bool exitProduction );///< follow the path defined by the given array of points
	virtual void privateFollowPathAppend( const Coord3D *pos, CommandSourceType cmdSource );
	virtual void privateEnter( Object *obj, CommandSourceType cmdSource );							///< enter the given object
	virtual void privateGetRepaired( Object *repairDepot, CommandSourceType cmdSource );///< get repaired at repair depot

	void pruneDeadTargeters();
	void positionLockon();
	
	virtual bool getTreatAsAircraftForLocoDistToGoal() const;
	bool isParkedAt(const Object* obj) const;

private:

	enum FlagType
	{
		HAS_PENDING_COMMAND,
		ALLOW_AIR_LOCO,
		HAS_PRODUCER_LOCATION,
		TAKEOFF_IN_PROGRESS,
		LANDING_IN_PROGRESS,
		USE_SPECIAL_RETURN_LOCO,
		ALLOW_CIRCLING,
		ALLOW_INTERRUPT_AND_RESUME_OF_CUR_STATE_FOR_RELOAD,
		TAXI_IN_PROGRESS
	};

	Coord3D									m_producerLocation;		///< remember this, so that if our producer dies, we have a place to circle aimlessly
	AICommandParmsStorage		m_mostRecentCommand;
	AudioEventRTS						m_afterburnerSound;		///< Sound when afterburners on
	UnsignedInt							m_attackLocoExpireFrame;
	UnsignedInt							m_attackersMissExpireFrame;
	UnsignedInt							m_returnToBaseFrame;	///< if nonzero, return to base at this frame when we are idle, even if not out of ammo
	std::list<ObjectID>			m_targetedBy;					///< object(s) currently targeting us, if any
	UnsignedInt							m_untargetableExpireFrame;
	Drawable*								m_lockonDrawable;
	Int											m_flags;
	Coord3D									m_landingPosForHelipadStuff;
	bool										m_enginesOn;					///<

	void getProducerLocation();
	void buildLockonDrawableIfNecessary();
	void doLandingCommand(Object *airfield, CommandSourceType cmdSource);
	inline bool getFlag(FlagType f) const { return (m_flags & (1<<f)) != 0; }
	inline void setFlag(FlagType f, bool v) { if (v) m_flags |= (1<<f); else m_flags &= ~(1<<f); }
};

#endif

