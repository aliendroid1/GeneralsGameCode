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

// FILE: WinMain.cpp //////////////////////////////////////////////////////////
// 
// Entry point for game application
//
// Author: Colin Day, April 2001
//
///////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  // only bare bones windows stuff wanted
#include <windows.h>
//#include <stdlib.h>
#include <crtdbg.h>
#include <eh.h>
//#include <ole2.h>
//#include <dbt.h>

// USER INCLUDES //////////////////////////////////////////////////////////////
//#include "WinMain.h"
//#include "Lib/BaseType.h"
#include "Common/CriticalSection.h"
//#include "Common/GlobalData.h"
#include "Common/GameEngine.h"
//#include "Common/GameSounds.h"
#include "Common/Debug.h"
#include "Common/GameMemory.h"
//#include "Common/SafeDisc/CdaPfn.h"
#include "Common/StackDump.h"
#include "Common/MessageStream.h"
//#include "Common/Team.h"
#include "GameClient/ClientInstance.h"
//#include "GameClient/InGameUI.h"
//#include "GameClient/GameClient.h"
//#include "GameLogic/GameLogic.h"  ///< @todo for demo, remove
#include "GameClient/Mouse.h"
#include "GameClient/IMEManager.h"
#include "Win32Device/GameClient/Win32Mouse.h"
#include "Win32Device/Common/Win32GameEngine.h"
#include "Common/version.h"
#include "BuildVersion.h"
#include "GeneratedVersion.h"
#include "resource.h"
#include "trim.h"

#ifdef RTS_INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma message("************************************** WARNING, optimization disabled for debugging purposes")
#endif

// GLOBALS ////////////////////////////////////////////////////////////////////
HINSTANCE ApplicationHInstance = NULL;  ///< our application instance
HWND ApplicationHWnd = NULL;  ///< our application window handle
Bool ApplicationIsWindowed = false;
Win32Mouse *TheWin32Mouse= NULL;  ///< for the WndProc() only
DWORD TheMessageTime = 0;	///< For getting the time that a message was posted from Windows.

const Char *g_strFile = "data\\Generals.str";
const Char *g_csfFile = "data\\%s\\Generals.csf";
const char *gAppPrefix = ""; /// So WB can have a different debug log file name.

#define DEFAULT_XRESOLUTION 800
#define DEFAULT_YRESOLUTION 600



static Bool gInitializing = false;
static Bool gDoPaint = true;
static Bool isWinMainActive = false; 

static HBITMAP gLoadScreenBitmap = NULL;


/*Giant hack in order to keep the game from getting stuck when alt-tabbing*/
void Reset_D3D_Device(bool active)
{
	if (TheDisplay && WW3D::Is_Initted() && !TheDisplay->getWindowed())
	{
		if (active)
		{
			//switch back to desired mode when user alt-tabs back into game
			WW3D::Set_Render_Device(WW3D::Get_Render_Device(), TheDisplay->getWidth(), TheDisplay->getHeight(), TheDisplay->getBitDepth(), TheDisplay->getWindowed(), true, true);
		}
		else
		{
			//switch to windowed mode whenever the user alt-tabs out of game. Don't restore assets after reset since we'll do it when returning.
			WW3D::Set_Render_Device(WW3D::Get_Render_Device(), TheDisplay->getWidth(), TheDisplay->getHeight(), TheDisplay->getBitDepth(), 1, true, true, false);
		}
	}
}






// WndProc ====================================================================
/** Window Procedure */
//=============================================================================
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	// First let the IME manager do it's stuff. 
	if ( TheIMEManager && TheIMEManager->serviceIMEMessage( hWnd, message, wParam, lParam ) )
			return TheIMEManager->result(); // The manager intercepted an IME message so return the result
	
	switch( message ) // handle all window messages
	{
		case WM_NCHITTEST:	// Prevent the user from selecting the menu in fullscreen mode
			if( !ApplicationIsWindowed )
				return HTCLIENT;
		break;
		case WM_POWERBROADCAST:
			switch( wParam )
			{
				case PBT_APMQUERYSUSPEND:
					// At this point, the app should save any data for open
					// network connections, files, etc., and prepare to go into
					// a suspended mode.
					return TRUE;
				case PBT_APMRESUMESUSPEND:
					// At this point, the app should recover any data, network
					// connections, files, etc., and resume running from when
					// the app was suspended.
					return TRUE;
			}
        break;
		case WM_SYSCOMMAND:
			switch (wParam)	// Prevent moving/sizing and power loss in fullscreen mode
			{
				case SC_MOVE: case SC_SIZE: case SC_MAXIMIZE: case SC_KEYMENU: case SC_MONITORPOWER:
					if (!ApplicationIsWindowed)
						return TRUE;
			}
        break;
		case WM_CLOSE:
			TheGameEngine->checkAbnormalQuitting();
			TheGameEngine->reset();
			TheGameEngine->setQuitting(TRUE);
			PostQuitMessage(0);
			return 0;
		break;
		case WM_SETFOCUS:
			// reset the state of our keyboard cause we haven't been paying attention to the keys while focus was away
			if( TheKeyboard )
				TheKeyboard->resetKeys();
			if (TheWin32Mouse)
				TheWin32Mouse->lostFocus(FALSE);
		break;
		case WM_KILLFOCUS:
			if (TheKeyboard)
				TheKeyboard->resetKeys();
			if (TheWin32Mouse)
				TheWin32Mouse->lostFocus(TRUE);
		break;
		case WM_SIZE:
			if (!gInitializing)		// When W3D initializes, it resizes the window.  So stop repainting.
				gDoPaint = false;
		break;
		case WM_ACTIVATEAPP:
			if ( (Bool)wParam != isWinMainActive)
			{	
				isWinMainActive = wParam;	
				if (TheGameEngine)
					TheGameEngine->setIsActive(isWinMainActive);
				Reset_D3D_Device(isWinMainActive);
				if (isWinMainActive)
				{	//restore mouse cursor to our custom version.
					if (TheWin32Mouse)
						TheWin32Mouse->setCursor(TheWin32Mouse->getMouseCursor());
				}
			}
			return 0;
		break;
		case WM_ACTIVATE:
		{
			Int active = LOWORD(wParam);
			// when window is becoming deactivated we must release mouse cursor locks on our region, 
			// otherwise set the mouse limit region again which will clip the cursor to our window
			if (active == WA_INACTIVE)
			{
				ClipCursor(NULL);
				if (TheAudio)
					TheAudio->loseFocus();
			}
			else
			{
				if (TheMouse)
					TheMouse->setMouseLimits();
				if (TheAudio)
					TheAudio->regainFocus();
			}
		}
		break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				PostQuitMessage( 0 );
				break;
			}  
			return 0;
		break;
		case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDBLCLK:
		{
			if( TheWin32Mouse )
				TheWin32Mouse->addWin32Event( message, wParam, lParam, TheMessageTime );
			return 0;
		}
		break;
		case WM_MOUSEWHEEL:
		{
			long x = (long) LOWORD(lParam);
			long y = (long) HIWORD(lParam);
			RECT rect;

			// ignore when outside of client area
			GetWindowRect(ApplicationHWnd, &rect);
			if (x < rect.left || x > rect.right || y < rect.top || y > rect.bottom)
				return 0;

			if (TheWin32Mouse)
				TheWin32Mouse->addWin32Event(message, wParam, lParam, TheMessageTime);
			return 0;
		}
		break;
		case WM_MOUSEMOVE:
		{
			Int x = (Int) LOWORD(lParam);
			Int y = (Int) HIWORD(lParam);
			RECT rect;

			// ignore when outside of client area
			GetClientRect(ApplicationHWnd, &rect);
			if (x < rect.left || x > rect.right || y < rect.top || y > rect.bottom)
				return 0;

			if (TheWin32Mouse)
				TheWin32Mouse->addWin32Event(message, wParam, lParam, TheMessageTime);

			return 0;
		}
		break;
		case WM_SETCURSOR:
			if (TheWin32Mouse && (HWND)wParam == ApplicationHWnd)
				TheWin32Mouse->setCursor(TheWin32Mouse->getMouseCursor());
			return TRUE;	//tell Windows not to reset mouse cursor image to default.
		break;
		case WM_PAINT:
			if (gDoPaint)
			{
				PAINTSTRUCT paint;
				HDC dc = ::BeginPaint(hWnd, &paint);
				if (gLoadScreenBitmap!=NULL) 
				{
					Int savContext = ::SaveDC(dc);
					HDC tmpDC = ::CreateCompatibleDC(dc);
					HBITMAP savBitmap = (HBITMAP)::SelectObject(tmpDC, gLoadScreenBitmap);
					::BitBlt(dc, 0, 0, DEFAULT_XRESOLUTION, DEFAULT_YRESOLUTION, tmpDC, 0, 0, SRCCOPY);
					::SelectObject(tmpDC, savBitmap);
					::DeleteDC(tmpDC);
					::RestoreDC(dc, savContext);
				}
				::EndPaint(hWnd, &paint);
				return TRUE;
			}
		break;

		case WM_ERASEBKGND:
			if (!gDoPaint) 
				return TRUE;	//we don't need to erase the background because we always draw entire window.
		break;
	}  // end switch
	return DefWindowProc( hWnd, message, wParam, lParam );
} 






// initializeAppWindows =======================================================
/** Register windows class and create application windows. */
//=============================================================================
static Bool initializeAppWindows( HINSTANCE hInstance, Int nCmdShow, Bool runWindowed )
{
	DWORD windowStyle;
	Int startWidth = DEFAULT_XRESOLUTION,
			startHeight = DEFAULT_YRESOLUTION;

	// register the window class

  WNDCLASS wndClass = { CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WndProc, 0, 0, hInstance,
                       LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ApplicationIcon)),
                       NULL/*LoadCursor(NULL, IDC_ARROW)*/, 
                       (HBRUSH)GetStockObject(BLACK_BRUSH), NULL,
	                     TEXT("Game Window") };
  RegisterClass( &wndClass );

   // Create our main window
	windowStyle =  WS_POPUP|WS_VISIBLE;
	if (runWindowed) 
		windowStyle |= WS_DLGFRAME | WS_CAPTION | WS_SYSMENU;
	else
		windowStyle |= WS_EX_TOPMOST | WS_SYSMENU;

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = startWidth;
	rect.bottom = startHeight;
	AdjustWindowRect (&rect, windowStyle, FALSE);
	if (runWindowed) {
		// Makes the normal debug 800x600 window center in the screen.
		startWidth = DEFAULT_XRESOLUTION;
		startHeight= DEFAULT_YRESOLUTION;
	}

	gInitializing = true;

  HWND hWnd = CreateWindow( TEXT("Game Window"),
                            TEXT("Command and Conquer Generals"),
                            windowStyle, 
														(GetSystemMetrics( SM_CXSCREEN ) / 2) - (startWidth / 2), // original position X
														(GetSystemMetrics( SM_CYSCREEN ) / 2) - (startHeight / 2),// original position Y
														// Lorenzen nudged the window higher
														// so the constantdebug report would 
														// not get obliterated by assert windows, thank you.
														//(GetSystemMetrics( SM_CXSCREEN ) / 2) - (startWidth / 2),   //this works with any screen res
														//(GetSystemMetrics( SM_CYSCREEN ) / 25) - (startHeight / 25),//this works with any screen res
														rect.right-rect.left,
														rect.bottom-rect.top,
														0L, 
														0L, 
														hInstance, 
														0L );


	if (!runWindowed)
	{	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOSIZE |SWP_NOMOVE);
	}
	else 
		SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0,SWP_NOSIZE |SWP_NOMOVE);

	SetFocus(hWnd);

	SetForegroundWindow(hWnd);
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	// save our application instance and window handle for future use
	ApplicationHInstance = hInstance;
	ApplicationHWnd = hWnd;
	gInitializing = false;
	if (!runWindowed) {
		gDoPaint = false;
	}

	return true;  // success

}  // end initializeAppWindows

void munkeeFunc(void);
//CDAPFN_DECLARE_GLOBAL(munkeeFunc, CDAPFN_OVERHEAD_L5, CDAPFN_CONSTRAINT_NONE);
void munkeeFunc(void)
{
//	CDAPFN_ENDMARK(munkeeFunc);
}

void checkProtection(void)
{
#ifdef RTS_INTERNAL
	__try
	{
		munkeeFunc();
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		exit(0); // someone is messing with us.
	}
#endif
}

char *nextParam(char *newSource, const char *seps)
{
	static char *source = NULL;
	if (newSource)
	{
		source = newSource;
	}
	if (!source)
	{
		return NULL;
	}

	// find first separator
	char *first = source;//strpbrk(source, seps);
	if (first)
	{
		// go past separator
		char *firstSep = strpbrk(first, seps);
		char firstChar[2] = {0,0};
		if (firstSep == first)
		{
			firstChar[0] = *first;
			while (*first == firstChar[0]) first++;
		}

		// find end
		char *end;
		if (firstChar[0])
			end = strpbrk(first, firstChar);
		else
			end = strpbrk(first, seps);

		// trim string & save next start pos
		if (end)
		{
			source = end+1;
			*end = 0;

			if (!*source)
				source = NULL;
		}
		else
		{
			source = NULL;
		}

		if (first && !*first)
			first = NULL;
	}

	return first;
}

// Necessary to allow memory managers and such to have useful critical sections
static CriticalSection critSec1, critSec2, critSec3, critSec4, critSec5;

// WinMain ====================================================================
/** Application entry point */
//=============================================================================
Int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine, Int nCmdShow )
{
	checkProtection();

	try {

		_set_se_translator( DumpExceptionInfo ); // Hook that allows stack trace.
		//
		// there is something about checkin in and out the .dsp and .dsw files 
		// that blows the working directory information away on each of the 
		// developers machines so we're going to hack it for a while and set our
		// working directory to the directory with the .exe since that's not the
		// default in a DevStudio project
		//

		TheAsciiStringCriticalSection = &critSec1;
		TheUnicodeStringCriticalSection = &critSec2;
		TheDmaCriticalSection = &critSec3;
		TheMemoryPoolCriticalSection = &critSec4;
		TheDebugLogCriticalSection = &critSec5;

		/// @todo remove this force set of working directory later
		Char buffer[ _MAX_PATH ];
		GetModuleFileName( NULL, buffer, sizeof( buffer ) );
		Char *pEnd = buffer + strlen( buffer );
		while( pEnd != buffer ) 
		{
			if( *pEnd == '\\' ) 
			{
				*pEnd = 0;
				break;
			}
			pEnd--;
		}
		::SetCurrentDirectory(buffer);


		/*
		** Convert WinMain arguments to simple main argc and argv
		*/
		int argc = 1;
		char * argv[20];
		argv[0] = NULL;

		char *token;
		token = nextParam(lpCmdLine, "\" ");
		while (argc < 20 && token != NULL) {
			argv[argc++] = strtrim(token);
			//added a preparse step for this flag because it affects window creation style
			if (stricmp(token,"-win")==0)
				ApplicationIsWindowed=true;
			token = nextParam(NULL, "\" ");	   
		}

		if (argc>2 && strcmp(argv[1],"-DX")==0) {  
			Int i;
			DEBUG_LOG(("\n--- DX STACK DUMP\n"));
			for (i=2; i<argc; i++) {
				Int pc;
				pc = 0;
				sscanf(argv[i], "%x",  &pc);
				char name[_MAX_PATH], file[_MAX_PATH];
				unsigned int line;
				unsigned int addr;
				GetFunctionDetails((void*)pc, name, file, &line, &addr);
				DEBUG_LOG(("0x%x - %s, %s, line %d address 0x%x\n", pc, name, file, line, addr));
			}
			DEBUG_LOG(("\n--- END OF DX STACK DUMP\n"));
			return 0;
		}

		#ifdef RTS_DEBUG
			// Turn on Memory heap tracking
			int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
			tmpFlag |= (_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
			tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;
			_CrtSetDbgFlag( tmpFlag );
		#endif



		// install debug callbacks
	//	WWDebug_Install_Message_Handler(WWDebug_Message_Callback);
	//	WWDebug_Install_Assert_Handler(WWAssert_Callback);

 		// [SKB: Jun 24 2003 @ 1:50pm] :
		// Force to be loaded from a file, not a resource so same exe can be used in germany and retail.
 		gLoadScreenBitmap = (HBITMAP)LoadImage(hInstance, "Install_Final.bmp",	IMAGE_BITMAP, 0, 0, LR_SHARED|LR_LOADFROMFILE);

		// register windows class and create application window
		if( initializeAppWindows( hInstance, nCmdShow, ApplicationIsWindowed) == false )
			return 0;

		if (gLoadScreenBitmap!=NULL) {
			::DeleteObject(gLoadScreenBitmap);
			gLoadScreenBitmap = NULL;
		}


		// BGC - initialize COM
	//	OleInitialize(NULL);

		// start the log
		DEBUG_INIT(DEBUG_FLAGS_DEFAULT);
		initMemoryManager();

 
		// Set up version info
		TheVersion = NEW Version;
		TheVersion->setVersion(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILDNUM, VERSION_LOCALBUILDNUM,
			AsciiString(VERSION_BUILDUSER), AsciiString(VERSION_BUILDLOC),
			AsciiString(__TIME__), AsciiString(__DATE__));



		// TheSuperHackers @refactor The instance mutex now lives in its own class.

		if (!rts::ClientInstance::initialize())
		{
			HWND ccwindow = FindWindow(rts::ClientInstance::getFirstInstanceName(), NULL);
			if (ccwindow)
			{
				SetForegroundWindow(ccwindow);
				ShowWindow(ccwindow, SW_RESTORE);
			}

			DEBUG_LOG(("Generals is already running...Bail!\n"));
			delete TheVersion;
			TheVersion = NULL;
			shutdownMemoryManager();
			DEBUG_SHUTDOWN();
			return 0;
		}
		DEBUG_LOG(("Create Generals Mutex okay.\n"));

		DEBUG_LOG(("CRC message is %d\n", GameMessage::MSG_LOGIC_CRC));

		// run the game main loop
		GameMain(argc, argv);



		delete TheVersion;
		TheVersion = NULL;

	#ifdef MEMORYPOOL_DEBUG
		TheMemoryPoolFactory->debugMemoryReport(REPORT_POOLINFO | REPORT_POOL_OVERFLOW | REPORT_SIMPLE_LEAKS, 0, 0);
	#endif
	#if defined(RTS_DEBUG) || defined(RTS_INTERNAL)
		TheMemoryPoolFactory->memoryPoolUsageReport("AAAMemStats");
	#endif

		// close the log
		shutdownMemoryManager();
		DEBUG_SHUTDOWN();

		// BGC - shut down COM
	//	OleUninitialize();
	}	
	catch (...) 
	{ 
	
	}

	TheAsciiStringCriticalSection = NULL;
	TheUnicodeStringCriticalSection = NULL;
	TheDmaCriticalSection = NULL;
	TheMemoryPoolCriticalSection = NULL;

	return 0;

}  // end WinMain

// CreateGameEngine ===========================================================
/** Create the Win32 game engine we're going to use */
//=============================================================================
GameEngine *CreateGameEngine( void )
{
	Win32GameEngine *engine;

	engine = NEW Win32GameEngine;
	//game engine may not have existed when app got focus so make sure it
	//knows about current focus state.
	engine->setIsActive(isWinMainActive);

	return engine;

}  // end CreateGameEngine
