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

// FILE: SubsystemInterface.cpp 
// ----------------------------------------------------------------------------
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "Common/SubsystemInterface.h"
#include "Common/Xfer.h"

#ifdef RTS_INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif



//-----------------------------------------------------------------------------
SubsystemInterface::SubsystemInterface()
{
	if (TheSubsystemList) {
		TheSubsystemList->addSubsystem(this);
	}
}


SubsystemInterface::~SubsystemInterface()
{
	if (TheSubsystemList) {
		TheSubsystemList->removeSubsystem(this);
	}
}


//-----------------------------------------------------------------------------
SubsystemInterfaceList::SubsystemInterfaceList()
{
}

//-----------------------------------------------------------------------------
SubsystemInterfaceList::~SubsystemInterfaceList()
{
	DEBUG_ASSERTCRASH(m_subsystems.empty(), ("not empty"));
	shutdownAll();
}

//-----------------------------------------------------------------------------
void SubsystemInterfaceList::addSubsystem(SubsystemInterface* sys)
{
#ifdef DUMP_PERF_STATS
	m_allSubsystems.push_back(sys);
#endif
}
//-----------------------------------------------------------------------------
void SubsystemInterfaceList::removeSubsystem(SubsystemInterface* sys)
{

}
//-----------------------------------------------------------------------------
void SubsystemInterfaceList::initSubsystem(SubsystemInterface* sys, const char* path1, const char* path2, const char* dirpath, Xfer *pXfer, AsciiString name)
{
	sys->setName(name);
	sys->init();

	INI ini;
	if (path1)
		ini.load(path1, INI_LOAD_OVERWRITE, pXfer );
	if (path2)
		ini.load(path2, INI_LOAD_OVERWRITE, pXfer );
	if (dirpath)
		ini.loadDirectory(dirpath, TRUE, INI_LOAD_OVERWRITE, pXfer );

	m_subsystems.push_back(sys);
}

//-----------------------------------------------------------------------------
void SubsystemInterfaceList::postProcessLoadAll()
{
	for (SubsystemList::iterator it = m_subsystems.begin(); it != m_subsystems.end(); ++it)
	{
		(*it)->postProcessLoad();
	}
}

//-----------------------------------------------------------------------------
void SubsystemInterfaceList::resetAll()
{
//	for (SubsystemList::iterator it = m_subsystems.begin(); it != m_subsystems.end(); ++it)
	for (SubsystemList::reverse_iterator it = m_subsystems.rbegin(); it != m_subsystems.rend(); ++it)
	{
		(*it)->reset();
	}
}

//-----------------------------------------------------------------------------
void SubsystemInterfaceList::shutdownAll()
{
	// must go in reverse order!
	for (SubsystemList::reverse_iterator it = m_subsystems.rbegin(); it != m_subsystems.rend(); ++it)
	{
		SubsystemInterface* sys = *it;
		delete sys;
	}
	m_subsystems.clear();
}

