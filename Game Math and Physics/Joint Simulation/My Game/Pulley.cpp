/// \file Pulley.cpp
///
/// \brief Code for the pulley class CPulley.
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

#include "Pulley.h"

#include "GameDefines.h"
#include "ObjectWorld.h"
#include "RenderWorld.h"
#include "debug.h"

extern b2World g_b2dPhysicsWorld; 
extern CObjectWorld g_cObjectWorld;
extern CRenderWorld g_cRenderWorld;

/// \brief Set the orientation of a b2Body.
/// Set the angle of a b2Body using the SetTransform function,
/// because b2Body apparently no longer has a SetAngle function.
/// \param p Pointer to a b2Body.
/// \param a An angle in radians.

void b2SetAngle(b2Body* p, const float a){
  p->SetTransform(p->GetPosition(), a);
} //b2SetAngle

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \param w Pulley wheel horizontal separation in Physics World units.

CPulley::CPulley(float x, float y, float w){
  //crate calculations

  const int nCrateWidth = g_cRenderWorld.GetWidth(CRATE_OBJECT); //crate width in Render World
  const int nCrateHt = g_cRenderWorld.GetHeight(CRATE_OBJECT); //crate height in Render World
  
  const float fCrateWidth2 = RW2PW(nCrateWidth)/2.0f; //crate half width in Physics World
  const float fCrateHt2 = RW2PW(nCrateHt)/2.0f; //crate half height in Physics World
  

  //safe calculations

  const int nSafeWidth = g_cRenderWorld.GetWidth(SAFE_OBJECT); //safe width in Render World
  const int nSafeHt = g_cRenderWorld.GetHeight(SAFE_OBJECT); //safe height in Render World
  const float fSafeWidth2 = RW2PW(nSafeWidth)/2.0f; //safe half width in Physics World
  const float fSafeHt2 = RW2PW(nSafeHt)/2.0f; //safe half height in Physics World

  //pulley wheel calculations

  const int nWheelDiam = g_cRenderWorld.GetWidth(PULLEY_OBJECT); //pulley wheel diameter in Render World
  m_fWheelRad = RW2PW(nWheelDiam)/2.0f - RW2PW(4); //pulley wheel radius in Physics World
  const int nWheel2Diam = g_cRenderWorld.GetHeight(PULLEY_OBJECT_2);	// Pulley 2 diameter in render world
  m_fWheel2Rad = RW2PW(nWheel2Diam)/2.0f - 0.5f;	// Pulley 2 half height in Physics World.
  const float fWheelSep2 = w/2.0f; //half pulley wheel separation in Physics World
  const float fWheelAlt = 2.0f*(y - 1.2f*m_fWheelRad); //wheel altitude on screen

   //calculate positions
  const float vWheelPos1X = x + fWheelSep2 + m_fWheel2Rad + 0.5f;
  const float vWheelPos1Y = fWheelAlt + m_fWheel2Rad + 0.5f;
  const float vWheelPos2X = x + fWheelSep2 - m_fWheel2Rad - 0.5f;
  const float vWheelPos2Y = fWheelAlt - m_fWheel2Rad - 2.5f;
  const float vWheelPos3X = x + m_fWheel2Rad*3;
  const float vWheelPos3Y = fWheelAlt - m_fWheel2Rad - 2.5f;
  const float vWheelPos4X = x + m_fWheel2Rad;
  const float vWheelPos4Y = fWheelAlt + m_fWheel2Rad + 0.5f;

  const b2Vec2 vCratePos = b2Vec2(x - fWheelSep2 - m_fWheelRad, fWheelAlt - 2.0f*m_fWheelRad - fCrateHt2);
  const b2Vec2 vSafePos = b2Vec2(x + fWheelSep2 + m_fWheelRad, fSafeHt2);
  const b2Vec2 vWheelPos0 = b2Vec2(x - fWheelSep2, fWheelAlt);
  const b2Vec2 vWheelPos1 = b2Vec2(vWheelPos1X, vWheelPos1Y);
  const b2Vec2 vWheelPos2 = b2Vec2(vWheelPos2X, vWheelPos2Y);
  const b2Vec2 vWheelPos3 = b2Vec2(vWheelPos3X, vWheelPos3Y);
  const b2Vec2 vWheelPos4 = b2Vec2(vWheelPos4X, vWheelPos4Y);

  //create bodies

  b2Body* pCrate = CreateCrate(vCratePos.x, vCratePos.y);
  b2Body* pSafe = CreateObject(vSafePos.x, vSafePos.y, 0.5f, SAFE_OBJECT);
  m_pWheel0 = CreateWheel(vWheelPos0.x, vWheelPos0.y, PULLEY_OBJECT);
  m_pWheel1 = CreateWheel(vWheelPos1.x, vWheelPos1.y, PULLEY_OBJECT_2);
  m_pWheel2 = CreateWheel(vWheelPos2.x, vWheelPos2.y, PULLEY_OBJECT_2);
  m_pWheel3 = CreateWheel(vWheelPos3.x, vWheelPos3.y, PULLEY_OBJECT_2);
  m_pWheel4 = CreateWheel(vWheelPos4.x, vWheelPos4.y, PULLEY_OBJECT_2);

  //calculate anchor points

  const b2Vec2 vCrateAnchor = vCratePos + b2Vec2(0.0f, fCrateHt2);
  const b2Vec2 vSafeAnchor = vSafePos + b2Vec2(0.0f, fSafeHt2);
  const b2Vec2 vWheelAnchor0 = vWheelPos0 - b2Vec2(m_fWheelRad, 0.0f);
  const b2Vec2 vWheelAnchor1 = vWheelPos1 + b2Vec2(m_fWheel2Rad, 0.0f);
  const b2Vec2 vWheelAnchor2 = vWheelPos2 + b2Vec2(m_fWheel2Rad, 0.0f);
  const b2Vec2 vWheelAnchor3 = vWheelPos3 + b2Vec2(m_fWheel2Rad, 0.0f);
  const b2Vec2 vWheelAnchor4 = vWheelPos4 + b2Vec2(m_fWheel2Rad, 0.0f);

  //create pulley joint

  b2PulleyJointDef jd;
  jd.Initialize(pCrate, pSafe, //bodies
    vWheelAnchor0, vWheelAnchor1, //anchors on wheels
    vCrateAnchor, vSafeAnchor, //anchors on bodies
    1.0f); 

  m_pJoint = (b2PulleyJoint*)g_b2dPhysicsWorld.CreateJoint(&jd);
  m_fJointLenA = m_pJoint->GetCurrentLengthA();

  //create lines to represent the rope

  //rope from big left wheel to crate
  b2Vec2 offset0 = b2Vec2(-m_fWheelRad, 0.0f);
  b2Vec2 offset1 = b2Vec2(0.0f, fCrateHt2 - RW2PW(4));
  g_cObjectWorld.CreateLine(m_pWheel0, offset0, FALSE, pCrate, offset1, TRUE);

  //rope from right wheel to safe
  offset0 = b2Vec2(m_fWheel2Rad, 0.0f);
  offset1 = b2Vec2(0.0f, 0.0f);
  g_cObjectWorld.CreateLine(m_pWheel1, offset0, FALSE, pSafe, offset1, TRUE);

  //rope from the left most pulley wheel across to the small left most pulley wheel
  offset0 = b2Vec2(0.0f, m_fWheelRad);
  offset1 = b2Vec2(0.0f, m_fWheel2Rad);
  g_cObjectWorld.CreateLine(m_pWheel0, offset0, FALSE, m_pWheel4, offset1, FALSE);

  //rope from the small left most pulley wheel across to the middle pulley wheel
  offset0 = b2Vec2(m_fWheel2Rad, 0.0f);
  offset1 = b2Vec2(-m_fWheel2Rad, 0.0f);
  g_cObjectWorld.CreateLine(m_pWheel4, offset0, FALSE, m_pWheel3, offset1, FALSE);

  //rope from the middle pulley wheel across to the small second to right pulley wheel
  offset0 = b2Vec2(0.0f, -m_fWheel2Rad);
  offset1 = b2Vec2(0.0f, -m_fWheel2Rad);
  g_cObjectWorld.CreateLine(m_pWheel3, offset0, FALSE, m_pWheel2, offset1, FALSE);

  //rope from the small second to right pulley wheel across to the small right most most pulley wheel
  offset0 = b2Vec2(m_fWheel2Rad, 0.0f);
  offset1 = b2Vec2(-m_fWheel2Rad, 0.0f);
  g_cObjectWorld.CreateLine(m_pWheel2, offset0, FALSE, m_pWheel1, offset1, FALSE);

  //the elephant needs to be the last thing created
  m_vSpawnPt = b2Vec2(vCratePos.x, 3.0f*y);
  CreateObject(m_vSpawnPt.x, m_vSpawnPt.y, 0.3f, ELEPHANT_OBJECT);
  m_bElephant = TRUE;
} //constructor

/// The pulley wheels are going to be static objects that we
/// rotate ourselves depending on the positions of the objects
/// that the pulley is attached to.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body.

b2Body* CPulley::CreateWheel(float x, float y, ObjectType type){ 
  CObject* pObj = g_cObjectWorld.create(type); //Object World

  //Physics World
  b2BodyDef bd;
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //body
  b2Body* p = g_b2dPhysicsWorld.CreateBody(&bd);
  pObj->SetPhysicsBody(p); //tell object world body about physics world body

  return p;
} //CreateWheel

/// The bucket is the thing on the left that the elephant falls into.
/// It consists of a left side, a right side, and a bottom, all
/// of which are rectangles.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body.

b2Body* CPulley::CreateCrate(float x, float y){  
  CObject* pObj = g_cObjectWorld.create(CRATE_OBJECT); //Object World

  //Physics World
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  b2Body* p = g_b2dPhysicsWorld.CreateBody(&bd);
  pObj->SetPhysicsBody(p); //tell object world body about physics world body

  float cw = RW2PW(g_cRenderWorld.GetWidth(CRATE_OBJECT))/2.0f; //crate half width 
  float ch = RW2PW(g_cRenderWorld.GetHeight(CRATE_OBJECT))/2.0f; //crate half height 
  const float sh = RW2PW(17)/2.0f; //half height of crate floor

  b2PolygonShape s; //shape for crate floor fixture
  s.SetAsBox(cw, sh, b2Vec2(0.0f, sh - ch), 0.0f); //align with bottom of sprite

  b2FixtureDef fd; 
  fd.shape = &s;
  fd.density = 1.0f;
  fd.restitution = 0.0f;

  p->CreateFixture(&fd);

  return p;
} //CreateCrate

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \param d Object density.
/// \param t Type of object to be created.
/// \return Pointer to physics body.

b2Body* CPulley::CreateObject(float x, float y, float d, ObjectType t){  
  CObject* pObj = g_cObjectWorld.create(t); //Object World

  int w, h; //width and height of object in Render World
  g_cRenderWorld.GetSize(w, h, t);

  //Physics World
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  //shape
  b2PolygonShape shape;
  shape.SetAsBox(RW2PW(w)/2.0f, RW2PW(h)/2.0f);

  //fixture
  b2FixtureDef fd;
  fd.shape = &shape;
  fd.density = d;
  fd.restitution = 0.0f;

  //body
  b2Body* p = g_b2dPhysicsWorld.CreateBody(&bd);
  pObj->SetPhysicsBody(p); //tell object world body about physics world body
  p->CreateFixture(&fd);
  p->SetAngularDamping(4.0f);
  p->SetLinearDamping(0.9f);

  return p;
} //CreateObject

void CPulley::move(){ 
  const float fLenA = m_pJoint->GetCurrentLengthA(); //current length of left rope
  const float theta = (fLenA - m_fJointLenA)/m_fWheelRad; //new wheel orientation
  const float theta2 = (fLenA - m_fJointLenA) / m_fWheel2Rad; //new wheel orientation

  b2SetAngle(m_pWheel0, theta); //left wheel
  b2SetAngle(m_pWheel1, theta2 + 2.4f); //right wheel is offset so it is oriented differently
  b2SetAngle(m_pWheel2, -(theta2 + 2.4f)); //right wheel is offset so it is oriented differently
  b2SetAngle(m_pWheel3, theta2 + 2.4f); //right wheel is offset so it is oriented differently
  b2SetAngle(m_pWheel4, theta2 + 2.4f); //right wheel is offset so it is oriented differently
} //move

/// Make the elephant disappear or reappear. Uses m_bElephant
/// to check for elephant existence.

void CPulley::ToggleElephant(){
  if(m_bElephant) //if it exists
    g_cObjectWorld.DeleteLastObject(ELEPHANT_OBJECT); //then delete it
  else //if it doesn't exist
    CreateObject(m_vSpawnPt.x, m_vSpawnPt.y, 0.3f, ELEPHANT_OBJECT); //create it

  m_bElephant = !m_bElephant; //toggle
} //ToggleElephant
