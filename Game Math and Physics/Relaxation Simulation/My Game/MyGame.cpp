/// \file MyGame.cpp  
/// \brief Main file for your game.
///
/// \mainpage Ball and Spring Toy
/// 
/// Having been introduced to the theory of Verlet integration and Gauss-
/// Seidel relaxation in Chapter 2, we now examine code for a Ball and Spring
/// Toy, which lets you play with a body made up of a bunch
/// of balls connected by constraints. The constraints can be either stick-like
/// (with no springiness), or spring-like (with lots of springiness). You can
/// cycle through various types of body and you can apply impulses to them
/// to see how they react. The toy starts with the body in the air, from which
/// position it drops under gravity to rest at the bottom of the screen.
/// 
/// The controls are very basic. The space bar applies an impulse in a
/// random direction, the Back key restarts with the current body type, and
/// the Enter key advances to the next body type. The Escape key, as always,
/// quits. This is a toy, not a game, because there is no concept of winning or losing.
///
/// Changes to the code since the book was published include the following.
/// The Visual Studio Solution and Projects have been upgraded to Visual Studio 2013.
/// The Solution has now been broken up into multiple Projects.
/// TinyXML has been upgraded from the original to TinyXML 2.
/// The graphics and sound code in the Engine project was upgraded to DirectX 11.2 and
/// DirectXTK with the assistance of Brandon Nelson. These changes naturally rippled
/// through this project, with the most notable effects being the replacement of
/// D3DX declarations by the corresponding DirectXTK declarations (the most frequently
/// used of which are probably D3DXVECTOR3 and D3DXVECTOR2, which were replaced by Vector3
/// and Vector2, respectively). Smaller changes include:
/// (1) the random impulse applied to bodies now tends to be generally towards
/// the center of the screen rather than totally random so that you are no longer
/// dashing the body against the floor half the time (see CBody::DeliverImpulse in
/// Body.cpp).
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

#include "GameDefines.h"
#include "WindowClass.h"
#include "SndList.h"

#include "RenderWorld.h"
#include "ObjectWorld.h"
#include "Timer.h"

#include "debug.h"

BodyType g_nCurrentBody = (BodyType)0; ///< Current body type.
SizeType g_nCurrentSize = (SizeType)0; ///< Current size type.

float g_fSizeMult = 1.0f;	///< The multiplier for size
CTimer g_cTimer; ///< The game timer.
CSoundManager* g_pSoundManager; ///< The Sound Manager.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

//externs
extern CWindow g_cWindow; ///< The window class.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int); ///< Default WinMain.
int  MessageLoop(); ///< Windows message loop.
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM); ///< Default window procedure.

/// Start the game.

void BeginGame(){ 
  g_cTimer.StartLevelTimer(); //starting level now     
  g_cObjectWorld.clear(); //clear old objects
  g_cObjectWorld.CreateBody(g_nCurrentBody);
} //BeginGame

/// Initialize and start the game.

void InitGame(){
  g_cRenderWorld.Initialize(g_cWindow); //bails if it fails
  g_cRenderWorld.LoadImages(); //load images from xml file list
  BeginGame(); //it starts...
} //InitGame

/// Shut down game and release resources.

void EndGame(){
  g_cRenderWorld.Release();
  SAFE_DELETE(g_pSoundManager); 
  ShowCursor(TRUE);
} //EndGame

/// Render a frame of animation.

void RenderFrame(){
  g_cRenderWorld.BeginFrame(); 
  g_cRenderWorld.DrawBackground(); //draw the background
  g_cObjectWorld.draw(); //draw the objects
  g_cRenderWorld.EndFrame(); //shut down graphics pipeline
} //RenderFrame

/// Process a frame of animation.
/// Called once a frame to animate game objects and take appropriate
/// action if the player has won or lost.

void ProcessFrame(){
  //stuff that gets done on every frame
  g_cTimer.beginframe(); //notify timer that frame has begun
  g_cObjectWorld.move(); //move all objects
  RenderFrame(); //render a frame of animation
  g_cTimer.endframe(); //notify timer that frame has ended
} //ProcessFrame

/// Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ 
  switch(k){
    case VK_ESCAPE: return TRUE; //quit    

    case VK_BACK: BeginGame(); break; //start again

    case VK_SPACE: g_cObjectWorld.DeliverImpulse(); break; //bam

    case VK_RETURN: //next victim
      g_nCurrentBody = (BodyType)((g_nCurrentBody + 1)%NUM_BODIES);
      BeginGame(); //start
      break;

	case VK_UP:	// Upscales the size
		if (g_nCurrentSize > (SizeType)0)
		{
			g_nCurrentSize = (SizeType)(g_nCurrentSize - 1);
			g_fSizeMult *= 2;
			BeginGame(); //start
		}
		break;

	case VK_DOWN: // Downscales the size
		if (g_nCurrentSize < (SizeType)2)
		{
			g_nCurrentSize = (SizeType)(g_nCurrentSize + 1);
			g_fSizeMult *= 0.5;
			BeginGame(); //start
		}
		break;
  } //switch
  return FALSE;
} //KeyboardHandler

// Windows functions.
// Dont mess with these unless you really know what you're doing.
// I've written default functions in the Engine library to take
// care of the boring details of Windows housekeeping.

/// Window procedure.
/// Handler for messages from the Windows API. Dont mess with these unless you really know what you're doing.
///  \param h window handle
///  \param m message code
///  \param w parameter for message 
///  \param l second parameter for message
///  \return 0 if message is handled

LRESULT CALLBACK WindowProc(HWND h, UINT m, WPARAM w, LPARAM l){
  return DefaultWindowProc(h, m, w, l);
} //WindowProc

/// Winmain.  
/// Main entry point for this application. Dont mess with these unless you really know what you're doing.
///  \param hI handle to the current instance of this application
///  \param hP unused
///  \param lpC unused 
///  \param nCS specifies how the window is to be shown
///  \return TRUE if application terminates correctly

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nCS){    
  ShowCursor(FALSE);                 
  if(!DefaultWinMain(hI, hP, lpC, nCS)) return 1;

  g_cTimer.start(); //start game timer
  g_cObjectWorld.SetWorldSize(g_cWindow.GetSize()); //set object world size to screen size

  InitGame(); //initialize the game

  g_pSoundManager = new CSoundManager(); //create sound manager
  g_pSoundManager->Load(); //load the sounds for this game

  return MessageLoop();
} //WinMain