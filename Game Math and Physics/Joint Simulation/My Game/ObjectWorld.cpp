/// \file ObjectWorld.cpp
/// \brief Code for the object world class CObjectWorld.
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

#include "ObjectWorld.h"
#include "RenderWorld.h"
#include "LineObject.h"
#include "Timer.h"
#include "debug.h"

extern CRenderWorld g_cRenderWorld;
extern b2World g_b2dPhysicsWorld; 
extern CTimer g_cTimer;

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

/// Create a line object in Object World.
/// \param b0 Pointer to first physics body. (The base.)
/// \param d0 Vector offset to anchor point on b0.
/// \param r0 Radius of b0.
/// \param b1 Pointer to second physics body. (The dangly bit.)
/// \param d1 Vector offset to anchor point on b1.
/// \param r1 Radius of b1.
/// \return Pointer to line object.

CObject* CObjectWorld::CreateLine(b2Body* b0, const b2Vec2& d0, BOOL r0, b2Body* b1, const b2Vec2& d1, BOOL r1){
  CObject* p =  new CLineObject(b0, d0, r0, b1, d1, r1);
  m_stdList.push_back(p);
  return p;
} //CreateLine

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

  // Make two passes to ensure that the lines are drawn behind the other objects
  // If I had lots of lines and objects, then I would make two lists. However,
  // these joint demos are small enough that it doesn't matter.

  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++)
    if((*i)->m_nObjectType == LINE_OBJECT){ //line objects are special
      const CLineObject* p = (CLineObject*)*i; //pointer to line object to be drawn

      //line goes from anchor 0 on body 0 to anchor 1 on body 1 in Physics World
      const b2Body* b0 = p->m_pBody0; //body 0
      const b2Body* b1 = p->m_pBody1; //body 1

      b2Vec2 da0 = p->m_vAnchor0;
      if(p->m_bRotates0)
        da0 = b2Mul(b2Rot(b0->GetAngle()), da0);

      b2Vec2 da1 = p->m_vAnchor1;
      if(p->m_bRotates1)
        da1 = b2Mul(b2Rot(b1->GetAngle()), da1);

      const b2Vec2 a0 = b0->GetPosition() + da0; //anchor 0 position in Physics World
      const b2Vec2 a1 = b1->GetPosition() + da1; //anchor 1 position in Physics World

      const Vector3 a0rw = Vector3(PW2RW(a0.x), PW2RW(a0.y), 2500.0f - (float)depth++); //anchor 0 position in Render World
      const Vector3 a1rw = Vector3(PW2RW(a1.x), PW2RW(a1.y), 2500.0f - (float)depth++); //anchor 1 position in Render World

      const Vector3 vrw = a1rw - a0rw; //vector from anchor 0 to anchor 1 in Render World

      const float a = atan2(vrw.y, vrw.x); //orientation of line
      const int w = g_cRenderWorld.GetWidth(LINE_OBJECT); //length of line sprite
      const float xscale = vrw.Length()/w; //length of line in Render World, scaled back by length of line sprite
      const Vector3 crw = (a0rw + a1rw)/2.0f; //position of center of line in Render World

      g_cRenderWorld.draw(LINE_OBJECT, crw, a, xscale); //draw the line in Render World, stretching it to the right length
    } //if

  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++)
    if((*i)->m_nObjectType != LINE_OBJECT){ //ordinary objects
      const CObject* p = *i; //pointer to object to be drawn
      const float a = p->m_pBody->GetAngle(); //orientation of object
      const b2Vec2 ppw = p->m_pBody->GetPosition(); //position in Physics World 
      const Vector3 prw = Vector3(PW2RW(ppw.x), PW2RW(ppw.y),  1000.0f - (float)depth++); //position in Render World 

      g_cRenderWorld.draw(p->m_nObjectType, prw, a); //draw the object in Render World
    } //else
} //draw

/// Move objects.

void CObjectWorld::move(){
  g_b2dPhysicsWorld.Step(1.0f/60.0f, 6, 2);
} //move

/// If the object list is nonempty and the last object in it is a given type,
/// then remove the last object from the object list and destroy it.
/// \param t Desired type of last object.

void CObjectWorld::DeleteLastObject(ObjectType t){
  CObject* p = m_stdList.back();
  if(p != nullptr && p->m_nObjectType == t){
    m_stdList.pop_back();
    delete p;
  } //if
} //DeleteLastObject