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

// FILE: ScriptConditions.h ///////////////////////////////////////////////////////////////////////////
// Script conditions evaluator for the scripting engine.
// Author: John Ahlquist, Nov 2001
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SCRIPTCONDITIONS_H_
#define __SCRIPTCONDITIONS_H_

class Condition;
class ObjectTypes;
class Parameter;
class Player;
//-----------------------------------------------------------------------------
// ScriptConditionsInterface
//-----------------------------------------------------------------------------
/** Pure virtual class for Script Conditions interface format */
//-----------------------------------------------------------------------------
class ScriptConditionsInterface : public SubsystemInterface
{

public:

	virtual ~ScriptConditionsInterface() { };

	virtual void init( void ) = 0;		///< Init
	virtual void reset( void ) = 0;		///< Reset
	virtual void update( void ) = 0;	///< Update

	virtual bool evaluateCondition( Condition *pCondition ) = 0; ///< evaluate a a script condition.

	virtual bool evaluateSkirmishCommandButtonIsReady( Parameter *pSkirmishPlayerParm, Parameter *pTeamParm, Parameter *pCommandButtonParm, bool allReady ) = 0;
	virtual bool evaluateTeamIsContained(Parameter *pTeamParm, bool allContained) = 0;

};  // end class ScriptConditionsInterface
extern ScriptConditionsInterface *TheScriptConditions;   ///< singleton definition


//-----------------------------------------------------------------------------
// ScriptConditions
//-----------------------------------------------------------------------------
/** Implementation for the Script Conditions singleton */
//-----------------------------------------------------------------------------
class ScriptConditions : public ScriptConditionsInterface
{

public:
	ScriptConditions();
	~ScriptConditions();

public:

	virtual void init( void );		///< Init
	virtual void reset( void );		///< Reset
	virtual void update( void );	///< Update

	bool evaluateCondition( Condition *pCondition );

protected:
	Player *playerFromParam(Parameter *pSideParm);			// Gets a player from a parameter.
	void objectTypesFromParam(Parameter *pTypeParm, ObjectTypes *outObjectTypes);		// Must pass in a valid objectTypes for outObjectTypes

	bool evaluateAllDestroyed(Parameter *pSideParm);
	bool evaluateAllBuildFacilitiesDestroyed(Parameter *pSideParm);
	bool evaluateIsDestroyed(Parameter *pTeamParm);
	bool evaluateBridgeBroken(Parameter *pBridgeParm);
	bool evaluateBridgeRepaired(Parameter *pBridgeParm);
	bool evaluateNamedUnitDestroyed(Parameter *pUnitParm);
	bool evaluateNamedUnitExists(Parameter *pUnitParm);
	bool evaluateNamedUnitDying(Parameter *pUnitParm);
	bool evaluateNamedUnitTotallyDead(Parameter *pUnitParm);
	bool evaluateHasUnits(Parameter *pTeamParm);
	
	bool evaluateTeamEnteredAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	bool evaluateTeamEnteredAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	bool evaluateTeamExitedAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	bool evaluateTeamExitedAreaPartially(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	bool evaluateTeamInsideAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	bool evaluateTeamInsideAreaPartially(Parameter *pUnitParm, Parameter *pTriggerParm, Parameter *pTypeParm);
	bool evaluateTeamOutsideAreaEntirely(Parameter *pTeamParm, Parameter *pTriggerParm, Parameter *pTypeParm);

	bool evaluateNamedEnteredArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	bool evaluateNamedExitedArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	bool evaluateNamedInsideArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	bool evaluateNamedOutsideArea(Parameter *pUnitParm, Parameter *pTriggerParm);
	
	bool evaluateTeamStateIs(Parameter *pTeamParm, Parameter *pStateParm);
	bool evaluateTeamStateIsNot(Parameter *pTeamParm, Parameter *pStateParm);
	bool evaluatePlayerHasCredits(Parameter *pCreditsParm, Parameter* pComparisonParm, Parameter *pPlayerParm);
	bool evaluateNamedCreated(Parameter* pUnitParm);	///< Implemented as evaluateNamedExists(...)
	bool evaluateTeamCreated(Parameter* pTeamParm);		///< Implemented as evaluateTeamExists(...)
	bool evaluateNamedOwnedByPlayer(Parameter *pUnitParm, Parameter *pPlayerParm);
	bool evaluateTeamOwnedByPlayer(Parameter *pTeamParm, Parameter *pPlayerParm);
	bool evaluateMultiplayerAlliedVictory(void);
	bool evaluateMultiplayerAlliedDefeat(void);
	bool evaluateMultiplayerPlayerDefeat(void);
	bool evaluateNamedAttackedByType(Parameter *pUnitParm, Parameter *pTypeParm);
	bool evaluateTeamAttackedByType(Parameter *pTeamParm, Parameter *pTypeParm);
	bool evaluateNamedAttackedByPlayer(Parameter *pUnitParm, Parameter *pPlayerParm);
	bool evaluateTeamAttackedByPlayer(Parameter *pTeamParm, Parameter *pPlayerParm);
	bool evaluateBuiltByPlayer(Condition *pCondition, Parameter* pTypeParm, Parameter* pPlayerParm);
	bool evaluatePlayerHasNOrFewerBuildings(Parameter *pBuildingCountParm, Parameter *pPlayerParm);
	bool evaluatePlayerHasNOrFewerFactionBuildings(Parameter *pBuildingCountParm, Parameter *pPlayerParm);
	bool evaluatePlayerHasPower(Parameter *pPlayerParm);
	bool evaluateNamedReachedWaypointsEnd(Parameter *pUnitParm, Parameter* pWaypointPathParm);
	bool evaluateTeamReachedWaypointsEnd(Parameter *pTeamParm, Parameter* pWaypointPathParm);
	bool evaluateNamedSelected(Condition *pCondition, Parameter *pUnitParm);
	bool evaluateVideoHasCompleted(Parameter *pVideoParm);
	bool evaluateSpeechHasCompleted(Parameter *pSpeechParm);
	bool evaluateAudioHasCompleted(Parameter *pAudioParm);
	bool evaluateNamedDiscovered(Parameter *pItemParm, Parameter* pPlayerParm);
	bool evaluateTeamDiscovered(Parameter *pTeamParm, Parameter *pPlayerParm);
	bool evaluateBuildingEntered(Parameter *pItemParm, Parameter *pUnitParm );
	bool evaluateIsBuildingEmpty(Parameter *pItemParm);
	bool evaluateEnemySighted(Parameter *pItemParm, Parameter *pAllianceParm, Parameter* pPlayerParm);
	bool evaluateTypeSighted(Parameter *pItemParm, Parameter *pTypeParm, Parameter* pPlayerParm);
	bool evaluateUnitHealth(Parameter *pItemParm, Parameter* pComparisonParm, Parameter *pHealthPercent);
	bool evaluatePlayerUnitCondition(Condition *pCondition, Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pCountParm, Parameter *pUnitTypeParm);
	bool evaluatePlayerSpecialPowerFromUnitTriggered(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter* pUnitParm);
	bool evaluatePlayerSpecialPowerFromUnitMidway		(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter* pUnitParm);
	bool evaluatePlayerSpecialPowerFromUnitComplete	(Parameter *pPlayerParm, Parameter *pSpecialPowerParm, Parameter* pUnitParm);
	bool evaluateUpgradeFromUnitComplete						(Parameter *pPlayerParm, Parameter *pUpgradeParm,			 Parameter* pUnitParm);
	bool evaluateScienceAcquired										(Parameter *pPlayerParm, Parameter *pScienceParm);
	bool evaluateCanPurchaseScience									(Parameter *pPlayerParm, Parameter *pScienceParm);
	bool evaluateSciencePurchasePoints							(Parameter *pPlayerParm, Parameter *pSciencePointParm);
	bool evaluatePlayerDestroyedNOrMoreBuildings(Parameter *pPlayerParm, Parameter *pNumParm, Parameter *pOppenentParm);
	bool evaluateUnitHasObjectStatus(Parameter *pUnitParm, Parameter *pObjectStatus);
	bool evaluateTeamHasObjectStatus(Parameter *pTeamParm, Parameter *pObjectStatus, bool entireTeam);
	bool evaluatePlayerHasComparisonPercentPower(Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pPercent);
	bool evaluatePlayerHasComparisonValueExcessPower(Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pKWHParm);
	bool evaluatePlayerHasUnitTypeInArea(Condition *pCondition, Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pCountParm, Parameter *pTypeParm, Parameter *pTriggerParm);
	bool evaluatePlayerHasUnitKindInArea(Condition *pCondition, Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pCountParm,Parameter *pKindParm, Parameter *pTriggerParm);
	bool evaluateUnitHasEmptied(Parameter *pUnitParm);
	bool evaluateTeamIsContained(Parameter *pTeamParm, bool allContained);
	bool evaluateMusicHasCompleted(Parameter *pMusicParm, Parameter *pIntParm);
	bool evaluatePlayerLostObjectType(Parameter *pPlayerParm, Parameter *pTypeParm);

	// Skirmish Scripts. Please note that ALL Skirmish conditions should first pass a pSkirmishPlayerParm to 
	// prevent the necessity of having to write additional scripts for other players / skirmish types later.
	bool evaluateSkirmishSpecialPowerIsReady(Parameter *pSkirmishPlayerParm, Parameter *pPower);
	bool evaluateSkirmishValueInArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pComparisonParm, Parameter *pMoneyParm, Parameter *pTriggerParm);
	bool evaluateSkirmishPlayerIsFaction(Parameter *pSkirmishPlayerParm, Parameter *pFactionParm);
	bool evaluateSkirmishSuppliesWithinDistancePerimeter(Parameter *pSkirmishPlayerParm, Parameter *pDistanceParm, Parameter *pLocationParm, Parameter *pValueParm);
	bool evaluateSkirmishPlayerTechBuildingWithinDistancePerimeter(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pDistanceParm, Parameter *pLocationParm);
	bool evaluateSkirmishCommandButtonIsReady( Parameter *pSkirmishPlayerParm, Parameter *pTeamParm, Parameter *pCommandButtonParm, bool allReady );
	bool evaluateSkirmishUnownedFactionUnitComparison( Parameter *pSkirmishPlayerParm, Parameter *pComparisonParm, Parameter *pCountParm );
	bool evaluateSkirmishPlayerHasPrereqsToBuild( Parameter *pSkirmishPlayerParm, Parameter *pObjectTypeParm );
	bool evaluateSkirmishPlayerHasComparisonGarrisoned(Parameter *pSkirmishPlayerParm, Parameter *pComparisonParm, Parameter *pCountParm );
	bool evaluateSkirmishPlayerHasComparisonCapturedUnits(Parameter *pSkirmishPlayerParm, Parameter *pComparisonParm, Parameter *pCountParm );
	bool evaluateSkirmishNamedAreaExists(Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm);
	bool evaluateSkirmishPlayerHasUnitsInArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm );
	bool evaluateSkirmishPlayerHasBeenAttackedByPlayer(Parameter *pSkirmishPlayerParm, Parameter *pAttackedByParm );
	bool evaluateSkirmishPlayerIsOutsideArea(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pTriggerParm );
	bool evaluateSkirmishPlayerHasDiscoveredPlayer(Parameter *pSkirmishPlayerParm, Parameter *pDiscoveredByParm );
	bool evaluateSkirmishSupplySourceSafe(Condition *pCondition, Parameter *pSkirmishPlayerParm, Parameter *pMinAmountOfSupplies );
	bool evaluateSkirmishSupplySourceAttacked(Parameter *pSkirmishPlayerParm );
	bool evaluateSkirmishStartPosition(Parameter *pSkirmishPlayerParm, Parameter *startNdx );

	
	// Stubs
	bool evaluateMissionAttempts(Parameter *pPlayerParm, Parameter *pComparisonParm, Parameter *pAttemptsParm);


};  // end class ScriptConditions


#endif  // end __SCRIPTCONDITIONS_H_
