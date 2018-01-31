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

#include "debug.h"

/// Load game images. Gets the file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ 
  InitBackground();
  LoadBackground(); 
  
  //Load sprites
  Load(BALL_SPRITE, "ball"); 
  Load(WOODCIRCLE_SPRITE, "woodcircle"); 
  Load(SPRING_SPRITE, "spring"); 
  Load(STICK_SPRITE, "stick"); 
} //LoadImages