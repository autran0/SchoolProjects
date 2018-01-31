/// \file MyGame.cpp 
/// \brief Main file for your game.

// This file is made available under the GNU All-Permissive License.
// Copying and distribution of this file, with or without
// modification, are permitted in any medium without royalty
// provided the copyright notice and this notice are preserved.
// This file is offered as-is, without any warranty.

#include "GameDefines.h"
#include "WindowClass.h"
#include "SndList.h"
#include "RenderWorld.h"
#include "ObjectWorld.h"
#include "Timer.h"

#include "debug.h"

//globals
GameStateType g_nGameState; ///< Current game state.

CTimer g_cTimer; ///< The game timer. 
CSoundManager* g_pSoundManager; ///< The Sound Manager.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

BOOL g_bDrawWelcomeScreen = TRUE; ///< Whether to draw the welcome screen.

//externs
extern CWindow g_cWindow; ///< The window class.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int); ///< Default WinMain.
int  MessageLoop(); ///< Windows message loop.
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM); ///< Default window procedure.

/// \brief Create all game objects.
/// This means creating the cue-ball and the 8-ball, and resetting the
/// impulse vector on the cue-ball.

void CreateObjects(){
  const float mid = g_cWindow.GetHeight()/2.0f; //half window height

  int d = g_cRenderWorld.GetWidth(EIGHTBALL_OBJECT); //8-ball diameter
  Vector2 v = RW2OW(732.0f, mid); //initial 8-ball position
  g_cObjectWorld.create(EIGHTBALL_OBJECT, v, d); //create 8-ball

  d = g_cRenderWorld.GetWidth(CUEBALL_OBJECT); //cue-ball diameter
  v = RW2OW(295.0f, mid); //initial cue-ball position
  g_cObjectWorld.create(CUEBALL_OBJECT, v, d); //create cue-ball

  g_cObjectWorld.ResetImpulseVector(); //impulse vector points from cue-ball to 8-ball
} //CreateObjects

/// \brief Start the game.
/// Initialize the game state variable, clear the object world, and
/// create the balls.

void BeginGame(){ 
  g_nGameState = INITIAL_GAMESTATE; //playing state   
  g_cObjectWorld.clear(); //clear old objects
  CreateObjects(); //create new objects
} //BeginGame

/// \brief Initialize and start the game.
/// Initialize the Render World, load the images for the background,
/// and start the game.

void InitGame(){
  //set up Render World
  g_cRenderWorld.Initialize(g_cWindow); //bails if it fails
  g_cRenderWorld.LoadImages(); //load images from xml file list
  g_cObjectWorld.SetWorldSize(Vector2(870.0f, 405.0f));

  //now start the game
  BeginGame();
} //InitGame

/// \brief Shut down game and release resources.
/// Release the Render World and delete the Sound Manager.

void EndGame(){
  g_cRenderWorld.Release();
  SAFE_DELETE(g_pSoundManager); 
} //EndGame

/// \brief Render a frame of animation. 
/// Draw the background, draw the balls,
/// and report a win or loss if that has occurred.

void RenderFrame(){
  g_cRenderWorld.BeginFrame();
  g_cRenderWorld.DrawBackground(); //draw the background
  g_cObjectWorld.draw(); //draw the balls
  g_cRenderWorld.DrawMessage(g_nGameState); //report win or loss
  g_cRenderWorld.EndFrame();
} //RenderFrame

/// \brief Process a frame of animation.
/// This is called once per frame to animate game objects and take appropriate
/// action if the player has won or lost.

void ProcessFrame(){
  g_cTimer.beginframe(); //notify timer that frame has begun
  g_cObjectWorld.move(); //move all objects
  RenderFrame(); //render a frame of animation

  //state-based actions
  static int time = 0; //timer for restarting game after win or loss

  switch(g_nGameState){
    case BALLSMOVING_GAMESTATE:  
      if(g_cObjectWorld.AllStopped()){
        if(g_cObjectWorld.CueBallDown()){ //cue ball is down
          g_nGameState = LOST_GAMESTATE; //player has lost
          time = g_cTimer.time();
          g_pSoundManager->play(LOSE_SOUND);
        } //if
    
        else if(g_cObjectWorld.BallDown()){ //must be the 8-ball that went down
          g_nGameState = WON_GAMESTATE; //player has won
          time = g_cTimer.time();
          g_pSoundManager->play(WIN_SOUND);
        } //else if

        else{       
          g_nGameState = SETTINGUPSHOT_GAMESTATE; //player is ready for the next shot
          g_cObjectWorld.ResetImpulseVector(); //compute new impulse vector
        } //else if
      } //if
      break;

    case WON_GAMESTATE:
    case LOST_GAMESTATE:  
      if(g_cTimer.elapsed(time, 3000))BeginGame(); //restart after 3 seconds      
      break;
  } //switch
  
  g_cTimer.endframe(); //notify timer that frame has ended
} //ProcessFrame

/// \brief Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ 
  const float MOVEDELTA = 5.0f; //small change in position.
  const float ANGLEDELTA = 0.005f; //small change in angle.
  const float ANGLEDELTA2 = 0.1f; // large change in angle.

  if(g_bDrawWelcomeScreen){
    g_bDrawWelcomeScreen = FALSE; //keystroke disables welcome screen
    return FALSE;
  } //if

  switch(k){
    case VK_ESCAPE: return TRUE; //quit

    case VK_UP: 
      if(g_nGameState == INITIAL_GAMESTATE){     
        g_cObjectWorld.AdjustCueBall(MOVEDELTA); 
        g_cObjectWorld.ResetImpulseVector();
      } //if
    break;

    case VK_DOWN:  
      if(g_nGameState == INITIAL_GAMESTATE){       
        g_cObjectWorld.AdjustCueBall(-MOVEDELTA); 
        g_cObjectWorld.ResetImpulseVector();
      } //if
      break;

    case VK_LEFT:
		if (g_nGameState == SETTINGUPSHOT_GAMESTATE || g_nGameState == INITIAL_GAMESTATE)
			g_cObjectWorld.AdjustImpulseVector(ANGLEDELTA);
      break;

    case VK_RIGHT:
		if (g_nGameState == SETTINGUPSHOT_GAMESTATE || g_nGameState == INITIAL_GAMESTATE)
			g_cObjectWorld.AdjustImpulseVector(-ANGLEDELTA);
      break;

    case VK_PRIOR: //page up
      if(g_nGameState == SETTINGUPSHOT_GAMESTATE || g_nGameState == INITIAL_GAMESTATE)
        g_cObjectWorld.AdjustImpulseVector(ANGLEDELTA2);
      break;

    case VK_NEXT: //page down
      if(g_nGameState == SETTINGUPSHOT_GAMESTATE || g_nGameState == INITIAL_GAMESTATE)
        g_cObjectWorld.AdjustImpulseVector(-ANGLEDELTA2);
      break;

    case VK_SPACE:   
      switch(g_nGameState){
        case WON_GAMESTATE:
        case LOST_GAMESTATE:
          BeginGame();
        break;

        case SETTINGUPSHOT_GAMESTATE:
        case INITIAL_GAMESTATE:    
          g_cObjectWorld.Shoot();   
          g_cTimer.IncrementFrame();
          g_nGameState = BALLSMOVING_GAMESTATE;
        break;

        case BALLSMOVING_GAMESTATE:
          g_cTimer.IncrementFrame();
        break;
      } //switch
      break;

    case VK_F1:
      g_cTimer.ToggleStepMode();
      break;
  } //switch

  return FALSE;
} //KeyboardHandler

/// \brief Window procedure.
/// This is a callback function to handle messages from the OS. 
/// It's just a stub that calls DefaultWindowProc from the Engine project.
/// Dont mess with it unless you know what you're doing.
///  \param h Window handle.
///  \param m Message code.
///  \param w Parameter for message. 
///  \param l Second parameter for message.
///  \return 0 If message is handled.

LRESULT CALLBACK WindowProc(HWND h, UINT m, WPARAM w, LPARAM l){
  return DefaultWindowProc(h, m, w, l);
} //WindowProc

/// \brief Winmain.  
/// This is the main entry point for this application. 
/// It's just a stub that calls DefaultWinMain from the Engine project.
/// Dont mess with it unless you know what you're doing.
///  \param hI Handle to the current instance of this application.
///  \param hP Unused.
///  \param lpC Unused.
///  \param nCS Specifies how the window is to be shown.
///  \return TRUE If application terminates correctly.

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nCS){   
  ShowCursor(FALSE);                           
  if(!DefaultWinMain(hI, hP, lpC, nCS)) return 1;

  g_cTimer.start(); //start game timer

  InitGame(); //initialize the game

  g_pSoundManager = new CSoundManager(); //create sound manager
  g_pSoundManager->Load(); //load the sounds for this game

  return MessageLoop();
} //WinMain