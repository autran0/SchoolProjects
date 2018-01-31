/// \file Hud.h
/// \brief Interface for the heads-up display class CHeadsUpDisplay.
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

#include <vector>

using namespace std;

#include "Object.h"

/// \brief The heads-up display.
///
/// The heads-up display class is responsible for managing parts of the heads-up display
/// (abbreviated HUD), which consists of a temperature guage and a clock, both of
/// which are animated.

class CHeadsUpDisplay{
  private:
    vector<CObject*> m_stdList; ///< Object list.

  public:
    BOOL m_bEasterEgg; ///< Has the easter egg been activated?

    void draw(int secs, float temp, float mt); ///< Draw HUD.
    CObject* create(ObjectType t); ///< Create HUD object.
}; //CHeadsUpDisplay