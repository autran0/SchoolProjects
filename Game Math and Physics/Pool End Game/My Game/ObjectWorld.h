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

using namespace std;

/// \brief The object world.
///
/// The Object World is an abstract representation of all of the objects
/// in the game.

class CObjectWorld{
  typedef vector<CObject*>::const_iterator OLit; ///< Object list iterator.

  private:
    vector<CObject*> m_stdList; ///< Object list.
    Vector2 m_vSize; ///< Width and height of Object World.

    int m_nIterations; ///< Number of iterations of collision-response.

    float m_fCueAngle; ///< Cue ball impulse angle.
	float m_fCircleAngle; ///< Circle Angle
    CObject* m_pCueBall; ///< Cue ball object pointer.
    CObject* m_p8Ball; ///< 8 ball object pointer.
    BOOL m_bDrawImpulseVector; ///< Whether to draw the impulse vector.

    BOOL BallCollision(Vector2& b1Pos, Vector2& b1Vel, 
      Vector2& b2Pos, Vector2& b2Vel, const float r, float& s); ///< Ball collision response for two balls.
    BOOL BallCollision(CObject* b1, CObject* b2); ///< Ball collision response for two balls.
    void BallCollision(OLit i); ///< Ball collision response for a single ball.

	BOOL CreateCircle(Vector2& b1Pos, Vector2& b1Vel,
		Vector2& b2Pos, Vector2& b2Vel, const float r, float& s); ///< Creates a circle at TOI

    BOOL RailCollision(float& s, float& v, float r, 
      function<bool(float, float)> hit); ///< Collision response for ball with rail.
    void RailCollision(CObject* b); ///< Collision response for ball with rail.

    void PocketCollision(CObject* b); ///< Collision response for ball with pocket.

    void Collision(); ///< Ball, rail, and pocket collision response for all balls.

  public:
    CObjectWorld(); ///< Constructor.
    ~CObjectWorld(); ///< Destructor.

    void SetWorldSize(const Vector2& s); ///< Set Object World size.
    void GetWorldSize(float &w, float &h); ///< Get Object World dimensions.

    void create(ObjectType t, const Vector2& v, int s); ///< Create new object.

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects.
    void draw(); ///< Draw all objects.
    
    void ResetImpulseVector(); ///< Reset the Impulse Vector.
    void AdjustImpulseVector(float a); ///< Adjust the Impulse Vector.
    void AdjustCueBall(float d); ///< Move cue-ball up or down.
    void Shoot(); ///< Shoot the cue ball.

    BOOL BallDown(); ///< Is a ball down in a pocket?
    BOOL CueBallDown(); ///< Is the cue ball down in a pocket?
    BOOL AllStopped(); ///< Have all balls stopped moving?
}; //CObjectWorld