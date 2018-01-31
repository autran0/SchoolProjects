/// \file Body.h
/// \brief Interface for the body class CBody.
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

#include "Spring.h"
#include "Particle.h"
#include "GameDefines.h"

/// \brief Enumerated type for points on the ragdoll.

enum PointType{
  HEAD_POINT, STERNUM_POINT, LEFT_HIP_POINT, RIGHT_HIP_POINT, 
  LEFT_SHOULDER_POINT, RIGHT_SHOULDER_POINT, LEFT_ELBOW_POINT, RIGHT_ELBOW_POINT,
  LEFT_HAND_POINT, RIGHT_HAND_POINT, LEFT_KNEE_POINT, RIGHT_KNEE_POINT, 
  LFOOT_POINT, RIGHT_FOOT_POINT, 
  NUM_POINTS
}; //PointType

/// \brief Enumerated type for bones on the ragdoll.

enum EdgeType{
  NECK_BONE,
  LEFT_COLLAR_BONE, LEFT_HUMERUS_BONE, LEFT_ULNUS_BONE,
  RIGHT_COLLAR_BONE, RIGHT_HUMERUS_BONE, RIGHT_ULNUS_BONE,  
  LEFT_THIGH_BONE, LEFT_SHIN_BONE,
  RIGHT_THIGH_BONE, RIGHT_SHIN_BONE,

  HIP_BRACER, LEFT_SHOULDER_BRACER, RIGHT_SHOULDER_BRACER,
  SHOULDER_BRACER, LEFT_NECK_BRACER, RIGHT_NECK_BRACER,

  LEFT_HEAD_SPRING, RIGHT_HEAD_SPRING,
  FEET_SPRING,
  LEFT_FOOT_SPRING, RIGHT_FOOT_SPRING,
  LEFT_KNEE_SPRING, RIGHT_KNEE_SPRING,
  HANDS_SPRING,
  LEFT_ELBOW_SPRING, RIGHT_ELBOW_SPRING,
  LEFT_HAND_SPRING, RIGHT_HAND_SPRING,

  NUM_EDGES
}; //EdgeType

/// \brief The body.
///
/// A body consists of a collection of particles and springs.
/// It maintains arrays of pointers to individual particles
/// and springs. It is assumed that the care and feeding of
/// the individual particles and springs is taken care of
/// elsewhere by separate managers. A special particle
/// is marked so that when an impulse is applied to a body,
/// a sideways impulse is also applied to that particle to
/// apply a torque to the body, which also has the pleasant
/// side-effect of making the springs twang internally.

class CBody{
  private:
    vector<CParticle*> m_stdPt; ///< Vector of pointers to particles in this body.
    vector<CParticle*> m_stdSCtr; ///< Vector of pointers to springs in this body.
    vector<CSpring*> m_stdSpr; ///< Vector of pointers to spring centers.

    void Initialize(int p, int s); ///< Reserve vector space.
    void GetSpriteType(SpriteType& b, SpriteType& s, float r); ///< Choose sprites for particles and springs.
    void ConnectSpring(int p0, int p1, int s, float r=0.5f); ///< Connect spring to particles.

    void SetSpringSprite(int spring, SpriteType sprite); ///< Create sprite to represent spring. 
    CParticle* CreateParticle(int p, SpriteType s, const Vector2& v); ///< Create sprite to represent particle.

  public:
    CParticle* MakeChain(int n, int r, float s, float a, Vector2 v); ///< Make a chain body.
    CParticle* MakeTriangle(int radius, float r, Vector2 v); ///< Make a triangle body.
    CParticle* MakeSquare(int radius, float r,  Vector2 v); ///< Make a square body.
    CParticle* MakeWheel(int sides, int radius, float r, Vector2 v); ///< Make a wheel body.
    CParticle* MakeRagdoll(Vector2 v); ///< Make a ragdoll body.

    void DeliverImpulse(float m); ///< Deliver impulse to body.
    void Teleport(float xdelta, float ydelta); ///< Magically teleport body by this amount.
    void move(); ///< Move body.
}; //CBody