/// \file Pulley.h
/// \brief Interface for the pulley class CPulley.
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

#include "Object.h"
#include "Box2D\Box2D.h"

/// \brief The pulley class.
/// The pulley class represents a system of pulleys in Physics World.

class CPulley{
  private:
    b2PulleyJoint* m_pJoint; ///< Pointer to the pulley joint.
    float m_fJointLenA; ///< Length of left side of pulley.

    b2Body* m_pWheel0; ///< Pointer to the left pulley wheel.
    b2Body* m_pWheel1; ///< Pointer to the right most pulley wheel.
	b2Body* m_pWheel2; ///< Pointer to the second right most pulley wheel.
	b2Body* m_pWheel3; ///< Pointer to the third right most pulley wheel.
	b2Body* m_pWheel4; ///< Pointer to the fourth right most pulley wheel.
    float m_fWheelRad; ///< Pulley wheel radius.
	float m_fWheel2Rad; ///< Pulley 2 wheel radius.

    BOOL m_bElephant; ///< TRUE if elephant exists.
    b2Vec2 m_vSpawnPt; ///< The elephant's spawn point in Physics World.

    b2Body* CreateWheel(float x, float y, ObjectType type); ///< Create a pulley wheel.
    b2Body* CreateCrate(float x, float y); ///< Create the elephant's crate.
    b2Body* CreateObject(float x, float y, float d, ObjectType t); ///< Create an object.

  public:
    CPulley(float x, float y, float w); ///< Constructor.
    void move(); ///< Rotate the pulley wheels.
    void ToggleElephant(); ///< Toggle the existence of the elephant.
}; //CPulley