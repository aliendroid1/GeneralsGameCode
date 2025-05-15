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

// FILE: GameLogic.h //////////////////////////////////////////////////////////////////////////////
// GameLogic singleton class - defines interface to GameLogic methods and objects
// Author: Michael S. Booth, October 2000
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "Common/GameCommon.h"	// ensure we get DUMP_PERF_STATS, or not
#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/STLTypedefs.h"
#include "Common/ObjectStatusTypes.h"
#include "GameNetwork/NetworkDefs.h"
#include "Common/STLTypedefs.h"
#include "GameLogic/Module/UpdateModule.h"	// needed for DIRECT_UPDATEMODULE_ACCESS

/*
	At one time, we distinguished between sleepy and nonsleepy
	update modules, and kept a separate list for each. however,
	now that the bulk of update modules are sleepy, profiling shows
	that there is no real advantage to having the separate list,
	so to simplify the world, I am removing it. If ALLOW_NONSLEEPY_UPDATES
	is still undefined when we ship, please just nuke all the undefed
	code. (srj)
*/
#define NO_ALLOW_NONSLEEPY_UPDATES

// forward declarations
class AudioEventRTS;
class Object;
class Drawable;
class Player;
class ThingTemplate;
class Team;
class CommandList;
class GameMessage;
class LoadScreen;
class WindowLayout;
class TerrainLogic;
class GhostObjectManager;
class CommandButton;
enum BuildableStatus : Int;

typedef const CommandButton* ConstCommandButtonPtr;

// What kind of game we're in.
enum
{
	GAME_SINGLE_PLAYER,
	GAME_LAN,
	GAME_SKIRMISH,
	GAME_REPLAY,
	GAME_SHELL,
	GAME_INTERNET,
	GAME_NONE
};

enum
{
	CRC_CACHED,
	CRC_RECALC
};

/// Function pointers for use by GameLogic callback functions.
typedef void (*GameLogicFuncPtr)( Object *obj, void *userData ); 
typedef std::hash_map<ObjectID, Object *, rts::hash<ObjectID>, rts::equal_to<ObjectID> > ObjectPtrHash;
typedef ObjectPtrHash::const_iterator ObjectPtrIter;


// ------------------------------------------------------------------------------------------------
/**
 * The implementation of GameLogic 
 */
class GameLogic : public SubsystemInterface, public Snapshot
{

public:

	GameLogic( void );
	virtual ~GameLogic();

	// subsytem methods
	virtual void init( void );															///< Initialize or re-initialize the instance
	virtual void reset( void );															///< Reset the logic system
	virtual void update( void );														///< update the world

	void processCommandList( CommandList *list );		///< process the command list

	void prepareNewGame( Int gameMode, GameDifficulty diff, Int rankPoints );						///< prepare for new game 

	void logicMessageDispatcher( GameMessage *msg, 
																			 void *userData );	///< Logic command list processing

	void registerObject( Object *obj );							///< Given an object, register it with the GameLogic and give it a unique ID

	void addObjectToLookupTable( Object *obj );			///< add object ID to hash lookup table
	void removeObjectFromLookupTable( Object *obj );///< remove object ID from hash lookup table

	/// @todo Change this to refer to a Region3D as an extent of the world
	void setWidth( Real width );										///< Sets the width of the world
	Real getWidth( void );													///< Returns the width of the world
	void setHeight( Real height );									///< Sets the height of the world
	Real getHeight( void );													///< Returns the height of the world

	bool isInGameLogicUpdate( void ) const { return m_isInUpdate; }
	UnsignedInt getFrame( void );										///< Returns the current simulation frame number
	UnsignedInt getCRC( Int mode = CRC_CACHED, AsciiString deepCRCFileName = AsciiString::TheEmptyString );		///< Returns the CRC

	void setObjectIDCounter( ObjectID nextObjID ) { m_nextObjID = nextObjID; }
	ObjectID getObjectIDCounter( void ) { return m_nextObjID; }

	//-----------------------------------------------------------------------------------------------
	void setBuildableStatusOverride(const ThingTemplate* tt, BuildableStatus bs);
	bool findBuildableStatusOverride(const ThingTemplate* tt, BuildableStatus& bs) const;

	void setControlBarOverride(const AsciiString& commandSetName, Int slot, ConstCommandButtonPtr commandButton);
	bool findControlBarOverride(const AsciiString& commandSetName, Int slot, ConstCommandButtonPtr& commandButton) const;

	//-----------------------------------------------------------------------------------------------
	/// create an object given the thing template. (Only for use by ThingFactory.)
	Object *friend_createObject( const ThingTemplate *thing, const ObjectStatusMaskType &objectStatusMask, Team *team );
	void destroyObject( Object *obj );							///< Mark object as destroyed for later deletion
	Object *findObjectByID( ObjectID id );								///< Given an ObjectID, return a pointer to the object.
	Object *getFirstObject( void );									///< Returns the "first" object in the world. When used with the object method "getNextObject()", all objects in the world can be iterated.
	ObjectID allocateObjectID( void );							///< Returns a new unique object id

	// super hack
	void startNewGame( bool saveGame );
	void loadMapINI( AsciiString mapName );

	void updateLoadProgress( Int progress );
	void deleteLoadScreen( void );
	
	void setGameLoading( bool loading );
	void setGameMode( Int mode );
	Int getGameMode( void );
	bool isInGame( void );
	bool isInLanGame( void );
	bool isInSinglePlayerGame( void );
	bool isInSkirmishGame( void );
	bool isInReplayGame( void );
	bool isInInternetGame( void );
	bool isInShellGame( void );
	bool isInMultiplayerGame( void );
	bool isLoadingGame();
	void enableScoring(Bool score) { m_isScoringEnabled = score; }
	bool isScoringEnabled() const { return m_isScoringEnabled; }

	void setShowBehindBuildingMarkers(Bool b) { m_showBehindBuildingMarkers = b; }
	bool getShowBehindBuildingMarkers() const { return m_showBehindBuildingMarkers; }

	void setDrawIconUI(Bool b) { m_drawIconUI = b; }
	bool getDrawIconUI() const { return m_drawIconUI; }

	void setShowDynamicLOD(Bool b) { m_showDynamicLOD = b; }
	bool getShowDynamicLOD() const { return m_showDynamicLOD; }

	void setHulkMaxLifetimeOverride(Int b) { m_scriptHulkMaxLifetimeOverride = b; }
	Int getHulkMaxLifetimeOverride() const { return m_scriptHulkMaxLifetimeOverride; }

	bool isIntroMoviePlaying();

	void updateObjectsChangedTriggerAreas(void) {m_frameObjectsChangedTriggerAreas = m_frame;}
	UnsignedInt getFrameObjectsChangedTriggerAreas(void) {return m_frameObjectsChangedTriggerAreas;}

	void clearGameData(Bool showScoreScreen = TRUE);														///< Clear the game data
	void closeWindows( void );

	void sendObjectCreated( Object *obj );
	void sendObjectDestroyed( Object *obj );

	void bindObjectAndDrawable(Object* obj, Drawable* draw);

	void setGamePaused( bool paused, bool pauseMusic = TRUE );
	bool isGamePaused( void );
	bool getInputEnabledMemory( void ) { return m_inputEnabledMemory; }

	void processProgress(Int playerId, Int percentage);
	void processProgressComplete(Int playerId);
	bool isProgressComplete( void );
	void timeOutGameStart( void );
	void initTimeOutValues( void );
	UnsignedInt getObjectCount( void );

	Int getRankLevelLimit() const { return m_rankLevelLimit; }
	void setRankLevelLimit(Int limit) 
	{ 
		if (limit < 1) limit = 1;
		m_rankLevelLimit = limit; 
	}
	
	// We need to allow access to this, because on a restartGame, we need to restart with the settings we started with
	Int getRankPointsToAddAtGameStart() const { return m_rankPointsToAddAtGameStart; }

#ifdef DUMP_PERF_STATS
	void getAIMetricsStatistics( UnsignedInt *numAI, UnsignedInt *numMoving, UnsignedInt *numAttacking, UnsignedInt *numWaitingForPath, UnsignedInt *overallFailedPathfinds );
	void resetOverallFailedPathfinds() { m_overallFailedPathfinds = 0; }
	void incrementOverallFailedPathfinds() { m_overallFailedPathfinds++; }
	UnsignedInt getOverallFailedPathfinds() const { return m_overallFailedPathfinds; }
#endif
	
	// NOTE: selectObject and deselectObject should be called *only* by logical things, NEVER by the
	// client. These will cause the client to select or deselect the object, if affectClient is true.
	// If createToSelection is TRUE, this object causes a new group to be selected.
	void selectObject(Object *obj, bool createNewSelection, PlayerMaskType playerMask, bool affectClient = FALSE);
	void deselectObject(Object *obj, PlayerMaskType playerMask, bool affectClient = FALSE);

	// this should be called only by UpdateModule, thanks.
	void friend_awakenUpdateModule(Object* obj, UpdateModulePtr update, UnsignedInt whenToWakeUp);

protected:

	// snapshot methods
	virtual void crc( Xfer *xfer );
	virtual void xfer( Xfer *xfer );
	virtual void loadPostProcess( void );

private:

	void pushSleepyUpdate(UpdateModulePtr u);
	UpdateModulePtr peekSleepyUpdate() const;
	void popSleepyUpdate();
	void eraseSleepyUpdate(Int i);
	void rebalanceSleepyUpdate(Int i);
	Int rebalanceParentSleepyUpdate(Int i);
	Int rebalanceChildSleepyUpdate(Int i);
	void remakeSleepyUpdate();
	void validateSleepyUpdate() const;

private:

	/**
		overrides to thing template buildable status. doesn't really belong here,
		but has to go somewhere. (srj)
	*/
	typedef std::hash_map< AsciiString, BuildableStatus, rts::hash<AsciiString>, rts::equal_to<AsciiString> > BuildableMap;
	BuildableMap m_thingTemplateBuildableOverrides;

	/**
		overrides to control bars. doesn't really belong here, but has to go somewhere. (srj)
	*/
	typedef std::hash_map< AsciiString, ConstCommandButtonPtr, rts::hash<AsciiString>, rts::equal_to<AsciiString> > ControlBarOverrideMap;
	ControlBarOverrideMap m_controlBarOverrides;

	Real m_width, m_height;																	///< Dimensions of the world
	UnsignedInt m_frame;																		///< Simulation frame number
	
	// CRC cache system -----------------------------------------------------------------------------
	UnsignedInt	m_CRC;																			///< Cache of previous CRC value
	std::map<Int, UnsignedInt> m_cachedCRCs;								///< CRCs we've seen this frame
	bool m_shouldValidateCRCs;															///< Should we validate CRCs this frame?
	//-----------------------------------------------------------------------------------------------

	//Added By Sadullah Nader
	//Used to for load scene
	bool m_loadingScene;

	bool m_isInUpdate;

	Int m_rankPointsToAddAtGameStart;

	bool m_isScoringEnabled;
	bool m_showBehindBuildingMarkers;	//used by designers to override the user setting for cinematics
	bool m_drawIconUI;
	bool m_showDynamicLOD;	//used by designers to override the user setting for cinematics
	Int m_scriptHulkMaxLifetimeOverride;	///< Scripts can change the lifetime of a hulk -- defaults to off (-1) in frames.

	/// @todo remove this hack
	bool m_startNewGame;
	WindowLayout *m_background;

	Object* m_objList;																			///< All of the objects in the world.
	ObjectPtrHash m_objHash;																///< Used for ObjectID lookups

	// this is a vector, but is maintained as a priority queue.
	// never modify it directly; please use the proper access methods.
	// (for an excellent discussion of priority queues, please see:
	// http://dogma.net/markn/articles/pq_stl/priority.htm)
	std::vector<UpdateModulePtr> m_sleepyUpdates;
	
#ifdef ALLOW_NONSLEEPY_UPDATES
	// this is a plain old list, not a pq.
	std::list<UpdateModulePtr> m_normalUpdates;
#endif

	UpdateModulePtr					 m_curUpdateModule;

	ObjectPointerList m_objectsToDestroy;										///< List of things that need to be destroyed at end of frame

	ObjectID m_nextObjID;																		///< For allocating object id's

	void setDefaults( bool saveGame );											///< Set default values of class object
	void processDestroyList( void );												///< Destroy all pending objects on the destroy list

	void destroyAllObjectsImmediate();											///< destroy, and process destroy list immediately

	/// factory for TheTerrainLogic, called from init()
	virtual TerrainLogic *createTerrainLogic( void );
	virtual GhostObjectManager *createGhostObjectManager(void);

	Int m_gameMode;
	Int m_rankLevelLimit;

	LoadScreen *getLoadScreen( bool saveGame );
	LoadScreen *m_loadScreen;
	bool m_gamePaused;
	bool m_inputEnabledMemory;// Latches used to remember what to restore to after we unpause
	bool m_mouseVisibleMemory;

	bool m_progressComplete[MAX_SLOTS];
	enum { PROGRESS_COMPLETE_TIMEOUT = 60000 };							///< Timeout we wait for when we've completed our Load
	Int m_progressCompleteTimeout[MAX_SLOTS];
	void testTimeOut( void );
	void lastHeardFrom( Int playerId );
	bool m_forceGameStartByTimeOut;													///< If we timeout someone we're waiting to load, set this flag to start the game

#ifdef DUMP_PERF_STATS
	UnsignedInt m_overallFailedPathfinds;
#endif

	UnsignedInt m_frameObjectsChangedTriggerAreas;					///< Last frame objects moved into/outof trigger areas, or were created/destroyed. jba.

	// ----------------------------------------------------------------------------------------------
	struct ObjectTOCEntry
	{
		AsciiString name;
		UnsignedShort id;
	};
	typedef std::list< ObjectTOCEntry > ObjectTOCList;
	typedef ObjectTOCList::iterator ObjectTOCListIterator;
	ObjectTOCList m_objectTOC;															///< the object TOC
	void addTOCEntry( AsciiString name, UnsignedShort id ); ///< add a new name/id TOC pair
	ObjectTOCEntry *findTOCEntryByName( AsciiString name );	///< find ObjectTOC by name
	ObjectTOCEntry *findTOCEntryById( UnsignedShort id );		///< find ObjectTOC by id
	void xferObjectTOC( Xfer *xfer );												///< save/load object TOC for current state of map
	void prepareLogicForObjectLoad( void );									///< prepare engine for object data from game file
		
};

// INLINE /////////////////////////////////////////////////////////////////////////////////////////
inline void GameLogic::setWidth( Real width ) { m_width = width; }
inline Real GameLogic::getWidth( void ) { return m_width; }
inline void GameLogic::setHeight( Real height ) { m_height = height; }
inline Real GameLogic::getHeight( void ) { return m_height; }
inline UnsignedInt GameLogic::getFrame( void ) { return m_frame; }

inline bool GameLogic::isInGame( void ) { return !(m_gameMode == GAME_NONE); }
inline void GameLogic::setGameMode( Int mode ) { m_gameMode = mode; }
inline Int  GameLogic::getGameMode( void ) { return m_gameMode; }
inline bool GameLogic::isInLanGame( void ) { return (m_gameMode == GAME_LAN); }
inline bool GameLogic::isInSkirmishGame( void ) { return (m_gameMode == GAME_SKIRMISH); }
inline bool GameLogic::isInMultiplayerGame( void ) { return ((m_gameMode == GAME_LAN) || (m_gameMode == GAME_INTERNET)) ; }
inline bool GameLogic::isInReplayGame( void ) { return (m_gameMode == GAME_REPLAY); }
inline bool GameLogic::isInInternetGame( void ) { return (m_gameMode == GAME_INTERNET); }
inline bool GameLogic::isInShellGame( void ) { return (m_gameMode == GAME_SHELL); }
//Check for loading scene
inline bool GameLogic::isLoadingGame(){ return m_loadingScene;}

inline Object* GameLogic::findObjectByID( ObjectID id )
{
	if( id == INVALID_ID )
		return NULL;

	ObjectPtrHash::iterator it = m_objHash.find(id);
	if (it == m_objHash.end())
		return NULL;
	
	return (*it).second;
}



// the singleton
extern GameLogic *TheGameLogic;

#endif // _GAME_LOGIC_H_

