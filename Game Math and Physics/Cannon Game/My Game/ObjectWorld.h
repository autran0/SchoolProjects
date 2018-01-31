/// \file ObjectWorld.h
/// \brief Interface for the object world class CObjectWorld.
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

#include "Hud.h"
#include "Cannon.h"
#include "Object.h"

/// \brief The object world.
///
/// The Object World is an abstract representation of all of the objects
/// in the game.

class CObjectWorld{
  friend BOOL KeyboardHandler(WPARAM k); 
  friend void CreateObjects();
  friend void BeginGame();
  friend void RenderFrame();
  friend void ProcessFrame();

  private:
    vector<CObject*> m_stdList; ///< Object list.
    Vector2 m_vSize; ///< Width and height of Object World.

    CCannon m_cCannon; ///< The cannon.
    CHeadsUpDisplay* m_pHeadsUpDisplay; ///< The HUD.

  public:
    CObjectWorld(); ///< Constructor.
    ~CObjectWorld(); ///< Destructor.

    CObject* create(ObjectType t); ///< Create new object.
    CObject* CreateHudObject(ObjectType t); ///< Create new object.

    void SetWorldSize(const Vector2& s); ///< Set Object World size.
    void GetWorldSize(float &width, float &height); ///< Get the Object World width and depth.

    BOOL PlayerHasWon(float level); ///< Detect whether the player has won.

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects. 
    void draw(); ///< Draw all objects.
    void MakeSound(); ///< Make sounds for all objects.
}; //CObjectWorld