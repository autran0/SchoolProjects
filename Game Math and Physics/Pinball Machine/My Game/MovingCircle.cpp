/// \file MovingCircle.cpp
/// \brief Code for the moving circle class CMovingCircle.
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

#include "MovingCircle.h"
#include "GameDefines.h"
#include "debug.h"

void EnforceSpeedLimit(Vector2& v){    
  if(v.LengthSquared() > MAXSPEEDSQ){
    v.Normalize();
    v *= MAXSPEED;
  } //if
} //EnforceSpeedLimit

CMovingCircle::CMovingCircle():
  CMovingCircle(Vector2(0.0f), 0.0f){
} //constructor

/// Construct a moving circle with zero velocity, given its position and radius.
/// \param p Position.
/// \param r Radius.

CMovingCircle::CMovingCircle(const Vector2& p, float r):
  CMovingCircle(p, Vector2(0.0f, 1.0f), r){
} //constructor

/// Construct a moving circle given its position, velocity, radius, elasticity, and magnification.
/// \param p Position.
/// \param v Velocity.
/// \param r Radius.
/// \param e Elasticity.
/// \param m Magnification.

CMovingCircle::CMovingCircle(const Vector2& p, const Vector2& v, float r, float e, float m):
  CCircle(p, r, e), m_vVel(v), m_fMass(XM_PI*r*r*r), m_fScale(m){  
  m_eShape = MOVINGCIRCLE_SHAPE;
} //constructor

/// Reflect velocity about a normal in the direction of a given vector.
/// \param n Vector to reflect in. It doesn't have to be a unit vector.
/// \param e Elasticity.

void CMovingCircle::ReflectVelocity(Vector2 n, const float e){
  if(n != Vector2(0.0f)){
    n.Normalize(); //we do need a unit vector, however
    m_vVel -= (1.0f + e)*m_vVel.Dot(n)*n; 
  } //if
} //ReflectVelocity

/// Moving circle to point collision detection and response.
/// Adjust the position and velocity after collision with a point.
/// \param P Pointer to the point to collide with.
/// \return TRUE if there was a collision.

BOOL CMovingCircle::Collide(CPoint* P, Vector2& poi){
  return Collide(&CCircle(P->m_vPos, 0.0f, P->m_fElasticity), poi); //a point is just a circle with zero radius
} //Collide

/// Moving circle to infinite line collision detection and response.
/// Adjust the position and velocity after collision with a line.
/// \param L Pointer to the line to collide with.
/// \param poi [out] Point of impact.
/// \return TRUE if there was a collision.

BOOL CMovingCircle::Collide(CLine* L, Vector2& poi){
  const Vector2 p0 = m_vPos; //shorthand
  const Vector2 v = m_vVel; //shorthand
  const float r = m_fRadius; //shorthand

  const float e = m_fElasticity*L->m_fElasticity;

  Vector2 p1 = L->ClosestPoint(p0); //closest point on L to p0

  if((p1 - p0).LengthSquared() >= m_fRadiusSq) //bail and fail if its more than one radius away
    return FALSE; 

  if((p1 - p0).Dot(v) <= 0.0f) //bail early if heading away
    return FALSE;

  //general case
  Vector2 nhat; //normal to L
  if(!L->GetNormal(-v, nhat))return FALSE; //get the normal
  Vector2 p2 = p1 + r*nhat; //position of center of moving circle 

  CLine L1(p2, L->m_fGradient); //L1 is parallel to L passing through p2  
  CLine L2(p0, v); //this circle is moving along L2

  Vector2 p3; //for center of circle at TOI
  L1.Intersect(&L2, p3); //get center of circle at TOI
  m_vPos = p3; //adjust position back to TOI
  poi = p3 - r*nhat; //get poi

  const float d = (p3 - p0).Length(); //setback distance
  const float t = d/m_vVel.Length(); //time to TOI

  ReflectVelocity(nhat, e); //adjust velocity
  EnforceSpeedLimit(m_vVel); //not too fast

  m_vPos += m_vVel*t; //movement after impact

  return TRUE;
} //Collide

/// Moving circle to line segment collision detection and response.
/// Adjust the position and velocity after collision with a line.
/// \param L Pointer to the line segment to collide with.
/// \param poi [out] Point of impact.
/// \return TRUE if there was a collision.

BOOL CMovingCircle::Collide(CLineSeg* L, Vector2& poi){
  //one-way line segment
  if(L->m_bOneWay && m_vVel.Dot(L->m_vCanCross) > 0.0f)
    return FALSE;

  //end points
  if(!L->m_bOpen && (Collide(L->m_pPoint0, poi) || Collide(L->m_pPoint1, poi))) 
    return TRUE;
  
  const Vector2 p0 = m_vPos; //shorthand
  Vector2 p1 = L->ClosestPoint(p0); //closest point on L to p0
  if(!L->Intersect(p1))
    return FALSE;

  CMovingCircle C = *this; //copy of this circle 

  //C collides with infinite line and poi is in line segment
  if(C.Collide((CLine*)L, poi) && L->Intersect(poi)){ 
    //collision with C is good enough
    m_vPos = C.m_vPos;
    m_vVel = C.m_vVel;
    return TRUE;
  } //if

  return FALSE;
} //Collide

/// Get the setback distance when colliding with a static circle at a given velocity.
/// The setback distance is the distance that we have to move back from the position
/// the time that the collision was actually detected to the position at the
/// time of impact.
/// \param c Pointer to a static circle.
/// \param vhat Velocity direction at time of collision.
/// \param d [out] Setback distance.
/// \return TRUE if there was a collision.

BOOL CMovingCircle::GetDistToTOI(CCircle* C, const Vector2& vhat, float& d){
  const Vector2 p0 = C->m_vPos; //center of the other circle
  const Vector2 p1 = m_vPos; //center of the other circle
  const Vector2 c = p0 - p1; //vector from p1 to p0

  const float r2 = m_fRadiusSq + 2.0f*m_fRadius*C->m_fRadius + C->m_fRadiusSq; //square of the sum of the radii

  if(c.LengthSquared() > r2) //bail early if too far away
    return FALSE;

  if(c.Dot(vhat) <= 0.0f) //bail early if heading away
    return FALSE;

  CLine L(c, vhat); //line through p0 parallel to velocity vector relative to center
  const float m = L.m_fGradient; //shorthand

  Vector2 q0, q1; //candidates for poi
  if(isfinite(m)){ //not infinite gradient, i.e. not vertical line
    const float b = L.m_fYIntercept; //shorthand for y-intercept

    //find roots of quadratic equation
    const float radicand = b*b*m*m - (m*m + 1)*(b*b - r2); //thing we take square root of
    if(radicand <= 0.0f)return FALSE; //for safety - this should almost never happen 

    //compute current position of where p0 hit the perimeter at TOI; there are 2 candidates
    const float x0 = (-b*m + sqrt(radicand))/(m*m + 1); 
    q0 = Vector2(x0, m*x0 + b); //first candidate

    const float x1 = (-b*m - sqrt(radicand))/(m*m + 1); 
    q1 = Vector2(x1, m*x1 + b); //second candidate
  } //if

  else{ //vertical line
    const float b = L.m_fXIntercept; //shorthand for x-intercept

    //find roots of quadratic equation
    const float radicand = r2 - b*b; //thing we take square root of
    if(radicand <= 0.0f)return FALSE; //for safety - this should almost never happen 

    //compute the current location of where p0 hit the perimeter at TOI; there are 2 candidates
    const float y0 = sqrt(radicand);
    q0 = Vector2(b, y0); //first candidate
    q1 = Vector2(b, -y0); //second candidate
  } //else

  //compute setback distance
  const float d0 = (q0 - c).Length(); //first candidate
  const float d1 = (q1 - c).Length(); //second candidate

  d = (q0 - c).Dot(vhat) > 0.0f? d0: d1; //pick the one in front

  return TRUE;
} //GetDistToTOI

/// Moving circle to static circle collision detection and response.
/// Adjust the position and velocity after collision with a static circle.
/// \param c Pointer to a static circle to collide with.
/// \param poi [out] Point of impact.
/// \return TRUE if there was a collision.

BOOL CMovingCircle::Collide(CCircle* C, Vector2& poi){
  //shorthands
  const Vector2 p0 = C->m_vPos; //static circle position
  Vector2& p1 = m_vPos; //moving circle position
  const float r1 = m_fRadius; //moving circle radius
  
  if(m_vVel == Vector2(0.0f)) //should never happen
    return FALSE;

  Vector2 vhat = m_vVel; //direction of velocity
  vhat.Normalize(); //this is why we bailed out if m_vVel was zero

  float d; //setback distance
  if(!GetDistToTOI(C, vhat, d)) //get setback distance, bail if no collision
    return FALSE;
  
  const float t = d/m_vVel.Length(); //time back to TOI

  const Vector2 p2 = p1 - d*vhat; //position of center of moving circle at TOI
  Vector2 chat = p2 - p0; //vector from old position of center to position at TOI 
  chat.Normalize();

  const float e = m_fElasticity*C->m_fElasticity; //elasticity

  ReflectVelocity(chat, e); //adjust velocity
  EnforceSpeedLimit(m_vVel); //not too fast
  poi = p2 - r1*chat; //calculate point of impact

  p1 = p2 + t*m_vVel; //adjust position from position at TOI according to new velocity

  return TRUE;
} //Collide

/// Moving circle to moving circle collision detection and response.
/// Adjust the position and velocity after collision with a moving circle.
/// Note that the positions and velocities of both circles must change.
/// \param C [in, out] Pointer to a moving circle to collide with.
/// \param poi [out] Point of impact.
/// \return TRUE if there was a collision.

BOOL CMovingCircle::Collide(CMovingCircle* C, Vector2& poi){
  //shorthands
  Vector2& p0 = C->m_vPos; //position of C
  Vector2& p1 = m_vPos; //position of this circle

  Vector2& v0 = C->m_vVel; //velocity of C
  Vector2& v1 = m_vVel; //velocity of this circle

  const float r1 = m_fRadius;  //radius of this circle

  const Vector2 oldv0 = v0; //save C's old velocity for later setback
  Vector2 v = v1 - v0; //velocity of this circle relative to C

  if(v == Vector2(0.0f)) //should never happen
    return FALSE;

  Vector2 vhat = v; //direction of relative velocity
  vhat.Normalize(); //this is why we bailed out if v was zero
  
  float d; //setback distance for this circle to TOI, relative to C

  if(!GetDistToTOI(C, vhat, d)) //get setback distance, bail if no collision
    return FALSE;

  //compute p2, the position of the center of this circle at TOI
  Vector2 p2 = p1 - d*vhat; //position of center of this circle at TOI
  const float t = d/v.Length(); //time back to TOI

  //prepare to adjust velocities...

  Vector2 c = p2 - p0; //vector from point to position at TOI 
  c.Normalize(); //normalize it

  const Vector2 u0 = v0.Dot(c)*c; //component of v0 parallel to c
  const Vector2 u1 = v1.Dot(c)*c; //component of v1 parallel to c
  
  const float e = m_fElasticity*C->m_fElasticity; //elasticity

  const float m0 = C->m_fMass; //mass of C
  const float m1 = m_fMass; //mass of this circle
  
  //adjust velocities v0 and v1, adjusting for elasticity and mass...

  //...subtract off component parallel to chat
  v0 -= u0;
  v1 -= u1;

  //...add in component parallel to chat after impact
  v0 += e*((2.0f*m1*u1 + (m0 - m1)*u0)/(m0 + m1));
  v1 += e*((2.0f*m0*u0 + (m1 - m0)*u1)/(m0 + m1));
  
  EnforceSpeedLimit(v0); //not too fast
  EnforceSpeedLimit(v1); //not too fast

  //calculate point of impact
  poi = p2 - r1*c;

  //adjust positions of both circles...
  
  //...move both circles back to TOI; C moved t times its old velocity
  p0 -= t*oldv0; 
  p1 -= t*oldv0; 
  p2 -= t*oldv0; 
  poi -= t*oldv0;

  //...move both circles along their reflected trajectories
  p1 = p2 + t*v1; 
  p0 += t*v0; 

  return TRUE;
} //Collide
