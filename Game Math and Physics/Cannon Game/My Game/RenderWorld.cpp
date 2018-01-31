/// \file RenderWorld.cpp
/// \brief Code for the render world CRenderWorld.
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

/// Load game images. Gets file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ 
  InitBackground();
  LoadBackground(); 
  
  //Load sprite for each object
  Load(BALL_OBJECT, "ball"); 
  Load(CRATE_OBJECT, "crate");
  Load(BARREL_OBJECT, "cannonbarrel");
  Load(MOUNT_OBJECT, "cannonmount");
  Load(WHEEL_OBJECT, "wheel");
  Load(TEMPGUAGE_OBJECT, "tempguage");
  Load(TEMPNEEDLE_OBJECT, "tempneedle");
  Load(TEMPMAXNEEDLE_OBJECT, "tempmaxneedle");
  Load(CLOCKFACE_OBJECT, "clockface");
  Load(CLOCKNEEDLE_OBJECT, "clockneedle");
  Load(PIRATE_OBJECT, "pirate");  //easter egg
} //LoadImages
 
/// Tell the player whether they've won or lost by plastering a text banner across the screen.
/// \param shots Number of shots fired.
/// \param state The game state, which tells whether the player has won or lost.
/// \param secs Time to completion, in seconds.

void CRenderWorld::DrawWinLoseMessage(int shots, GameStateType state, int secs){
  const int BUFSIZE = 256;
  char buffer[BUFSIZE];

  switch(state){
    case WON_GAMESTATE:
      if(shots == 1)
        sprintf_s(buffer, BUFSIZE, "You won in %d seconds with 1 shot", secs);
      else
        sprintf_s(buffer, BUFSIZE, "You won in %d seconds with %d shots", secs, shots);
      drawtext(buffer);
      break;

    case LOST_GAMESTATE:
      drawtext("You Lose");
      break;
  } //switch
} //DrawWinLoseMessage