/// \file ObjectWorld.cpp
/// \brief Code for the object world CObjectWorld.
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
#include "Timer.h"

#include "debug.h"

extern CRenderWorld g_cRenderWorld;
extern b2World g_b2dPhysicsWorld; 
extern CTimer g_cTimer; ///< The game timer.

CObjectWorld::CObjectWorld(){
  m_pHeadsUpDisplay = new CHeadsUpDisplay();
  m_pHeadsUpDisplay->m_bEasterEgg = FALSE; //easter egg
} //constructor

CObjectWorld::~CObjectWorld(){
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++)
    delete *i;
  m_stdList.clear();

  delete m_pHeadsUpDisplay;
} //destructor

/// Create an object in the Object World.
/// \param t Object type.
/// \return Pointer to the object created.

CObject* CObjectWorld::create(ObjectType t){ 
  CObject* p = new CObject(t);
  m_stdList.push_back(p);
  return p;
} //create

/// Create a HUD object in the Object World.
/// \param t Object type.
/// \return Pointer to the object created.

CObject* CObjectWorld::CreateHudObject(ObjectType t){ 
  return m_pHeadsUpDisplay->create(t);
} //create

/// \param s World width and height.

void CObjectWorld::SetWorldSize(const Vector2& s){
  m_vSize = s;
} //SetWorldSize

/// Get the Object World's width and height.
/// \param width Calling parameter will be set to Object World width.
/// \param height Calling parameter will be set to Object World height.

void CObjectWorld::GetWorldSize(float& width, float& height){
  width = m_vSize.x; height = m_vSize.y;
} //SetWorldSize

/// Determine whether player has won.
/// \param level Proportion of Object World height that the blocks must be below to win.
/// \return TRUE if player has won.

BOOL CObjectWorld::PlayerHasWon(float level){
  BOOL result = TRUE;

  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){
    CObject* p = (*i);
    if(p->m_pBody && p->m_nObjectType == CRATE_OBJECT) 
      result = result && PW2RW(p->m_pBody->GetPosition().y) < m_vSize.y/level;
  } //for

  return result;
} //PlayerHasWon

/// Clear objects.

void CObjectWorld::clear(){
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++)
    delete *i;
  m_stdList.clear();
} //clear

/// Draw the game objects, then the HUD in that order using Painter's Algorithm.

void CObjectWorld::draw(){
  int depth = 1000; //z depth
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){
    CObject* p = (*i);
    float a = p->m_pBody->GetAngle(); //orientation
    b2Vec2 v = p->m_pBody->GetPosition(); //position in Physics World units
    Vector3 v3 = Vector3(PW2RW(v.x), PW2RW(v.y), (float)depth--);
    g_cRenderWorld.draw(p->m_nObjectType, v3, a);
  } //for

  m_pHeadsUpDisplay->draw( //draw HUD on top of that
    g_cTimer.GetLevelElapsedTime()/1000, //time
    m_cCannon.m_fTemp, m_cCannon.m_fMaxTemp); //temperature
} //draw

/// Move objects.

void CObjectWorld::move(){
  g_b2dPhysicsWorld.Step(1.0f/60.0f, 6, 2);
} //move

/// Make sounds for all objects in the Object World.

void CObjectWorld::MakeSound(){ 
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++)
    (*i)->MakeSound();
} //MakeSound