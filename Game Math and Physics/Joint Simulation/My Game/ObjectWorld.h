/// \file ObjectWorld.h
/// \brief Interface for the object world class CObjectWorld.
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

#pragma once

#include <vector>

using namespace std;

#include "Object.h"

/// \brief The Object World.
///
/// The Object World is an abstract representation of all of the objects
/// in the game.

class CObjectWorld{
  private:
    vector<CObject*> m_stdList; ///< Object list.
    Vector2 m_vSize; ///< Width and height of Object World.

  public:
    ~CObjectWorld(); ///< Destructor.

    void SetWorldSize(const Vector2& s); ///< Set Object World size.
    void GetWorldSize(float &width, float &height); ///< Get Object World dimensions.

    CObject* create(ObjectType t); ///< Create new object.
    CObject* CreateLine(b2Body* b0, const b2Vec2& d0, BOOL r0, b2Body* b1, const b2Vec2& d1, BOOL r1);

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects.
    void draw(); ///< Draw all objects.

    void DeleteLastObject(ObjectType t); ///< Delete the last object in the object list if it is type t.
}; //CObjectWorld