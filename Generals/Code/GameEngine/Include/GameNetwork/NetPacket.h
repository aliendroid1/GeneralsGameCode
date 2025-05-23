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

/*
Ok, how this should have been done is to make each of the command types
have a bitmask telling which command message header information
each command type required.  That would make finding out the size of
a particular command easier to find, without so much repetitious code.
We would still need to have a separate function for each command type
for the data, but at least that wouldn't be repeating code, that would
be specialized code.
*/



#pragma once

#ifndef __NETPACKET_H
#define __NETPACKET_H

#include "NetworkDefs.h"
#include "GameNetwork/NetCommandList.h"
#include "Common/MessageStream.h"
#include "Common/GameMemory.h"

class NetPacket;

typedef std::list<NetPacket *> NetPacketList;
typedef std::list<NetPacket *>::iterator NetPacketListIter;

class NetPacket : public MemoryPoolObject
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(NetPacket, "NetPacket")		
public:
	NetPacket();
	NetPacket(TransportMessage *msg);
	//virtual ~NetPacket();

	void init();
	void reset();
	void setAddress(Int addr, Int port);
	bool addCommand(NetCommandRef *msg);
	Int getNumCommands();

	NetCommandList *getCommandList();

	static NetCommandRef * ConstructNetCommandMsgFromRawData(UnsignedByte *data, UnsignedShort dataLength);
	static NetPacketList ConstructBigCommandPacketList(NetCommandRef *ref);

	UnsignedByte *getData();
	Int getLength();
	UnsignedInt getAddr();
	UnsignedShort getPort();

protected:
	static UnsignedInt GetBufferSizeNeededForCommand(NetCommandMsg *msg);
	static void FillBufferWithCommand(UnsignedByte *buffer, NetCommandRef *msg);

	// These functions return the size of the command without any compression, repetition, etc.
	// i.e. All of the required fields are taken into account when returning the size.
	static UnsignedInt GetGameCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetAckCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetFrameCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetPlayerLeaveCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetRunAheadMetricsCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetRunAheadCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDestroyPlayerCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetKeepAliveCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDisconnectKeepAliveCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDisconnectPlayerCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetPacketRouterQueryCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetPacketRouterAckCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDisconnectChatCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDisconnectVoteCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetChatCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetProgressMessageSize(NetCommandMsg *msg);
	static UnsignedInt GetLoadCompleteMessageSize(NetCommandMsg *msg);
	static UnsignedInt GetTimeOutGameStartMessageSize(NetCommandMsg *msg);
	static UnsignedInt GetWrapperCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetFileCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetFileAnnounceCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetFileProgressCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDisconnectFrameCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetDisconnectScreenOffCommandSize(NetCommandMsg *msg);
	static UnsignedInt GetFrameResendRequestCommandSize(NetCommandMsg *msg);

	static void FillBufferWithGameCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithAckCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithFrameCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithPlayerLeaveCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithRunAheadMetricsCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithRunAheadCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDestroyPlayerCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithKeepAliveCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDisconnectKeepAliveCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDisconnectPlayerCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithPacketRouterQueryCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithPacketRouterAckCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDisconnectChatCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDisconnectVoteCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithChatCommand(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithProgressMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithLoadCompleteMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithTimeOutGameStartMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithFileMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithFileProgressMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithFileAnnounceMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDisconnectFrameMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithDisconnectScreenOffMessage(UnsignedByte *buffer, NetCommandRef *msg);
	static void FillBufferWithFrameResendRequestMessage(UnsignedByte *buffer, NetCommandRef *msg);

	bool addFrameCommand(NetCommandRef *msg);
	bool isRoomForFrameMessage(NetCommandRef *msg);
	bool addAckCommand(NetCommandRef *msg, UnsignedShort commandID, UnsignedByte originalPlayerID);
	bool addAckStage1Command(NetCommandRef *msg);
	bool addAckStage2Command(NetCommandRef *msg);
	bool addAckBothCommand(NetCommandRef *msg);
	bool isRoomForAckMessage(NetCommandRef *msg);
	bool addGameCommand(NetCommandRef *msg);
	bool isRoomForGameMessage(NetCommandRef *msg, GameMessage *gmsg);
	bool addPlayerLeaveCommand(NetCommandRef *msg);
	bool isRoomForPlayerLeaveMessage(NetCommandRef *msg);
	bool addRunAheadMetricsCommand(NetCommandRef *msg);
	bool isRoomForRunAheadMetricsMessage(NetCommandRef *msg);
	bool addRunAheadCommand(NetCommandRef *msg);
	bool isRoomForRunAheadMessage(NetCommandRef *msg);
	bool addDestroyPlayerCommand(NetCommandRef *msg);
	bool isRoomForDestroyPlayerMessage(NetCommandRef *msg);
	bool addKeepAliveCommand(NetCommandRef *msg);
	bool isRoomForKeepAliveMessage(NetCommandRef *msg);
	bool addDisconnectKeepAliveCommand(NetCommandRef *msg);
	bool isRoomForDisconnectKeepAliveMessage(NetCommandRef *msg);
	bool addDisconnectPlayerCommand(NetCommandRef *msg);
	bool isRoomForDisconnectPlayerMessage(NetCommandRef *msg);
	bool addPacketRouterQueryCommand(NetCommandRef *msg);
	bool isRoomForPacketRouterQueryMessage(NetCommandRef *msg);
	bool addPacketRouterAckCommand(NetCommandRef *msg);
	bool isRoomForPacketRouterAckMessage(NetCommandRef *msg);
	bool addDisconnectChatCommand(NetCommandRef *msg);
	bool isRoomForDisconnectChatMessage(NetCommandRef *msg);
	bool addChatCommand(NetCommandRef *msg);
	bool isRoomForChatMessage(NetCommandRef *msg);
	bool addDisconnectVoteCommand(NetCommandRef *msg);
	bool isRoomForDisconnectVoteMessage(NetCommandRef *msg);
	bool addProgressMessage( NetCommandRef *msg );
	bool isRoomForProgressMessage( NetCommandRef *msg );
	bool addLoadCompleteMessage( NetCommandRef *msg );
	bool isRoomForLoadCompleteMessage( NetCommandRef *msg );
	bool addTimeOutGameStartMessage( NetCommandRef *msg );
	bool isRoomForTimeOutGameStartMessage( NetCommandRef *msg );
	bool addWrapperCommand(NetCommandRef *msg);
	bool isRoomForWrapperMessage(NetCommandRef *msg);
	bool addFileCommand(NetCommandRef *msg);
	bool isRoomForFileMessage(NetCommandRef *msg);
	bool addFileAnnounceCommand(NetCommandRef *msg);
	bool isRoomForFileAnnounceMessage(NetCommandRef *msg);
	bool addFileProgressCommand(NetCommandRef *msg);
	bool isRoomForFileProgressMessage(NetCommandRef *msg);
	bool addDisconnectFrameCommand(NetCommandRef *msg);
	bool isRoomForDisconnectFrameMessage(NetCommandRef *msg);
	bool addDisconnectScreenOffCommand(NetCommandRef *msg);
	bool isRoomForDisconnectScreenOffMessage(NetCommandRef *msg);
	bool addFrameResendRequestCommand(NetCommandRef *msg);
	bool isRoomForFrameResendRequestMessage(NetCommandRef *msg);

	bool isAckRepeat(NetCommandRef *msg);
	bool isAckBothRepeat(NetCommandRef *msg);
	bool isAckStage1Repeat(NetCommandRef *msg);
	bool isAckStage2Repeat(NetCommandRef *msg);
	bool isFrameRepeat(NetCommandRef *msg);

	static NetCommandMsg * readGameMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readAckBothMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readAckStage1Message(UnsignedByte *data, Int &i);
	static NetCommandMsg * readAckStage2Message(UnsignedByte *data, Int &i);
	static NetCommandMsg * readFrameMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readPlayerLeaveMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readRunAheadMetricsMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readRunAheadMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDestroyPlayerMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readKeepAliveMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDisconnectKeepAliveMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDisconnectPlayerMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readPacketRouterQueryMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readPacketRouterAckMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDisconnectChatMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDisconnectVoteMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readChatMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readProgressMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readLoadCompleteMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readTimeOutGameStartMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readWrapperMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readFileMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readFileAnnounceMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readFileProgressMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDisconnectFrameMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readDisconnectScreenOffMessage(UnsignedByte *data, Int &i);
	static NetCommandMsg * readFrameResendRequestMessage(UnsignedByte *data, Int &i);

	void writeGameMessageArgumentToPacket(GameMessageArgumentDataType type, GameMessageArgumentType arg);
	static void readGameMessageArgumentFromPacket(GameMessageArgumentDataType type, NetGameCommandMsg *msg, UnsignedByte *data, Int &i);

	void dumpPacketToLog();

protected:
	UnsignedByte		m_packet[MAX_PACKET_SIZE];
	Int							m_packetLen;
	UnsignedInt			m_addr;
	Int							m_numCommands;
	NetCommandRef*	m_lastCommand;
	UnsignedInt			m_lastFrame;
	UnsignedShort		m_port;
	UnsignedShort		m_lastCommandID;
	UnsignedByte		m_lastPlayerID;
	UnsignedByte		m_lastCommandType;
	UnsignedByte		m_lastRelay;
};

#endif // __NETPACKET_H
