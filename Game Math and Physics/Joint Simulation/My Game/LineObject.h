/// \file LineObject.h
/// \brief Interface for the line object class CLineObject.
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

using namespace std;
#include <vector>

/// \brief A line in Object World.
///
/// A line object differs from an ordinary object in that it is
/// associated with 2 bodies in Physics World instead of just 1.
/// The line goes from an anchor on one body to an anchor on
/// another body. AN anchor is just a vector offset from the
/// center of the body in Physics World. The anchors may or 
/// may not rotate with their associated bodies.

class CLineObject: public CObject{
  friend class CObjectWorld;

  private:
    b2Body* m_pBody0; ///< Pointer to body0 in Physics World.
    b2Vec2 m_vAnchor0; ///< Anchor on body0 in Physics World.
    BOOL m_bRotates0; ///< Whether anchor rotates with body0.

    b2Body* m_pBody1; ///< Pointer to body1 in Physics World.
    b2Vec2 m_vAnchor1; ///< Anchor on body1 in Physics World.
    BOOL m_bRotates1; ///< Whether anchor rotates with body1.

  public:
    CLineObject(b2Body* b0, b2Vec2 d0, BOOL r0,
      b2Body* b1, b2Vec2 d1, BOOL r1): CObject(LINE_OBJECT),
      m_pBody0(b0), m_vAnchor0(d0), m_bRotates0(r0),
      m_pBody1(b1), m_vAnchor1(d1), m_bRotates1(r1){}; ///< Constructor.
}; //CLineObject
