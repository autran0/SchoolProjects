/// \file ObjectWorld.h
/// \brief Interface for the Object World class CObjectWorld.
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

#include "Object.h"

#include <vector>
#include <functional>

#include "LineSeg.h"
#include "MovingCircle.h"

using namespace std;

const int NUMRETICLES = 256; ///< Number of reticles.

/// \brief The reticle.
///
/// A reticle is a round thing that gets drawn on the screen to mark recent points of impact.
/// This object stores the reticle's type, when it was created, and its location.

class CReticle{
  friend class CObjectWorld;

  private:
    ObjectType m_eObject; ///< Object type.
    int m_nBirthTime; ///< Creation time.
    Vector2 m_vPos; ///< Position.

  public:
    CReticle(const ObjectType obj, const int t, const Vector2& v): 
      m_eObject(obj), m_nBirthTime(t), m_vPos(v){}; ///< Constructor.
    CReticle(): CReticle(UNKNOWN_OBJECT, 0, Vector2(0.0f)){}; ///< Default constructor.
}; //CReticle

/// \brief The object world.
///
/// The Object World is an abstract representation of all of the objects
/// in the game.

class CObjectWorld{
  private:
    vector<CObject*> m_stdList; ///< Object list.
    vector<CShape*> m_stdShape; ///< Shape list.
    vector<CReticle> m_stdDot; ///< Dot list.
    vector<CPoint*> m_stdPoint; ///< Point list.

    Vector2 m_vSize; ///< Width and height of Object World.
    CAabb2D m_cAABB; ///< AABB for playable part of Object World.
    int m_nLostBalls; ///< Number of lost balls.

    CReticle m_cReticle[NUMRETICLES]; ///< Reticle array.
    int m_nNumReticles; ///< Number of retuicles.
    int m_nFirstReticle; ///< Index of first reticle.
    int m_nNextReticle; ///< Next imdex after the last reticle.

    float m_fLHPAngle; ///< Angle of reticle at last hit point.
    
    int m_nCollisionCnt; ///< Number of collisions in current frame.
    int m_nCollisionTestCnt; ///< Number of collision tests in current frame.
    int m_nAABBTestCnt; ///< Number of AABB collision tests in current frame.


    void BroadPhase(); ///< Broad phase collision detection and response.
    void NarrowPhase(CObject* p0, CObject* p1); ///< Narrow phase collision detection and response.

  public:
    CObjectWorld(); ///< Constructor.
    ~CObjectWorld(); ///< Destructor.

    void SetWorldSize(const Vector2& s); ///< Set Object World size.
    void GetWorldSize(float &w, float &h); ///< Get Object World dimensions.

    void create(ObjectType t, ObjectType c, const Vector2& s, const Vector2& v, float r, float e, float m); ///< Create new object.

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects.
    void draw(); ///< Draw all objects.

    void MakeShapes(); ///< Create shapes.
    int GetSize(); ///< Get number of moving objects.
    void ClearDots(); ///< Clear the dot list.

    int GetCollisionCnt(); ///< Get number of collision this frame.
    int GetCollisionTestCnt(); ///< Get number of collision tests this frame.
    int GetAABBTestCnt(); ///< Get number of AABB tests this frame.
    int GetLostBallCnt(); ///< Get number of lost balls.
}; //CObjectWorld