/// \file RenderWorld.cpp
/// \brief Code for the render world class CRenderWorld.
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

#include "RenderWorld.h"
#include "debug.h"

/// Load game images. Gets file list from gamesettings.xml.

void CRenderWorld::LoadImages(){ //load images
  InitBackground();
  LoadBackground(); 
  
  Load(LINE_OBJECT, "line");
  Load(CRATE_OBJECT, "bucket");
  Load(SAFE_OBJECT, "safe");
  Load(ELEPHANT_OBJECT, "elephant");
  Load(PULLEY_OBJECT, "pulley");
  Load(PULLEY_OBJECT_2, "pulley2");
} //LoadImages

/// This function uses the renderer's draw text function to draw
/// text on the screen.
/// \param t Null terminated text string to be drawn.

void CRenderWorld::DrawScreenText(char* t){
  drawtext(t, FALSE);
} //DrawScreenText