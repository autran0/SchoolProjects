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

/// Load game images. Gets the file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ 
  InitBackground();
  LoadBackground(); 
  
  //Load sprite for each object
  Load(ARROW_OBJECT, "arrow"); 

  Load(REDBALL_OBJECT, "redball"); 
  Load(REDCIRCLE_OBJECT, "redcircle"); 
  Load(REDRETICLE_OBJECT, "redreticle"); 
  Load(REDDOT_OBJECT, "reddot"); 

  Load(BLUEBALL_OBJECT, "blueball"); 
  Load(BLUECIRCLE_OBJECT, "bluecircle"); 
  Load(BLUERETICLE_OBJECT, "bluereticle"); 
  Load(BLUEDOT_OBJECT, "bluedot"); 
  
  Load(YELLOWBALL_OBJECT, "yellowball"); 
  Load(YELLOWCIRCLE_OBJECT, "yellowcircle"); 
  Load(YELLOWRETICLE_OBJECT, "yellowreticle"); 
  Load(YELLOWDOT_OBJECT, "yellowdot"); 
  
  Load(GRAYBALL_OBJECT, "grayball"); 
  Load(GRAYCIRCLE_OBJECT, "graycircle"); 
  Load(GRAYRETICLE_OBJECT, "grayreticle"); 
  Load(GRAYDOT_OBJECT, "graydot"); 

  Load(GREENBALL_OBJECT, "greenball"); 
  Load(GREENCIRCLE_OBJECT, "greencircle"); 
  Load(GREENRETICLE_OBJECT, "greenreticle");
  Load(GREENDOT_OBJECT, "greendot");  
  
  Load(MAGENTABALL_OBJECT, "magentaball"); 
  Load(MAGENTACIRCLE_OBJECT, "magentacircle"); 
  Load(MAGENTARETICLE_OBJECT, "magentareticle"); 
  Load(MAGENTADOT_OBJECT, "magentadot"); 
  
  Load(CYANBALL_OBJECT, "cyanball"); 
  Load(CYANCIRCLE_OBJECT, "cyancircle"); 
  Load(CYANRETICLE_OBJECT, "cyanreticle"); 
  Load(CYANDOT_OBJECT, "cyandot"); 
  
  Load(ORANGEBALL_OBJECT, "orangeball"); 
  Load(ORANGECIRCLE_OBJECT, "orangecircle"); 
  Load(ORANGERETICLE_OBJECT, "orangereticle"); 
  Load(ORANGEDOT_OBJECT, "orangedot"); 
  
  Load(PURPLEBALL_OBJECT, "purpleball"); 
  Load(PURPLECIRCLE_OBJECT, "purplecircle"); 
  Load(PURPLERETICLE_OBJECT, "purplereticle"); 
  Load(PURPLEDOT_OBJECT, "purpledot"); 

  Load(BIGREDRETICLE_OBJECT, "bigredreticle"); 
} //LoadImages
