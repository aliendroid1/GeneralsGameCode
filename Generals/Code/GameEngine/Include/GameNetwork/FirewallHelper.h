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


/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /RedAlert2/NAT.CPP                                          $*
 *                                                                                             *
 *                      $Author:: Steve_t                                                     $*
 *                                                                                             *
 *                     $Modtime:: 3/15/01 12:00PM                                             $*
 *                                                                                             *
 *                    $Revision:: 1                                                           $*
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#ifndef __FIREWALLHELPER_H
#define __FIREWALLHELPER_H

class UDP;

#define NUM_TEST_PORTS 4
#define MAX_SPARE_SOCKETS 8

/*
**
**   Class to help in dealing with firewalls.
**
**
**
*/

struct SpareSocketStruct {
	UDP *udp;
	UnsignedShort port;
};

enum FirewallDetectionState : Int {
	DETECTIONSTATE_IDLE,
	DETECTIONSTATE_BEGIN,
	DETECTIONSTATE_TEST1,
	DETECTIONSTATE_TEST2,
	DETECTIONSTATE_TEST3,
	DETECTIONSTATE_TEST3_WAITFORRESPONSES,
	DETECTIONSTATE_TEST4_1,
	DETECTIONSTATE_TEST4_2,
	DETECTIONSTATE_TEST5,
	DETECTIONSTATE_DONE
};

#pragma pack(push, 1)

// size = 16 bytes
struct ManglerData {
		unsigned int		CRC;
		unsigned short	magic;
		unsigned short	PacketID;
		unsigned short	MyMangledPortNumber;
		unsigned short	OriginalPortNumber;
		unsigned char		MyMangledAddress[4];
		unsigned char		NetCommandType;
		unsigned char		BlitzMe;
		unsigned short	Padding;
};

// size = TransportMessageHeader + ManglerData + 10 bytes = 26 bytes
struct ManglerMessage {
        ManglerData							data;
        int											length;
        unsigned int						ip;
        unsigned short					port;
};

#pragma pack(pop)

static const Int MAX_NUM_MANGLERS = 4;
static const UnsignedShort MANGLER_PORT = 4321;

class FirewallHelperClass {

	public:

		/*
		** Enumeration of firewall behaviors we can detect.
		**
		** It is assumed that all port mangling firewalls change the mangled source port in response to
		** an application source port change.
		*/
		typedef enum tFirewallBehaviorType {

			FIREWALL_MIN = 0,

			/*
			** Just used as an initialiser.
			*/
			FIREWALL_TYPE_UNKNOWN = 0,

			/*
			** This is a simple, non-port translating firewall, or there is no firewall at all.
			*/
			FIREWALL_TYPE_SIMPLE = 1,

			/*
			** This is a firewall/NAT with port mangling but it's pretty dumb - it uses the same mangled
			** source port regardless of the destination address.
			*/
			FIREWALL_TYPE_DUMB_MANGLING = 2,

			/*
			** This is a smarter firewall/NAT with port mangling that uses different mangled source ports
			** for different destination IPs.
			*/
			FIREWALL_TYPE_SMART_MANGLING = 4,

			/*
			** This is a firewall that exhibits the bug as seen in the Netgear firewalls. A previously good
			** source port mapping will change in response to unsolicited traffic from a known IP.
			*/
			FIREWALL_TYPE_NETGEAR_BUG = 8,

			/*
			** This firewall has a simple absolute offset port allocation scheme.
			*/
			FIREWALL_TYPE_SIMPLE_PORT_ALLOCATION = 16,

			/*
			** This firewall has a relative offset port allocation scheme. For these firewalls, we have to
			** subtract the actual source port from the mangled source port to discover the allocation scheme.
			** The mangled port number is based in part on the original source port number.
			*/
			FIREWALL_TYPE_RELATIVE_PORT_ALLOCATION = 32,

			/*
			** This firewall mangles source ports differently depending on the destination port.
			*/
			FIREWALL_TYPE_DESTINATION_PORT_DELTA = 64,

			FIREWALL_MAX = 128

		} FirewallBehaviorType;



		FirewallHelperClass(void);
		virtual ~FirewallHelperClass(void);
		bool detectFirewall(void);
		UnsignedShort getRawFirewallBehavior(void) {return((UnsignedShort)m_behavior);}
		Short getSourcePortAllocationDelta(void);
		Int getFirewallHardness(FirewallBehaviorType behavior);
		Int getFirewallRetries(FirewallBehaviorType behavior);
		void setSourcePortPoolStart(Int port) {m_sourcePortPool = port;};
		Int getSourcePortPool(void) {return(m_sourcePortPool);};
		void readFirewallBehavior(void);
		void reset(void);
		bool behaviorDetectionUpdate(void);
		
		FirewallBehaviorType getFirewallBehavior(void);
		void writeFirewallBehavior(void);

		void flagNeedToRefresh(bool flag);

		static void getManglerName(Int manglerIndex, Char *nameBuf);
		bool sendToManglerFromPort(UnsignedInt address, UnsignedShort port, UnsignedShort packetID, bool blitzme = FALSE);
		UnsignedShort getManglerResponse(UnsignedShort packetID, Int time = 0);
		bool openSpareSocket(UnsignedShort port);
		void closeSpareSocket(UnsignedShort port);
		void closeAllSpareSockets();
		UnsignedShort getNextTemporarySourcePort(Int skip);

		bool detectionBeginUpdate(void);
		bool detectionTest1Update(void);
		bool detectionTest2Update(void);
		bool detectionTest3Update(void);
		bool detectionTest3WaitForResponsesUpdate(void);
		bool detectionTest4Stage1Update(void);
		bool detectionTest4Stage2Update(void);
		bool detectionTest5Update(void);


		/*
		** Behavior query functions.
		*/
		inline bool isNAT(void) {
			if (m_behavior == FIREWALL_TYPE_UNKNOWN || (m_behavior & FIREWALL_TYPE_SIMPLE) != 0) {
				return(FALSE);
			}
			return(TRUE);
		};

		inline bool isNAT(FirewallBehaviorType behavior) {
			if (behavior == FIREWALL_TYPE_UNKNOWN || (behavior & FIREWALL_TYPE_SIMPLE) != 0) {
				return(FALSE);
			}
			return(TRUE);
		};

		inline bool isNetgear(FirewallBehaviorType behavior) {
			if ((behavior & FIREWALL_TYPE_NETGEAR_BUG) != 0) {
				return(TRUE);
			}
			return(FALSE);
		};

		inline bool isNetgear(void) {
			if ((m_behavior & FIREWALL_TYPE_NETGEAR_BUG) != 0) {
				return(TRUE);
			}
			return(FALSE);
		};



	private:

		Int getNATPortAllocationScheme(Int numPorts, UnsignedShort *originalPorts, UnsignedShort *mangledPorts, bool &relativeDelta, bool &looksGood);
		void detectFirewallBehavior(/*bool &canRecord*/);
		bool getReferencePort(void);

		SpareSocketStruct * findSpareSocketByPort(UnsignedShort port);
		ManglerMessage * findEmptyMessage();

		void byteAdjust(ManglerData *data);

		/*
		** How does our firewall behave?
		*/
		FirewallBehaviorType m_behavior;

		/*
		** How did the firewall behave the last time we ran the game.
		*/
		FirewallBehaviorType m_lastBehavior;

		/*
		** What is the delta in our firewalls NAT port allocation scheme.
		*/
		Int m_sourcePortAllocationDelta;

		/*
		** What was the delta the last time we ran?
		*/
		Int m_lastSourcePortAllocationDelta;

		/*
		** Source ports used only to discover port allocation patterns.
		** Needs to be static so that previous communications with the manglers
		** don't affect the current one.
		*/
		static Int m_sourcePortPool;

		/*
		** Spare sockets used for detecting mangling and such.
		*/
		SpareSocketStruct m_spareSockets[MAX_SPARE_SOCKETS];

		UnsignedInt m_manglers[MAX_NUM_MANGLERS];
		Int m_numManglers;

		UnsignedShort m_sparePorts[MAX_SPARE_SOCKETS];
		UnsignedShort m_mangledPorts[MAX_SPARE_SOCKETS];
		UnsignedShort m_packetID;

		ManglerMessage m_messages[MAX_SPARE_SOCKETS];

		FirewallDetectionState m_currentState;
		time_t m_timeoutStart;
		time_t m_timeoutLength;

		Int m_numResponses;
		Int m_currentTry;
};



extern FirewallHelperClass *TheFirewallHelper;
FirewallHelperClass * createFirewallHelper();


#endif	// __FIREWALLHELPER_H

