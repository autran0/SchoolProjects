/// \file RenderWorld.h 
/// \brief Interface for the render world class CRenderWorld.
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

#pragma once

#include "Renderer.h"
#include "GameDefines.h"

/// \brief The render world.
///
/// Render world handles the game-specific rendering tasks, relying on
/// CRenderer to do all of the actual API-specific rendering.

class CRenderWorld: public CRenderer{
public:
  void DrawWinLoseMessage(int shots, GameStateType state, int secs); ///< Draw win/lose text.
  void LoadImages(); ///< Load images.
}; //CRenderWorld