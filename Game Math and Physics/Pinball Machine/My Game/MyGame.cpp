/// \file MyGame.cpp 
/// \brief Main file for your game.
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

#include "resource.h"

#include "debug.h"

CTimer g_cTimer; ///< The game timer. 
CSoundManager* g_pSoundManager; ///< The Sound Manager.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

int g_nShowTrack = 0; ///< Show trail of breadcrumbs.
BOOL g_bShowHitPt = FALSE; ///< Show reticle at hit point.
BOOL g_bShowImpacts = FALSE; ///< Draw dots at collision points.
BOOL g_bShowArrows = FALSE; ///< Draw arrows for velocity vectors.
BOOL g_bMute = FALSE; ///< Mute the sound.

float g_fBallScale = 0.75f; ///< Scale factor for ball size.

int g_nMIterations = 4; ///< Number of motion iterations.
int g_nCIterations = 4; ///< Number of collision iterations.

HWND g_hDlg = nullptr; ///< Control dialog box handle.

//externs
extern CWindow g_cWindow; ///< The window class.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int); ///< Default WinMain.
int  MessageLoop(); ///< Windows message loop.
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM); ///< Default window procedure.

/// \brief Create all game objects.
/// Your code goes here.

void CreateObjects(){
  g_cObjectWorld.MakeShapes();
} //CreateObjects

/// \brief Start the game.
/// Initialize the game state variable, clear the object world, and
/// create the objects.

void BeginGame(){   
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
  g_cRenderWorld.InitScreenText(); //load screen text

  //set world size in Object World 
  g_cObjectWorld.SetWorldSize(g_cWindow.GetSize());

  //now start the game
  BeginGame();
} //InitGame

/// \brief Shut down game and release resources.
/// Release the Render World and delete the Sound Manager.

void EndGame(){
  g_cRenderWorld.Release();
  SAFE_DELETE(g_pSoundManager); 
} //EndGame

/// \brief Set an edit box to an integer value.
/// \param item Id of an edit box.
/// \param n Value to put into the edit box.

void SetEditBox(const int item, const int n){
  SetDlgItemInt(g_hDlg, item, n, FALSE);
} //SetEditBox

/// \brief Get integer value from an edit box.
/// \param item Id of an edit box.
/// \return Value from that edit box, 0 if there was an error.

int GetEditBox(const int item){
  BOOL ok = TRUE;
  int n = GetDlgItemInt(g_hDlg, item, &ok, FALSE);
  return ok? n: 0;
} //SetEditBox

void UpdateDialogBox(){  
  static int frames = 0; //frame counter
  static int curframes = 0; //frames rendered in previous second
  static int ft = g_cTimer.time(); //start of frame time
  
  //gather stats 

  int n = g_cObjectWorld.GetSize(); //number of balls

  if(g_cTimer.time() - ft >= 1000 && g_hDlg){ //a second has passed
    ft = g_cTimer.time(); //start frame timer
    
    SetEditBox(IDC_EDIT1, curframes);

    curframes = frames; //display frame rate for new second
    frames = 0; //start again from zero
 
    SetEditBox(IDC_EDIT3, g_cObjectWorld.GetAABBTestCnt());
    SetEditBox(IDC_EDIT4, g_cObjectWorld.GetCollisionTestCnt());
    SetEditBox(IDC_EDIT5, g_cObjectWorld.GetCollisionCnt());  
    SetEditBox(IDC_EDIT8, g_cObjectWorld.GetLostBallCnt());
  } //if
  
  ++frames; 
} //UpdateDialogBox

/// \brief Render a frame of animation. 
/// Draw the background, draw the objects,
/// and report a win or loss if that has occurred.

void RenderFrame(){
  UpdateDialogBox(); //update information in dialog box
  g_cRenderWorld.BeginFrame();
  g_cRenderWorld.DrawBackground(); //draw the background
  g_cObjectWorld.draw(); //draw the objects
  g_cRenderWorld.EndFrame();
} //RenderFrame

/// \brief Process a frame of animation.
/// This is called once per frame to animate game objects and take appropriate
/// action if the player has won or lost.

void Launch();

void ProcessFrame(){
  g_cTimer.beginframe(); //notify timer that frame has begun
  g_cObjectWorld.move(); //move all objects
  RenderFrame(); //render a frame of animation
  g_cTimer.endframe(); //notify timer that frame has ended

} //ProcessFrame

/// \brief Launch a new ball.

void Launch(){
  int n = g_cObjectWorld.GetSize(); //number of balls launched
  const int NUMBALLCOLORS = 9; //number of colors

  ObjectType obj0 = ObjectType(GRAYBALL_OBJECT + n%NUMBALLCOLORS);
  ObjectType obj1 = ObjectType(GRAYCIRCLE_OBJECT + n%NUMBALLCOLORS);

  g_cObjectWorld.create(obj0, obj1,
    Vector2(955.0f, 60.0f), //position
    20.0f*Vector2(0.0f, 1.0f), //velocity
    g_cRenderWorld.GetWidth(obj0)/2.0f, //radius
    0.9f, //elasticity
    g_fBallScale); //magnification
  
  SetEditBox(IDC_EDIT2, n + 1);

  g_pSoundManager->play(LAUNCH_SOUND); 
} //Launch

/// \brief Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM k){ 
  switch(k){
    case VK_ESCAPE: return TRUE; //quit
  } //switch
  return FALSE;
} //KeyboardHandler

/// \brief The dialog box message handler.
///
/// Processes messages from the controls on the dialog box, including 
/// radio buttons, check buttons, and edit boxes.
/// \param hDlg Handle to the dialog box.
/// \param iMsg WM code for the message.
/// \param wParam Wparam.
/// \param lParam lParam.
/// \return TRUE if the user quits.

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam){
  switch(iMsg){
    case WM_INITDIALOG: //set initial values of controls
      SetDlgItemInt(hDlg, IDC_EDIT1, 0, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT2, 0, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT3, 0, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT4, 0, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT5, 0, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT6, g_nMIterations, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT7, g_nCIterations, FALSE);
      SetDlgItemInt(hDlg, IDC_EDIT8, 0, FALSE);
  
      Button_SetCheck(GetDlgItem(hDlg, IDC_RADIO1), BST_CHECKED);
      Button_SetCheck(GetDlgItem(hDlg, IDC_RADIO5), BST_CHECKED);
      Button_SetCheck(GetDlgItem(hDlg, IDC_CHECK1), BST_CHECKED);

      Button_SetCheck(GetDlgItem(hDlg, IDC_CHECK1), g_bMute? BST_CHECKED: BST_UNCHECKED);
      break;

    case WM_COMMAND: //user has clicked on a control
      switch(LOWORD(wParam)){ //which control?
        case IDCANCEL:
          EndDialog(hDlg, -1);
          SendMessage(g_cWindow.GetHwnd(), WM_DESTROY, 0, 0);
          return TRUE;

        case IDC_BUTTON1: Launch(); break; //launch button
          
        case IDC_BUTTON2: //clear button
          g_cObjectWorld.clear(); //no objects
          g_cObjectWorld.MakeShapes(); //no shapes
          SetEditBox(IDC_EDIT2, 0); //report 0 balls
          break;

        case IDC_CHECK1: //mute checkbox
          g_bMute = !g_bMute;
          g_pSoundManager->mute();
          break;
          
        case IDC_CHECK2: //show impacts checkbox
          g_bShowImpacts = !g_bShowImpacts;
          if(g_bShowImpacts)
            g_cObjectWorld.ClearDots();
          break;
          
        case IDC_CHECK3: g_bShowHitPt = !g_bShowHitPt; break; //show hit points checkbox
        case IDC_CHECK4: g_bShowArrows = !g_bShowArrows; break; //show arrows checkbox
          
        case IDC_RADIO1: g_nShowTrack = 0; break; //show no tracks radio box       
        case IDC_RADIO2: g_nShowTrack = 1; break; //show circle tracks radio box  
        case IDC_RADIO3: g_nShowTrack = 2; break; //show dot tracks radio box  

        case IDC_RADIO4: g_fBallScale = 0.5f; break; //small ball radio button
        case IDC_RADIO5: g_fBallScale = 0.75f; break; //medium ball radio button
        case IDC_RADIO6: g_fBallScale = 1.0f; break;  //large ball radio button

        case IDC_EDIT6: { //motion iterations edit box
          int n = GetEditBox(IDC_EDIT6); //get number from edit box
          g_nMIterations = max(1, min(n, 16)); //clamp between sensible bounds
          if(g_nMIterations != n) //number in edit box is out of bounds
            SetEditBox(IDC_EDIT6, g_nMIterations); //correct it
          break;
        } //case
         
        case IDC_EDIT7: { //collision iterations edit box
          int n = GetEditBox(IDC_EDIT7); //get number from edit box
          g_nCIterations = max(1, min(n, 64)); //clamp between sensible bounds 
          if(g_nCIterations != n) //number in edit box is out of bounds
            SetEditBox(IDC_EDIT7, g_nCIterations); //correct it
          break;
        } //case
      } //switch
      break;
  } //switch

  return FALSE;
} //DlgProc

/// \brief Window procedure.
/// This is a callback function to handle messages from the OS. 
/// It's just a stub that calls DefaultWindowProc from the Engine project.
/// Dont mess with it unless you know what you're doing.
/// \param h Window handle.
/// \param m Message code.
/// \param w Parameter for message. 
/// \param l Second parameter for message.
/// \return 0 If message is handled.

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
  if(!DefaultWinMain(hI, hP, lpC, nCS)) return 1;

  g_hDlg =CreateDialog(hI, MAKEINTRESOURCE(IDD_DIALOG1), g_cWindow.GetHwnd(), (DLGPROC)DlgProc);
  ShowWindow(g_hDlg, SW_SHOW); 

  g_cTimer.start(); //start game timer
  InitGame(); //initialize the game

  g_pSoundManager = new CSoundManager(); //create sound manager
  g_pSoundManager->Load(); //load the sounds for this game
  if(g_bMute)g_pSoundManager->mute(); //mute it if necessary

  return MessageLoop();
} //WinMain