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

// FILE: GlobalData.h /////////////////////////////////////////////////////////////////////////////
// Global data used by both the client and logic
// Author: trolfs, Michae Booth, Colin Day, April 2001
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_

#include "Common/GameCommon.h"	// ensure we get DUMP_PERF_STATS, or not
#include "Common/AsciiString.h"
#include "Common/GameType.h"
#include "Common/GameMemory.h"
#include "Common/SubsystemInterface.h"
#include "GameClient/Color.h"
#include "Common/STLTypedefs.h"

// FORWARD DECLARATIONS ///////////////////////////////////////////////////////////////////////////
struct FieldParse;
enum _TerrainLOD : Int;
class GlobalData;
class INI;
class WeaponBonusSet;
enum BodyDamageType : Int;
enum AIDebugOptions : Int;

// PUBLIC /////////////////////////////////////////////////////////////////////////////////////////

const Int MAX_GLOBAL_LIGHTS	= 3;

//-------------------------------------------------------------------------------------------------
/** Global data container class
  *	Defines all global game data used by the system
	* @todo Change this entire system. Otherwise this will end up a huge class containing tons of variables,
	* and will cause re-compilation dependancies throughout the codebase. */
//-------------------------------------------------------------------------------------------------
class GlobalData : public SubsystemInterface
{

public:

	GlobalData();
	virtual ~GlobalData();

	void init();
	void reset();
	void update() { }

	bool setTimeOfDay( TimeOfDay tod );		///< Use this function to set the Time of day;

	static void parseGameDataDefinition( INI* ini );

	//-----------------------------------------------------------------------------------------------
	struct TerrainLighting
	{
		RGBColor ambient;
		RGBColor diffuse;
		Coord3D lightPos;
	};

	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------

	AsciiString m_mapName;  ///< hack for now, this whole this is going away
	AsciiString m_moveHintName;
	bool m_useTrees;
	bool m_useTreeSway;
	bool m_useDrawModuleLOD;
	bool m_useFpsLimit;
	bool m_dumpAssetUsage;
	Int m_framesPerSecondLimit;
	Int	m_chipSetType;	///<See W3DShaderManager::ChipsetType for options
	bool m_windowed;
	Int m_xResolution;
	Int m_yResolution;
	Int m_maxShellScreens;  ///< this many shells layouts can be loaded at once
	bool m_useCloudMap;
	Int  m_use3WayTerrainBlends;	///< 0 is none, 1 is normal, 2 is debug.
	bool m_useLightMap;
	bool m_bilinearTerrainTex;
	bool m_trilinearTerrainTex;
	bool m_multiPassTerrain;
	bool m_adjustCliffTextures;
	bool m_stretchTerrain;
	bool m_useHalfHeightMap;
	bool m_drawEntireTerrain;
	_TerrainLOD m_terrainLOD;
	bool m_enableDynamicLOD;
	bool m_enableStaticLOD;
	Int m_terrainLODTargetTimeMS;
	bool m_useAlternateMouse;
	bool m_rightMouseAlwaysScrolls;
	bool m_useWaterPlane;
	bool m_useCloudPlane;
	bool m_useShadowVolumes;
	bool m_useShadowDecals;
	Int  m_textureReductionFactor;	//how much to cut texture resolution: 2 is half, 3 is quarter, etc.
	bool m_enableBehindBuildingMarkers;
	Real m_waterPositionX;
	Real m_waterPositionY;
	Real m_waterPositionZ;
	Real m_waterExtentX;	
	Real m_waterExtentY;
	Int	m_waterType;
	bool m_showSoftWaterEdge;
	bool m_usingWaterTrackEditor;

	Int m_featherWater;

	// these are for WATER_TYPE_3 vertex animated water
	enum { MAX_WATER_GRID_SETTINGS = 4 };
	AsciiString m_vertexWaterAvailableMaps[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterHeightClampLow[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterHeightClampHi[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterAngle[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterXPosition[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterYPosition[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterZPosition[ MAX_WATER_GRID_SETTINGS ];
	Int m_vertexWaterXGridCells[ MAX_WATER_GRID_SETTINGS ];
	Int m_vertexWaterYGridCells[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterGridSize[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterAttenuationA[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterAttenuationB[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterAttenuationC[ MAX_WATER_GRID_SETTINGS ];
	Real m_vertexWaterAttenuationRange[ MAX_WATER_GRID_SETTINGS ];

	Real m_downwindAngle;
	Real m_skyBoxPositionZ;
	Real m_drawSkyBox;
	Real m_skyBoxScale;
	Real m_cameraPitch;
	Real m_cameraYaw;
	Real m_cameraHeight;
	Real m_maxCameraHeight;
	Real m_minCameraHeight;
	Real m_terrainHeightAtEdgeOfMap;
	Real m_unitDamagedThresh;
	Real m_unitReallyDamagedThresh;
	Real m_groundStiffness;
	Real m_structureStiffness;
	Real m_gravity;	// acceleration due to gravity, in dist/frame^2
	Real m_stealthFriendlyOpacity;
	UnsignedInt m_defaultOcclusionDelay;	///<time to delay building occlusion after object is created.

	bool m_preloadAssets;
	bool m_preloadEverything;			///< Preload everything, everywhere (for debugging only)
	bool m_preloadReport;					///< dump a log of all W3D assets that are being preloaded.

	Real m_partitionCellSize;

	Coord3D m_ammoPipWorldOffset;
	Coord3D m_containerPipWorldOffset;
	Coord2D m_ammoPipScreenOffset;
	Coord2D m_containerPipScreenOffset;
	Real m_ammoPipScaleFactor;
	Real m_containerPipScaleFactor;

	UnsignedInt m_historicDamageLimit;

	//Settings for terrain tracks left by vehicles with treads or wheels
	Int m_maxTerrainTracks; ///<maximum number of objects allowed to generate tracks.
	Int m_maxTankTrackEdges;	///<maximum length of tank track
	Int m_maxTankTrackOpaqueEdges;	///<maximum length of tank track before it starts fading.
	Int m_maxTankTrackFadeDelay;	///<maximum amount of time a tank track segment remains visible.
	
	AsciiString m_levelGainAnimationName; ///< The animation to play when a level is gained.
	Real m_levelGainAnimationDisplayTimeInSeconds;		///< time to play animation for
	Real m_levelGainAnimationZRisePerSecond;					///< rise animation up while playing

	AsciiString m_getHealedAnimationName; ///< The animation to play when emergency repair does its thing.
	Real m_getHealedAnimationDisplayTimeInSeconds;		///< time to play animation for
	Real m_getHealedAnimationZRisePerSecond;					///< rise animation up while playing

	TimeOfDay	m_timeOfDay;
	Weather m_weather;
	bool m_makeTrackMarks;
	bool m_hideGarrisonFlags;
	bool m_forceModelsToFollowTimeOfDay;
	bool m_forceModelsToFollowWeather;

	TerrainLighting	m_terrainLighting[TIME_OF_DAY_COUNT][MAX_GLOBAL_LIGHTS];
	TerrainLighting	m_terrainObjectsLighting[TIME_OF_DAY_COUNT][MAX_GLOBAL_LIGHTS];

	//Settings for each global light
	RGBColor m_terrainAmbient[MAX_GLOBAL_LIGHTS];
	RGBColor m_terrainDiffuse[MAX_GLOBAL_LIGHTS];
	Coord3D m_terrainLightPos[MAX_GLOBAL_LIGHTS];

	Real m_infantryLightScale[TIME_OF_DAY_COUNT];
	Real m_scriptOverrideInfantryLightScale;

	Real m_soloPlayerHealthBonusForDifficulty[PLAYERTYPE_COUNT][DIFFICULTY_COUNT];

	Int m_maxVisibleTranslucentObjects;
	Int m_maxVisibleOccluderObjects;
	Int m_maxVisibleOccludeeObjects;
	Int m_maxVisibleNonOccluderOrOccludeeObjects;
	Real m_occludedLuminanceScale;

	Int m_numGlobalLights;	//number of active global lights
	Int m_maxRoadSegments;
	Int m_maxRoadVertex;
	Int m_maxRoadIndex;
	Int m_maxRoadTypes;

	bool m_audioOn;
	bool m_musicOn;
	bool m_soundsOn;
	bool m_sounds3DOn;
	bool m_speechOn;
	bool m_videoOn;
	bool m_disableCameraMovement;

	bool m_useFX;									///< If false, don't render effects
	bool m_showClientPhysics;
	bool m_showTerrainNormals;

	UnsignedInt m_noDraw;					///< Used to disable drawing, to profile game logic code.
	AIDebugOptions m_debugAI;			///< Used to display AI debug information
	bool m_debugSupplyCenterPlacement; ///< Dumps to log everywhere it thinks about placing a supply center
	bool m_debugAIObstacles;			///< Used to display AI obstacle debug information
	bool m_showObjectHealth;			///< debug display object health
	bool m_scriptDebug;						///< Should we attempt to load the script debugger window (.DLL)
	bool m_particleEdit;					///< Should we attempt to load the particle editor (.DLL)
	bool m_displayDebug;					///< Used to display display debug info
	bool m_winCursors;						///< Should we force use of windows cursors?
	bool m_constantDebugUpdate;		///< should we update the debug stats constantly, vs every 2 seconds?
	bool m_showTeamDot;						///< Shows the little colored team dot representing which team you are controlling.
	
#ifdef DUMP_PERF_STATS
	bool m_dumpPerformanceStatistics;
#endif
	
	bool m_forceBenchmark;	///<forces running of CPU detection benchmark, even on known cpu's.

	Int m_fixedSeed;							///< fixed random seed for game logic (less than 0 to disable)

	Real m_particleScale;					///< Global size modifier for particle effects.

	AsciiString m_autoFireParticleSmallPrefix;
	AsciiString m_autoFireParticleSmallSystem;
	Int m_autoFireParticleSmallMax;
	AsciiString m_autoFireParticleMediumPrefix;
	AsciiString m_autoFireParticleMediumSystem;
	Int m_autoFireParticleMediumMax;
	AsciiString m_autoFireParticleLargePrefix;
	AsciiString m_autoFireParticleLargeSystem;
	Int m_autoFireParticleLargeMax;
	AsciiString m_autoSmokeParticleSmallPrefix;
	AsciiString m_autoSmokeParticleSmallSystem;
	Int m_autoSmokeParticleSmallMax;
	AsciiString m_autoSmokeParticleMediumPrefix;
	AsciiString m_autoSmokeParticleMediumSystem;
	Int m_autoSmokeParticleMediumMax;
	AsciiString m_autoSmokeParticleLargePrefix;
	AsciiString m_autoSmokeParticleLargeSystem;
	Int m_autoSmokeParticleLargeMax;
	AsciiString m_autoAflameParticlePrefix;
	AsciiString m_autoAflameParticleSystem;
	Int m_autoAflameParticleMax;

	// Latency insertion, packet loss for network debugging
	Int m_netMinPlayers;					///< Min players needed to start a net game

	UnsignedInt m_defaultIP;			///< preferred IP address for LAN
	UnsignedInt m_firewallBehavior;	///< Last detected firewall behavior
	bool m_firewallSendDelay;			///< Use send delay for firewall connection negotiations
	UnsignedInt m_firewallPortOverride;	///< User-specified port to be used
	Short m_firewallPortAllocationDelta; ///< the port allocation delta last detected.

	Int m_baseValuePerSupplyBox;
	Real m_BuildSpeed;
	Real m_MinDistFromEdgeOfMapForBuild;
	Real m_SupplyBuildBorder;
	Real m_allowedHeightVariationForBuilding;  ///< how "flat" is still flat enough to build on
	Real m_MinLowEnergyProductionSpeed;
	Real m_MaxLowEnergyProductionSpeed;
	Real m_LowEnergyPenaltyModifier;
	Real m_MultipleFactory;
	Real m_RefundPercent;

	Real m_commandCenterHealRange;		///< radius in which close by ally things are healed
	Real m_commandCenterHealAmount;   ///< health per logic frame close by things are healed
	Int m_maxLineBuildObjects;				///< line style builds can be no longer than this
	Int m_maxTunnelCapacity;					///< Max people in Player's tunnel network
	Real m_horizontalScrollSpeedFactor;	///< Factor applied to the game screen scrolling speed.
	Real m_verticalScrollSpeedFactor;		///< Seperated because of our aspect ratio
	Real m_scrollAmountCutoff;				///< Scroll speed to not adjust camera height
	Real m_cameraAdjustSpeed;					///< Rate at which we adjust camera height
	bool m_enforceMaxCameraHeight;		///< Enfoce max camera height while scrolling?
	bool m_buildMapCache;
	AsciiString m_initialFile;				///< If this is specified, load a specific map/replay from the command-line
	AsciiString m_pendingFile;				///< If this is specified, use this map at the next game start

	Int m_maxParticleCount;						///< maximum number of particles that can exist
	Int m_maxFieldParticleCount;			///< maximum number of field-type particles that can exist (roughly)
	WeaponBonusSet* m_weaponBonusSet;
	Real m_healthBonus[LEVEL_COUNT];			///< global bonuses to health for veterancy.
	Real m_defaultStructureRubbleHeight;	///< for rubbled structures, compress height to this if none specified

	AsciiString m_shellMapName;				///< Holds the shell map name
	bool m_shellMapOn;								///< User can set the shell map not to load
	bool m_playIntro;									///< Flag to say if we're to play the intro or not
	bool m_playSizzle;								///< Flag to say whether we play the sizzle movie after the logo movie.
	bool m_afterIntro;								///< we need to tell the game our intro is done
	bool m_allowExitOutOfMovies;			///< flag to allow exit out of movies only after the Intro has played

	bool m_loadScreenRender;						///< flag to disallow rendering of almost everything during a loadscreen

	Real m_keyboardScrollFactor;			///< Factor applied to game scrolling speed via keyboard scrolling
	Real m_keyboardDefaultScrollFactor;			///< Factor applied to game scrolling speed via keyboard scrolling
	
  Real m_musicVolumeFactor;         ///< Factor applied to loudness of music volume
  Real m_SFXVolumeFactor;           ///< Factor applied to loudness of SFX volume
  Real m_voiceVolumeFactor;         ///< Factor applied to loudness of voice volume
  bool m_3DSoundPref;               ///< Whether user wants to use 3DSound or not

	bool m_animateWindows;						///< Should we animate window transitions?

	bool m_incrementalAGPBuf;
	
	UnsignedInt m_iniCRC;							///< CRC of important INI files
	UnsignedInt m_exeCRC;							///< CRC of the executable

	BodyDamageType m_movementPenaltyDamageState;	///< at this body damage state, we have movement penalties

	Int m_groupSelectMinSelectSize;		// min number of units to treat as group select for audio feedback
	Real m_groupSelectVolumeBase;			// base volume for group select sound
	Real m_groupSelectVolumeIncrement;// increment to volume for selecting more units
	Int m_maxUnitSelectSounds;				// max number of select sounds to play per selection

	Real m_selectionFlashSaturationFactor; /// how colorful should the selection flash be? 0-4
	bool m_selectionFlashHouseColor ;  /// skip the house color and just use white.

	Real m_cameraAudibleRadius;				///< If the camera is being used as the position of audio, then how far can we hear?
	Real m_groupMoveClickToGatherFactor; /** if you take all the selected units and calculate the smallest possible rectangle 
																			 that contains them all, and click within that, all the selected units will break 
																			 formation and gather at the point the user clicked (if the value is 1.0). If it's 0.0,
																			 units will always keep their formation. If it's <1.0, then the user must click a 
																			 smaller area within the rectangle to order the gather. */

	Int m_antiAliasBoxValue;          ///< value of selected antialias from combo box in options menu
	bool m_languageFilterPref;        ///< bool if user wants to filter language
	bool m_loadScreenDemo;						///< bool if true, run the loadscreen demo movie
	bool m_disableRender;							///< if true, no rendering!

	bool m_saveCameraInReplay;
	bool m_useCameraInReplay;

	Real m_shakeSubtleIntensity;			///< Intensity for shaking a camera with SHAKE_SUBTLE
	Real m_shakeNormalIntensity;			///< Intensity for shaking a camera with SHAKE_NORMAL
	Real m_shakeStrongIntensity;			///< Intensity for shaking a camera with SHAKE_STRONG
	Real m_shakeSevereIntensity;			///< Intensity for shaking a camera with SHAKE_SEVERE
	Real m_shakeCineExtremeIntensity; ///< Intensity for shaking a camera with SHAKE_CINE_EXTREME
	Real m_shakeCineInsaneIntensity;  ///< Intensity for shaking a camera with SHAKE_CINE_INSANE
	Real m_maxShakeIntensity;					///< The maximum shake intensity we can have
	Real m_maxShakeRange;							///< The maximum shake range we can have

	Real m_sellPercentage;						///< when objects are sold, you get this much of the cost it would take to build it back
	Real m_baseRegenHealthPercentPerSecond;	///< auto healing for bases
	UnsignedInt m_baseRegenDelay;			///< delay in frames we must be damage free before we can auto heal

#ifdef ALLOW_SURRENDER
	Real m_prisonBountyMultiplier;		///< the cost of the unit is multiplied by this and given to the player when prisoners are returned to the a prison with KINDOF_COLLECTS_PRISON_BOUNTY
	Color m_prisonBountyTextColor;		///< color of the text that displays the money acquired at the prison
#endif

	Color m_hotKeyTextColor;					///< standard color for all hotkeys.
	
	AsciiString m_specialPowerViewObjectName;	///< Created when certain special powers are fired so players can watch.

	std::vector<AsciiString> m_standardPublicBones;

	Real m_standardMinefieldDensity;
	Real m_standardMinefieldDistance;

	
	bool m_showMetrics;								///< whether or not to show the metrics.
	Int m_defaultStartingCash;				///< The amount of cash a player starts with by default.
	
	bool m_debugShowGraphicalFramerate;		///< Whether or not to show the graphical framerate bar.

	Int m_powerBarBase;										///< Logrithmic base for the power bar scale
	Real m_powerBarIntervals;							///< how many logrithmic intervals the width will be divided into
	Int m_powerBarYellowRange;						///< Red if consumption exceeds production, yellow if consumption this close but under, green if further under
	Real m_displayGamma;									///<display gamma that's adjusted with "brightness" control on options screen.

	UnsignedInt m_unlookPersistDuration;	///< How long after unlook until the sighting info executes the undo

	bool m_shouldUpdateTGAToDDS;					///< Should we attempt to update old TGAs to DDS stuff on loadup?
	
	UnsignedInt m_doubleClickTimeMS;	///< What is the maximum amount of time that can seperate two clicks in order
																		///< for us to generate a double click message?

	RGBColor m_shroudColor;						///< What color should the shroud be?  Remember, this is a lighting multiply, not an add
	UnsignedByte m_clearAlpha;				///< 255 means perfect visibility
	UnsignedByte m_fogAlpha;					///< 127 means fog is half as obscuring as shroud
	UnsignedByte m_shroudAlpha;				///< 0 makes this opaque, but they may get fancy

	// network timing values.
	UnsignedInt m_networkFPSHistoryLength;			///< The number of fps history entries
	UnsignedInt m_networkLatencyHistoryLength;	///< The number of ping history entries.
	UnsignedInt m_networkRunAheadMetricsTime;		///< The number of miliseconds between run ahead metrics things
	UnsignedInt m_networkCushionHistoryLength;	///< The number of cushion values to keep.
	UnsignedInt m_networkRunAheadSlack;					///< The amount of slack in the run ahead value.  This is the percentage of the calculated run ahead that is added.
	UnsignedInt m_networkKeepAliveDelay;				///< The number of seconds between when the connections to each player send a keep-alive packet.
	UnsignedInt m_networkDisconnectTime;				///< The number of milliseconds between when the game gets stuck on a frame for a network stall and when the disconnect dialog comes up.
	UnsignedInt m_networkPlayerTimeoutTime;			///< The number of milliseconds between when a player's last keep alive command was recieved and when they are considered disconnected from the game.
	UnsignedInt	m_networkDisconnectScreenNotifyTime; ///< The number of milliseconds between when the disconnect screen comes up and when the other players are notified that we are on the disconnect screen.
	
	Real				m_keyboardCameraRotateSpeed;    ///< How fast the camera rotates when rotated via keyboard controls.
  Int					m_playStats;									///< Int whether we want to log play stats or not, if <= 0 then we don't log

#if defined(RTS_DEBUG) || defined(RTS_INTERNAL)
	bool m_wireframe;
	bool m_stateMachineDebug;
	bool m_useCameraConstraints;
	bool m_specialPowerUsesDelay;
	bool m_shroudOn;
	bool m_fogOfWarOn;
	bool m_jabberOn;
	bool m_munkeeOn;
	bool m_allowUnselectableSelection;			///< Are we allowed to select things that are unselectable?
	bool m_disableCameraFade;								///< if true, script commands affecting camera are disabled
	bool m_disableScriptedInputDisabling;		///< if true, script commands can't disable input
	bool m_disableMilitaryCaption;					///< if true, military briefings go fast
	Int m_benchmarkTimer;										///< how long to play the game in benchmark mode?
	bool m_checkForLeaks;
	bool m_vTune;
	bool m_debugCamera;						///< Used to display Camera debug information
	bool m_debugVisibility;						///< Should we actively debug the visibility
	Int m_debugVisibilityTileCount;		///< How many tiles we should show when debugging visibility
	Real m_debugVisibilityTileWidth;	///< How wide should these tiles be?
	Int m_debugVisibilityTileDuration;	///< How long should these tiles stay around, in frames?
	bool m_debugThreatMap;						///< Should we actively debug the threat map
	UnsignedInt m_maxDebugThreat;			///< This value (and any values greater) will appear full RED.
	Int m_debugThreatMapTileDuration;	///< How long should these tiles stay around, in frames?
	bool m_debugCashValueMap;					///< Should we actively debug the threat map
	UnsignedInt m_maxDebugValue;			///< This value (and any values greater) will appear full GREEN.
	Int m_debugCashValueMapTileDuration;	///< How long should these tiles stay around, in frames?
	RGBColor m_debugVisibilityTargettableColor;	///< What color should the targettable cells be?
	RGBColor m_debugVisibilityDeshroudColor;			///< What color should the deshrouding cells be?
	RGBColor m_debugVisibilityGapColor;					///< What color should the gap generator cells be?
	bool m_debugProjectilePath;						///< Should we actively debug the bezier paths on projectiles
	Real m_debugProjectileTileWidth;			///< How wide should these tiles be?
	Int m_debugProjectileTileDuration;		///< How long should these tiles stay around, in frames?
	RGBColor m_debugProjectileTileColor;	///< What color should these tiles be?
	bool m_showCollisionExtents;	///< Used to display collision extents
  bool m_showAudioLocations;    ///< Used to display audio markers and ambient sound radii
	bool m_saveStats;
	bool m_saveAllStats;
	bool m_useLocalMOTD;
	AsciiString m_baseStatsDir;
	AsciiString m_MOTDPath;
	Int m_latencyAverage;					///< Average latency to insert
	Int m_latencyAmplitude;				///< Amplitude of sinusoidal modulation of latency
	Int m_latencyPeriod;					///< Period of sinusoidal modulation of latency
	Int m_latencyNoise;						///< Max amplitude of jitter to throw in
	Int m_packetLoss;							///< Percent of packets to drop
	bool m_extraLogging;					///< More expensive debug logging to catch crashes.
#endif

#ifdef DEBUG_CRASHING
	bool m_debugIgnoreAsserts;						///< Ignore all asserts.
#endif

#ifdef DEBUG_STACKTRACE
	bool m_debugIgnoreStackTrace;					///< No stacktraces for asserts.
#endif

	bool				m_isBreakableMovie;							///< if we enter a breakable movie, set this flag
	bool				m_breakTheMovie;								///< The user has hit escape!
	
	AsciiString m_modDir;
	AsciiString m_modBIG;

	// the trailing '\' is included!
	AsciiString getPath_UserData() const;

private:

	static const FieldParse s_GlobalDataFieldParseTable[];

	// this is private, since we read the info from Windows and cache it for
	// future use. No one is allowed to change it, ever. (srj)
	AsciiString m_userDataDir;
	
	// just the "leaf name", read from INI. private because no one is ever allowed
	// to look at it directly; they must go thru getPath_UserData(). (srj)
	AsciiString m_userDataLeafName;

	static GlobalData *m_theOriginal;		///< the original global data instance (no overrides)
	GlobalData *m_next;									///< next instance (for overrides)
	GlobalData *newOverride( void );		/** create a new override, copy data from previous
																			override, and return it */

#if defined(_MSC_VER) && _MSC_VER < 1300
	GlobalData(const GlobalData& that) { DEBUG_CRASH(("unimplemented")); }
	GlobalData& operator=(const GlobalData& that) { DEBUG_CRASH(("unimplemented")); return *this; }
#else
	GlobalData(const GlobalData& that) = delete;
	GlobalData& operator=(const GlobalData& that) = default;
#endif

};

// singleton
extern GlobalData* TheWritableGlobalData;

#define TheGlobalData ((const GlobalData*)TheWritableGlobalData)

#endif
