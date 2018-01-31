/// \file debug.h
/// \brief Debug definitions.
/// Interface for the debug manager class CDebugManager, and definition
/// of the DEBUGPRINTF macro. This macro will be defined if you are compiling
/// in Debug mode, otherwise it will be aliased to nil, naught, nothing,
/// not even a shadow. Of course, if you want to use it in Release mode,
/// you will have to move the "#define DEBUG_ON" outside of the "#ifdef".
///
/// Created by Ian Parberry to accompany his book
/// "Introduction to Game Physics with Box2D", published by CRC Press in 2013.
/// Copyright Ian Parberry, Laboratory for Recreational Computing,
/// Department of Computer Science & Engineering, University of North
/// Texas, Denton, TX, USA. URL: http://larc.unt.edu/ian.
///
/// This file is made available under the GNU All-Permissive License.
/// Copying and distribution of this file, with or without
/// modification, are permitted in any medium without royalty
/// provided the copyright notice and this notice are preserved.
/// This file is offered as-is, without any warranty.

#pragma once

#ifdef _DEBUG
  #define DEBUG_ON ///< Define this for debugging, comment out to turn off.
#endif

#ifdef DEBUG_ON //ensure debug code is not compiled unless needed

#include <windows.h>
#include <stdio.h>

#include "IPMgr.h"

#include "tinyxml2.h"
using namespace tinyxml2;

#define DEBUG_OUTBUF_SIZE 1024 ///< Size of debug output buffer.
#define DEBUG_FNAME_SIZE 256 ///< Size of debug file name.

/// The debug manager class. The debug manager allows you to send a debug string 
/// to a file, to the Visual Studio debugger, and/or to IP to be displayed by a client 
/// console app. It is designed to be accessed using the macro DEBUGPRINTF.

class CDebugManager{ 
  private:
    FILE *m_fileHandle; ///< File handle for debug output file.
    char m_szOutBuffer[DEBUG_OUTBUF_SIZE]; ///< Output buffer.
    char m_szFileName[DEBUG_FNAME_SIZE]; ///< File that debug message comes from.
    int m_nLineNumber; ///< Line that debug message comes from.
    BOOL m_bOutputToFile; ///< TRUE for debug output to file.
    BOOL m_bOutputToDebugger; ///< TRUE for debug output to debugger.
    BOOL m_bOutputToIP; ///< TRUE for debug output to IP.
    BOOL m_bPrependFileInfo; ///< TRUE to output file name and line in debug messages
    BOOL m_bHeader; ///< TRUE to output header in debug messages
    char m_szDebugFileName[DEBUG_FNAME_SIZE]; ///< Output file for debug messages.
    char m_szDebugIPAddress[16]; ///< IP address to send debug messages to.
    int m_nDebugPort; ///< Debug port number for IP.
    IPManager* m_pIPManager; ///< IP manager.

  public:
    CDebugManager(); ///< Constructor.
    ~CDebugManager(); ///< Destructor.
    void open(); ///< Open output methods.
    void printf(const char* format,...); ///< Debug printf.
    void setsource(char* file, int line); ///< Set file and line number.
    void GetDebugSettings(XMLElement* xmlSettings); ///< Get names from XML element.
}; //CDebugManager
 
extern CDebugManager g_cDebugManager;

/// Real debug printf function.

/// Normally we will call this function using the DEBUGPRINTF macro.

void realDebugPrintf(const char *fmt, ...);

#endif

/// \brief Debug printf.
///< This nasty little macro allows DEBUGPRINTF to take a variable number of arguments.
  
#ifdef DEBUG_ON 
  #define DEBUGPRINTF (g_cDebugManager.setsource(__FILE__,__LINE__),realDebugPrintf)
#else
  #define DEBUGPRINTF //nothing
#endif //DEBUG_ON

