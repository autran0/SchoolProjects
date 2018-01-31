/// \file main.cpp 
/// \brief Main file for Ian Parberry's game engine.
///
/// This file contains some custom Windows code for creating, maintaining,
/// and closing down the game window. It consists of some initialization code,
/// a default window procedure, and a default wwinmain. The real ones
/// will be in your game project, but they should mostly just call the
/// default ones here. The idea is to hide all of the nasty, messy,
/// horrible Windows details to a place where the normal user won't
/// see it and be scared by it.
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

#include <windows.h>
#include <windowsx.h>
#include <process.h>

#include "defines.h"
#include "abort.h"
#include "tinyxml2.h"
#include "debug.h"
#include "Sound.h"
#include "WindowClass.h"

#ifdef DEBUG_ON
  CDebugManager g_cDebugManager; ///< The debug manager.
#endif //DEBUG_ON

CWindow g_cWindow; ///< The window class.

static tinyxml2::XMLDocument g_xmlDoc; ///< TinyXML document for settings.
XMLElement* g_xmlSettings = nullptr; ///< TinyXML element for settings tag.

//function prototypes from MyGame.cpp

void ProcessFrame(); ///< Process an animation frame.
void EndGame(); ///< Game termination clean-up.
BOOL KeyboardHandler(WPARAM keystroke); ///< Keyboard handler
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); ///< Window procedure.

/// \brief Initialize XML settings.
/// Opens an XML file and prepares to read settings from it. Settings
/// tag is loaded to XML element g_xmlSettings for later processing. Aborts if it
/// cannot load the file or cannot find settings tag in loaded file.

void InitXMLSettings(){
  const char* xmlFileName = "gamesettings.xml"; //settings file name

  if(g_xmlDoc.LoadFile(xmlFileName))
    ABORT("Cannot load settings file %s.", xmlFileName);

  //get settings tag
  g_xmlSettings = g_xmlDoc.FirstChildElement("settings"); //settings tag
  if(!g_xmlSettings) //abort if tag not found
    ABORT("Cannot find <settings> tag in %s.", xmlFileName);
} //InitXMLSettings

/// \brief Default window procedure.
/// This is the handler for messages from the operating system. 
/// \param hwnd window handle
/// \param message message code
/// \param wParam parameter for message 
/// \param lParam second parameter for message
/// \return 0 if message is handled

LRESULT CALLBACK DefaultWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
  switch(message){ //handle message
    case WM_ACTIVATEAPP: 
      g_cWindow.SetActive((BOOL)wParam); 
      break; 

    case WM_KEYDOWN: //keyboard hit
      if(KeyboardHandler(wParam))DestroyWindow(hwnd);
      break;

    case WM_DESTROY: //on exit
      EndGame();
      PostQuitMessage(0); //this is the last thing to do on exit
      break;

    default: //default window procedure
      return DefWindowProc(hwnd, message, wParam, lParam);
  } //switch(message)
  return 0;
} //DefaultWindowProc

/// Load essential game settings from an xml file.

void LoadGameSettings(){
  if(!g_xmlSettings)
    ABORT("XML settings corrupted."); //bail and fail

  //get game name
  XMLElement* e = g_xmlSettings->FirstChildElement("game"); 
  if(e)
    g_cWindow.SetName(e->Attribute("name"));

   //get renderer settings
  XMLElement* rs = 
    g_xmlSettings->FirstChildElement("renderer"); //renderer tag
  if(rs) //read renderer tag attributes
    g_cWindow.SetSize(rs->IntAttribute("width"), rs->IntAttribute("height"));

  //get debug settings
  #ifdef DEBUG_ON
    g_cDebugManager.GetDebugSettings(g_xmlSettings);
  #endif //DEBUG_ON
} //LoadGameSettings
              
/// \brief Default WinMain.  
/// The main entry point for this application should call this function first. 
///  \param h handle to the current instance of this application
///  \param ph unused - deprecated by Microsoft
///  \param s unused - command line string
///  \param show specifies how the window is to be shown
///  \return TRUE if application terminates correctly

BOOL DefaultWinMain(HINSTANCE h, HINSTANCE ph, LPSTR s, int show){

  #ifdef DEBUG_ON
    g_cDebugManager.open(); //open debug streams, settings came from XML file
  #endif //DEBUG_ON

  InitXMLSettings(); //initialize XML settings reader
  LoadGameSettings(); //load game settings from xml file

  HWND hwnd = g_cWindow.CreateDefaultWindow(h, show); //create window
  if(!hwnd)return FALSE; //bail if problem creating window

  return TRUE;
} //DefaultWinMain

/// \brief The Windows message loop.
/// Every WinMain must have a message loop. This function does essential
/// Windows type things, but also calls out ProcessFrame() as often
/// as it can.

int MessageLoop(){
  MSG msg; //current message

  while(TRUE)
    if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)){ //if message waiting
      if(!GetMessage(&msg, nullptr, 0, 0))
        return (int)msg.wParam; //get it
      TranslateMessage(&msg);
      DispatchMessage(&msg); 
    } //if
    else ProcessFrame();
      //if(g_cWindow.GetActive())ProcessFrame(); //process animation frame
      //else WaitMessage(); //wait in background

  return 0;
} //MessageLoop