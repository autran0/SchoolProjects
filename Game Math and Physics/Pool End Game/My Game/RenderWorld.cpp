/// \file RenderWorld.cpp
/// \brief Code for the render world class CRenderWorld.
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

#include "RenderWorld.h"
#include "debug.h"

extern BOOL g_bDrawWelcomeScreen;

/// Load game images. Gets the file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ 
  InitBackground();
  LoadBackground(); 
  
  //Load sprite for each object
  Load(ARROW_OBJECT, "arrow"); 
  Load(CUEBALL_OBJECT, "cueball"); 
  Load(EIGHTBALL_OBJECT, "eightball"); 
  Load(CIRCLE_OBJECT, "circle");
  Load(YOUWIN_OBJECT, "youwin"); 
  Load(YOULOSE_OBJECT, "youlose"); 
  Load(WELCOME_OBJECT, "welcome"); 
} //LoadImages
 
/// Tell the player whether they've won or lost by plastering a text banner across the screen.
/// Also handles the welcome image at the start of the game, which isn't reflected in the
/// function name, but then nobody is perfect.
/// \param s The game state, which tells whether the player has won or lost.

void CRenderWorld::DrawMessage(const GameStateType s){
  Vector3 v = Vector3(m_nScreenWidth/2.0f, m_nScreenHeight/2.0f, -200.0f); //screen center
  switch(s){ //state-specific actions
    case WON_GAMESTATE:
      draw(YOUWIN_OBJECT, v);
      break;

    case LOST_GAMESTATE:
      draw(YOULOSE_OBJECT, v);
      break;

    case INITIAL_GAMESTATE:
      if(g_bDrawWelcomeScreen)
        draw(WELCOME_OBJECT, v);
      break;
  } //switch
} //DrawMessage