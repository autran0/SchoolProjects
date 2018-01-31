/// \file MyGame.cpp 
/// \brief Main file for your game.

/// \mainpage Box2D Getting Started.
///
/// This is a simple toy that helps you get started quickly with Box2D. The next
/// 3 chapters will drill down into Box2D more deeply, so don't be concerned if you
/// don't fully understand all of the code here. 
///
/// Each time you hit the space bar a new ball or crate will fall out of the sky.
/// It's not too interesting the first couple of times you do that. It gets more
/// interesting when you hold the space bar down for autorepeat. Then it will rain
/// stuff that will collect at the bottom of the screen after bouncing off of each
/// other in a pleasing fashion. It won't rain forever though, no matter how often
/// or how long you mash the space bar. The screen should just about fill with objects,
/// which will allow you to observe just how well Box2D is able to perform relaxation.
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
/// and Vector2, respectively).
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

#include "Box2D\Box2D.h"
#include "RenderWorld.h"
#include "ObjectWorld.h"

#include "debug.h"

extern CWindow g_cWindow; ///< The window class.

//Physics World. 
b2World g_b2dPhysicsWorld(b2Vec2(0, RW2PW(-100))); ///< Box2D Physics World.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int); ///< Default WinMain.
int  MessageLoop(); ///< Windows message loop.
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM); ///< Default window procedure.

//prototypes for functions in nonplayerobjects.cpp
void CreateWorldEdges();
void CreateCrate(float x, float y);
void CreateTire(float x, float y);
void CreateWindmill(float x, float y);

/// Create all game objects.

void CreateObjects(){
  CreateWorldEdges(); //edges of screen
  CreateWindmill(50, 27);
} //CreateObjects

/// \brief Start the game.

void BeginGame(){     
  g_cObjectWorld.clear(); //clear old objects
  CreateObjects(); //create new objects
} //BeginGame

/// \brief Initialize and start the game.

void InitGame(){
  //set up Render World
  g_cRenderWorld.Initialize(g_cWindow); //bails if it fails
  g_cRenderWorld.LoadImages(); //load images from xml file list

  //set up Object World
  g_cObjectWorld.SetWorldSize(g_cWindow.GetSize()); //set object world size to screen size

  //now start the game
  BeginGame();
} //InitGame

/// \brief Shut down game and release resources.

void EndGame(){
  g_cRenderWorld.Release();
} //EndGame

/// \brief Render a frame of animation.

void RenderFrame(){
  g_cRenderWorld.BeginFrame(); //start up graphics pipeline
  g_cRenderWorld.DrawBackground(); //draw the background
  g_cObjectWorld.draw(); //draw the objects
  g_cRenderWorld.EndFrame(); //shut down graphics pipeline
} //RenderFrame

/// \brief Process a frame of animation.
/// Called once a frame to animate game objects and take appropriate
/// action if the player has won or lost.

void ProcessFrame(){
  g_cObjectWorld.move(); //move all objects
  RenderFrame(); //render a frame of animation
} //ProcessFrame

/// \brief Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ //keyboard handler
  static int nCrateCount = 0; //number of crates generated

  //pick a chaotic x, and a y above the top of the screen
  static unsigned int seed = 51; //random number seed
  float x = (float)(seed%100); //first one from middle of screen
  seed = 4373*seed/761; //linear congruential pseudorandom number generator
  float y = 77.0f; //above the top of the screen

  switch(k){ //keystroke
    case VK_ESCAPE:
      return TRUE;

    case VK_SPACE: 
      //create a tire at (x,y) in Physics World most of the time, but
      //make 32 of them crates more or less random
      if((int)x%(nCrateCount + 11) == 0 && nCrateCount < 32){
        CreateCrate(x, y); 
        nCrateCount++;
      } //if
      else CreateTire(x, y);
      break;

    case VK_BACK: //clear
      g_cObjectWorld.clear(); 
      nCrateCount = 0;
      break;
  } //switch

  return FALSE;
} //KeyboardHandler

// Windows functions.
// Dont mess with these unless you really know what you're doing.
// I've written default functions in the Engine library to take
// care of the boring details of Windows housekeeping.

/// \brief Window procedure.
/// Handler for messages from the Windows API. Dont mess with these unless you really know what you're doing.
///  \param h window handle
///  \param m message code
///  \param w parameter for message 
///  \param l second parameter for message
///  \return 0 if message is handled

LRESULT CALLBACK WindowProc(HWND h, UINT m, WPARAM w, LPARAM l){
  return DefaultWindowProc(h, m, w, l);
} //WindowProc

/// \brief Winmain.  
/// Main entry point for this application. Dont mess with these unless you really know what you're doing.
///  \param hI handle to the current instance of this application
///  \param hP unused
///  \param lpC unused 
///  \param nCS specifies how the window is to be shown
///  \return TRUE if application terminates correctly

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nCS){    
  ShowCursor(FALSE);                 
  if(!DefaultWinMain(hI, hP, lpC, nCS)) return 1;

  InitGame(); //initialize the game

  return MessageLoop();
} //WinMain