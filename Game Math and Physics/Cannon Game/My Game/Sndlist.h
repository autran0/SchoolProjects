/// \file SndList.h
/// \brief Enumerated types for sounds.
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

#include "Sound.h"

/// Game sound enumerated type. 
/// These are the sounds used in actual gameplay. Sounds must be listed here in
/// the same order that they are in the sound settings XML file.

enum GameSoundType{ 
  THUMP_SOUND, THUMP2_SOUND, CANNONFIRE_SOUND, BOOM_SOUND, TICK_SOUND, 
  WIN_SOUND, LOSE_SOUND, CLANG_SOUND, 
  R_SOUND //easter egg
}; //GameSoundType