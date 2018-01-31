/// \file GameDefines.h
/// \brief Game specific defines.
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

#include "Defines.h"

/// \brief Object type.
/// Types of object that can appear in the game. 

enum ObjectType{
  ARROW_OBJECT, CUEBALL_OBJECT, EIGHTBALL_OBJECT, CIRCLE_OBJECT,
  YOUWIN_OBJECT, YOULOSE_OBJECT, WELCOME_OBJECT
}; //ObjectType

/// \brief Game state type.
/// State of game play, including whether the player has won or lost.

enum GameStateType{
  INITIAL_GAMESTATE, BALLSMOVING_GAMESTATE,  SETTINGUPSHOT_GAMESTATE, 
  WON_GAMESTATE, LOST_GAMESTATE
}; //GameStateType

//Translate units between Render World and Object World

static const Vector2 MARGIN = Vector2(78.0f, 64.0f); ///< Distance from top left corner of image to top left of table.

/// \brief Object World to Render World units.
inline Vector2 OW2RW(const Vector2 v){
  return v + MARGIN;
} //OW2RW

/// \brief Object World to Render World units.
inline Vector2 OW2RW(const float x, const float y){
  return Vector2(x, y) + MARGIN;
} //OW2RW

/// \brief Render World to Object World units.
inline Vector2 RW2OW(const Vector2 v){
  return v - MARGIN;
} //RW2OW

/// \brief Render World to Object World units.
inline Vector2 RW2OW(const float x, const float y){
  return Vector2(x, y) - MARGIN;
} //RW2OW
