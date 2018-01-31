/// \file MyGame.cpp 
/// \brief Main file for your game.
///
/// \mainpage Box2D Joint Demo
///
/// This is a simple toy that demonstrates some Box2D joints, including
/// revolute, gear, prismatic, wheel, distance, and pulley joints.
/// The space bar changes something in the current demo, the backspace
/// key restarts it, and the enter key goes on to the next demo.
/// There are 7 demos in all. 
///
/// <ol>
/// <li> Demo 1 constructs a windmill using a revolute joint with a motor. 
/// The space bar reverses the motor's direction of rotation. </li>
///
/// <li>Demo 2 uses a collection of revolute and
/// gear joints to build a system of 9 interlocked cogs.The largest
/// cog has a motor, while the others are driven by their respective
/// gear joints. The space bar reverses the motor's direction of
/// rotation. There are 4 square gears which fortunately act
/// like round gears in the sense that they rotate at constant speed.</li>
///
/// <li>Demo 3 demonstrates a system of 5 nautilus gears, which are cogs
/// in the shape of a nautilus shell. The center cog has a revolute
/// joint with a motor.  The space bar reverses the motor's direction of rotation.
/// The other cogs have variable speed, and so
/// must be implemented as static objects and animated in code.</li>
///
/// <li>Demo 4 uses revolute, gear, and prismatic joints to build a
/// rack-and-pinion, consisting of a round cog connected to a toothed straight
/// rack. As the cog rotates, the rack moves horizontally. The cog has a
/// revolute joint with a motor. The space bar reverses the motor's direction of rotation.</li>
///
/// <li>Demo 5 uses two wheel joints to build a car that drives back and forth over a pair of
/// ramps. The wheel joints have motors. The space bar reverses the motors' directions of rotation.</li>
///
/// <li>Demo 6 uses distance joints to construct Newton's cradle with 6 balls.
/// When the balls are stopped, the space bar lifts and releases from 1 to 5 balls in sequence.
/// When the balls are in motion, the space bar stops them.</li>
///
/// <li>Demo 7 uses a pulley joint to lift up a safe when an elephant is dropped
/// into the cradle. The pulley wheels and ropes are animated in code.
/// The space bar makes the elephant appear/disappear.
/// </li>
/// </ol>
///
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

#include "Pulley.h"

#include "debug.h"

extern CWindow g_cWindow; ///< The window class.

//globals
CTimer g_cTimer; ///< The game timer.

//Physics World
b2World g_b2dPhysicsWorld(b2Vec2(0, RW2PW(-1000))); ///< Box2D Physics World.

static CPulley* g_pPulley = nullptr; ///< Pulley system in Physics World.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int); ///< Default WinMain.
int  MessageLoop(); ///< Windows message loop.
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM); ///< Default window procedure.

/// \brief Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// top, bottom, right, and left edges of the screen in Render World.

void CreateWorldEdges(){
  float w, h;
  g_cObjectWorld.GetWorldSize(w, h);

  //Box2D ground
  b2BodyDef bd;
  b2Body* edge = g_b2dPhysicsWorld.CreateBody(&bd);
  b2EdgeShape shape;
  shape.Set(b2Vec2(0, 0), 
    b2Vec2(RW2PW(w), 0));
  edge->CreateFixture(&shape, 0);

  //Box2D left edge of screen
  edge = g_b2dPhysicsWorld.CreateBody(&bd);
  shape.Set(b2Vec2(0, RW2PW(-1000)), b2Vec2(0, RW2PW(1000)));
  edge->CreateFixture(&shape, 0);

  //Box2D right edge of screen
  bd.position.x = RW2PW(w);
  edge = g_b2dPhysicsWorld.CreateBody(&bd);
  edge->CreateFixture(&shape, 0); 
} //CreateWorldEdges

/// \brief Create all game objects.

void CreateObjects(){
  int w, h; //screen width and height
  g_cWindow.GetSize(w, h);

  SAFE_DELETE(g_pPulley);
  g_pPulley = new CPulley(RW2PW(w)/2.0f, RW2PW(h)/2.0f, RW2PW(400));
} //CreateObjects

/// \brief Start the game.

void BeginGame(){ 
  g_cTimer.StartLevelTimer(); //starting level now     
  g_cObjectWorld.clear(); //clear old objects
  CreateObjects(); //create new objects
} //BeginGame

/// \brief Initialize and start the game.

void InitGame(){
  //set up Render World
  g_cRenderWorld.Initialize(g_cWindow); //bails if it fails
  g_cRenderWorld.InitScreenText(); //load screen text
  g_cRenderWorld.LoadImages(); //load images from xml file list

  //set up Object World
  g_cObjectWorld.SetWorldSize(g_cWindow.GetSize());

  //put boundaries on the screen edges
  CreateWorldEdges();

  //now start the game
  BeginGame();
} //InitGame

/// \brief Shut down game and release resources.

void EndGame(){
  g_cRenderWorld.Release();
  SAFE_DELETE(g_pPulley);
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
  g_cTimer.beginframe(); //capture current time
  g_cObjectWorld.move(); //move all objects

  if(g_pPulley)g_pPulley->move();

  RenderFrame(); //render a frame of animation
} //ProcessFrame

/// \brief Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ //keyboard handler
  //variables for use the NEWTON_GAMESTATE
  static int i = 1; //number of balls to move
  static BOOL parity = TRUE; //deliver impulse when TRUE, stop all when FALSE

  //keystrokes that work in any state
  if(k == VK_ESCAPE)return TRUE; //quit

  else if(k == VK_BACK){
    i = 1; parity = TRUE;
    BeginGame();
  } //else if

  else if(k == VK_SPACE)
    if(g_pPulley)
      g_pPulley->ToggleElephant(); //make elephant disappear/reappear

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