/// \file MyGame.cpp 
/// \brief Main file for your game.

/// \mainpage Box2D Cannon Game
///
/// The aim of the Cannon Game is to use the cannon to knock down a tower of crates
/// in less than 60 seconds. If the cannon overheats, the cannon explodes and
/// you lose. If time runs out, you lose anyway. You have to get all of the crates
/// below a line approximately at the horizon to win.
///
/// Use the space bar to fire the cannon, the left and right arrow keys to 
/// start it moving to the left or right (which it will continue to do),
/// the up and down arrow keys to elevate the cannon barrel, the Enter key to restart
/// the game, and the ESC key to quit.
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
#include "SndList.h"

#include "Box2D\Box2D.h"
#include "RenderWorld.h"
#include "ObjectWorld.h"
#include "WindowClass.h"

//globals
GameStateType g_nGameState; ///< Current game state.

CTimer g_cTimer; ///< The game timer.
CSoundManager* g_pSoundManager; ///< The sound manager. 

extern CWindow g_cWindow; ///< The game class.

//game worlds
b2World g_b2dPhysicsWorld(b2Vec2(0, RW2PW(-100))); ///< Box2D Physics World.
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int); ///< Default WinMain.
int  MessageLoop(); ///< Windows message loop.
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM); ///< Default window procedure.

//prototypes for functions in nonplayerobjects.cpp
void CreateWorldEdges();
void CreateTower();

/// Create all game objects.

void CreateObjects(){
  //HUD, in Object World but not in Physics World
  g_cObjectWorld.CreateHudObject(PIRATE_OBJECT);  //easter egg

  //stuff that is in both Object and Physics Worlds
  CreateWorldEdges(); //edges of screen
  CreateTower(); //tower of crates
  g_cObjectWorld.m_cCannon.create(); //player character
} //CreateObjects

/// Start the game.

void BeginGame(){ 
  g_nGameState = PLAYING_GAMESTATE; //playing state
  g_cObjectWorld.clear(); //clear old objects
  CreateObjects(); //create new objects
  g_cObjectWorld.m_cCannon.Reset(); //reset the cannon
} //BeginGame

/// Initialize and start the game.

void InitGame(){
  //set up Render World
  g_cRenderWorld.Initialize(g_cWindow); //bails if it fails
  g_cRenderWorld.InitScreenText(); //load screen text
  g_cRenderWorld.LoadImages(); //load images from xml file list

  //set up Object World
  g_cObjectWorld.SetWorldSize(g_cWindow.GetSize());

  //now start the game
  BeginGame();
} //InitGame

/// Shut down game and release resources.

void EndGame(){
  g_cRenderWorld.Release();
  SAFE_DELETE(g_pSoundManager); 
} //EndGame

/// Render a frame of animation.

void RenderFrame(){ 
  g_cRenderWorld.BeginFrame(); //start up graphics pipeline
  g_cRenderWorld.DrawBackground(); //draw the background
  g_cObjectWorld.draw(); //draw the objects
  g_cRenderWorld.EndFrame(); //shut down graphics pipeline
} //RenderFrame

/// Process a frame of animation.
/// Called once a frame to animate game objects and take appropriate
/// action if the player has won or lost.

void ProcessFrame(){
  //stuff that gets done on every frame
  g_cTimer.beginframe(); //capture current time
  g_cObjectWorld.move(); //move all objects
  g_cObjectWorld.MakeSound(); //all objects play sounds

  RenderFrame(); //render a frame of animation

  //Stuff that's done only when playing
  if(g_nGameState == PLAYING_GAMESTATE){ 
      if(g_cObjectWorld.PlayerHasWon(4.0f)){ //won
        g_nGameState = WON_GAMESTATE;
        g_pSoundManager->play(WIN_SOUND);
      } //else if won
  } //if playing
} //ProcessFrame

/// Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ 
  const float CANNONBARREL_DELTA_ANGLE = 0.01f;
  const float CANNONMOVE_DELTA = 2.0f;
  CCannon* cannon = &(g_cObjectWorld.m_cCannon);

  //keystrokes that work in any state
  if(k ==  VK_ESCAPE)return TRUE; //quit

  if(g_nGameState == PLAYING_GAMESTATE) //keystrokes while playing
    switch(k){
      case VK_UP: //barrel up
        cannon->BarrelUp(CANNONBARREL_DELTA_ANGLE);
        break;
      case VK_DOWN: //barrel down
       cannon->BarrelUp(-CANNONBARREL_DELTA_ANGLE);
        break;
      case VK_LEFT: //move left
        cannon->StartMovingLeft(CANNONMOVE_DELTA);
        break;
      case VK_RIGHT: //move right
        cannon->StartMovingLeft(-CANNONMOVE_DELTA);
        break;
      case VK_SPACE: //fire cannon
        if(cannon->Fire())
          g_pSoundManager->play(CANNONFIRE_SOUND);
        break;
    } //switch

  else  //keystrokes while not playing
    if(k == VK_RETURN)
      BeginGame(); //restart

  //Easter egg!
  if(k == 'R'){ //arrrr!
    if(!g_cObjectWorld.m_pHeadsUpDisplay->m_bEasterEgg)
      g_pSoundManager->play(R_SOUND);
    g_cObjectWorld.m_pHeadsUpDisplay->m_bEasterEgg = TRUE;
  }

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

  InitGame(); //initialize the game

  g_pSoundManager = new CSoundManager(); //create sound manager
  g_pSoundManager->Load(); //load the sounds for this game

  return MessageLoop();
} //WinMain