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

/// Load game images. Gets file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ //load images
  InitBackground();
  LoadBackground(); 
  
  //Load sprite for each object
  Load(TIRE_OBJECT, "tire"); 
  Load(CRATE_OBJECT, "crate");
  Load(WINDMILLBASE_OBJECT, "windmillbase");
  Load(WINDMILLBLADES_OBJECT, "windmillblades");
} //LoadImages