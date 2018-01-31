/// \file SndList.h
/// \brief Enumerated type for sounds.
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

/// \brief Game sound enumerated type. 
/// These are the sounds used in actual gameplay. The sounds must be listed here in
/// the same order that they are in the sound settings XML file.

enum GameSoundType{ 
  BEEP_SOUND, BOOP_SOUND, BUMP_SOUND, BLASTER_SOUND, FIRE_SOUND, LAUNCH_SOUND
}; //GameSoundType