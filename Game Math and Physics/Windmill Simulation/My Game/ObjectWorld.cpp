/// \file ObjectWorld.cpp
/// \brief Code for the object world class CObjectWorld.
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

#include "ObjectWorld.h"
#include "RenderWorld.h"

#include "debug.h"

extern CRenderWorld g_cRenderWorld;
extern b2World g_b2dPhysicsWorld; 

CObjectWorld::~CObjectWorld(){
  clear();
} //destructor

/// Create an object of a specified type, enter it into the object list, and return a pointer to it.
/// \param t Object type.
/// \return Pointer to the object created.

CObject* CObjectWorld::create(ObjectType t){ 
  CObject* p = new CObject(t);
  m_stdList.push_back(p);
  return p;
} //create

/// Delete all of the objects in the object list. This involves deleting
/// all of the CObject instances pointed to by the object list, then
/// clearing the object list itself.

void CObjectWorld::clear(){
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++) //for each object
    delete *i; //delete object

  m_stdList.clear(); //clear the object list
} //clear

/// \param s World width and height.

void CObjectWorld::SetWorldSize(const Vector2& s){
  m_vSize = s;
} //SetWorldSize

/// Get the Object World's width and height.
/// \param width Calling parameter will be set to Object World width.
/// \param height Calling parameter will be set to Object World height.

void CObjectWorld::GetWorldSize(float& width, float& height){
  width = m_vSize.x; height = m_vSize.y;
} //GetWorldSize

/// Draw the game objects using Painter's Algorithm.

void CObjectWorld::draw(){
  int depth = 0; //z depth
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){
    CObject* p = (*i);
    float a = p->m_pBody->GetAngle(); //orientation
    b2Vec2 v = p->m_pBody->GetPosition(); //position in Physics World units
    Vector3 v3 = Vector3(PW2RW(v.x), PW2RW(v.y), 1000.0f - (float)depth++);
    g_cRenderWorld.draw(p->m_nObjectType, v3, a);
  } //for
} //draw

/// Move objects.

void CObjectWorld::move(){
  g_b2dPhysicsWorld.Step(1.0f/60.0f, 6, 2);
} //move