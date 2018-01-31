/// \file Cannon.h
/// \brief Interface for the cannon class CCannon.
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

#include "GameDefines.h"
#include "Box2D\Box2D.h"

const float CANNONEXPLODETEMP = 150.0f; ///< Temperature at which the cannon explodes when next fired.

/// \brief The cannon object.
///
/// The cannon object is a composite made up of several sprites in Render World
/// and several bodies in Physics World. It is intended to represent the player.

class CCannon{
  friend class CObjectWorld; 

  private:
    b2Body* m_pBarrel; ///< Pointer to cannon barrel in Physics World.
    b2Body* m_pBase; ///< Pointer to cannon base in Physics World.
    b2Body* m_pWheel1; ///< Pointer to cannon wheel in Physics World.
    b2Body* m_pWheel2; ///< Pointer to cannon wheel in Physics World.

    b2WheelJoint* m_pWheelJoint1; ///< Pointer to cannon wheel joint in Physics World.
    b2WheelJoint* m_pWheelJoint2;  ///< Pointer to cannon wheel joint in Physics World.
    b2RevoluteJoint* m_pBarrelJoint;  ///< Pointer to cannon barrel body in Physics World.

    float m_fTemp; ///< Cannon temperature.
    float m_fMaxTemp; ///< Cannon maximum allowable temperature.
    int m_nBallsFired; ///< Number of cannonballs fired in current level.
    BOOL m_bExploded; ///< Whether cannon has exploded.

    b2Body* CreateMount(int x, int y, int nIndex); ///< Create a cannon mount in Physics World.
    b2Body* CreateBarrel(int x, int y, int nIndex);  ///< Create a cannon barrel in Physics World.
    b2Body* CreateWheel(int x, int y, int nIndex);  ///< Create a cannon wheel in Physics World.

    void Impulse(b2Body* b, const b2Vec2& v, const b2Vec2& ds=b2Vec2(0,0)); ///< Apply an impulse in Physics World.
    void MakeCollide(b2Body* b); ///< Make cannon parts collide-able in Physics World
    void Stop(); ///< Stop the cannon moving under its own power.

  public:
    CCannon(); ///< Constructor.
    void create(); ///< Create a cannon.
    void Explode(); ///< Make cannon explode.

    //user control over cannon
    BOOL Fire(); ///< Fire the cannon.
    void BarrelUp(float a); ///< Rotate the cannon barrel.
    void StartMovingLeft(float speed); ///< Start the cannon moving.

    int BallsFired(); ///< Get number of cannonballs fired in this level.
    BOOL IsDead(); ///< Whether cannon has exploded.
    void CoolDown(); ///< Cannon temperature drops over time.
    void Reset(); ///< Reset cannon to initial conditions.
}; //CCannon