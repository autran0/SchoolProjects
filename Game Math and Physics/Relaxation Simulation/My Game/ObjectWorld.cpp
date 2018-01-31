/// \file ObjectWorld.cpp
/// \brief Code for the Object World class CObjectWorld.
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

#include "Timer.h"
#include "RenderWorld.h"
#include "Body.h"

#include "debug.h"

extern CRenderWorld g_cRenderWorld;
extern CTimer g_cTimer; 

extern BodyType g_nCurrentBody; 
extern float g_fSizeMult;

CObjectWorld::CObjectWorld(){
  m_pCurrentBody = nullptr;
  m_pCurrentBody2 = nullptr;
} //constructor

CObjectWorld::~CObjectWorld(){
  clear();
} //destructor

/// \param s World width and height.

void CObjectWorld::SetWorldSize(const Vector2& s){
  m_vSize = s;
} //SetWorldSize

/// Clear objects.

void CObjectWorld::clear(){
  for(auto i=m_stdPts.begin(); i!=m_stdPts.end(); i++)
    delete *i;

  for(auto i=m_stdSpr.begin(); i!=m_stdSpr.end(); i++)
    delete *i;

  m_stdPts.clear();
  m_stdSpr.clear();

  m_pCurrentBody = nullptr;
  m_pCurrentBody2 = nullptr;
} //clear

/// Ask the Render World to draw all of the game particles.

void CObjectWorld::draw(){ 
  int depth = 0; //z depth
  for(auto i=m_stdPts.begin(); i!=m_stdPts.end(); i++){
    CParticle* p = *i; //shorthand
    Vector3 v = Vector3(p->m_vPos.x, p->m_vPos.y, (float)-depth++);
	if(p->m_nSpriteType == 3 || p->m_nSpriteType == 4)	// Does not mess with centers since those are already fine
		g_cRenderWorld.draw(p->m_nSpriteType, v,
			p->m_fAngle, p->m_fXScale, p->m_fYScale*g_fSizeMult);
	else
		g_cRenderWorld.draw(p->m_nSpriteType, v, 
		  p->m_fAngle, p->m_fXScale*g_fSizeMult, p->m_fYScale*g_fSizeMult);
  } //for
} //draw

/// Move the particles, perform Gauss-Seidel relaxation on the springs, and recompute
/// the spring centers based on the position of the particles that they are connected to.

void CObjectWorld::move(){
  //move particles
  for(auto i=m_stdPts.begin(); i!=m_stdPts.end(); i++) 
    (*i)->move(m_vSize); 

  Relax(2); //Gauss-Seidel relaxation

  //compute new spring centers from particle locations
  for(auto i=m_stdSpr.begin(); i!=m_stdSpr.end(); i++)  
    (*i)->ComputeCenter(); 

  if(m_pCurrentBody)m_pCurrentBody->move();
  if(m_pCurrentBody2)m_pCurrentBody2->move();
} //move

/// Deliver an impulse to the two current bodies.

void CObjectWorld::DeliverImpulse(){
  if(m_pCurrentBody)m_pCurrentBody->DeliverImpulse(20.0f*g_fSizeMult); 
  if(m_pCurrentBody2)m_pCurrentBody2->DeliverImpulse(20.0f*g_fSizeMult);
} //DeliverImpulse

/// Create a body.
/// \param b Type of body to be created.

void CObjectWorld::CreateBody(BodyType b){
  Vector2 v = m_vSize/2.0f; //screen center

  if(m_pCurrentBody)delete m_pCurrentBody;
  m_pCurrentBody = new CBody();

  if(m_pCurrentBody2)delete m_pCurrentBody2;
  m_pCurrentBody2 = new CBody();

  switch(g_nCurrentBody){
    case CHAIN2_BODY:
       m_pCurrentBody->MakeChain(2, (int)(75 * g_fSizeMult), 0.5f, XM_PI/6.0f, v);
       m_pCurrentBody2->MakeChain(2, (int)(75 * g_fSizeMult), 0.01f, XM_PI/6.0f, v);
      break;

    case CHAIN3_BODY:
       m_pCurrentBody->MakeChain(3, (int)(75 * g_fSizeMult), 0.5f, XM_PI/6.0f, v);
       m_pCurrentBody2->MakeChain(3, (int)(75 * g_fSizeMult), 0.01f, XM_PI/6.0f, v);
      break;

    case CHAIN4_BODY:
       m_pCurrentBody->MakeChain(4, (int)(75 * g_fSizeMult), 0.5f, XM_PI/6.0f, v);
       m_pCurrentBody2->MakeChain(4, (int)(75 * g_fSizeMult), 0.01f, XM_PI/6.0f, v);
      break;

    case TRIANGLE_BODY:
      m_pCurrentBody->MakeTriangle((int)(75 * g_fSizeMult), 0.5f, v);
      m_pCurrentBody2->MakeTriangle((int)(75 * g_fSizeMult), 0.02f, v);
      break;

    case SQUARE_BODY:
      m_pCurrentBody->MakeSquare((int)(75 * g_fSizeMult), 0.5f, v);
      m_pCurrentBody2->MakeSquare((int)(75 * g_fSizeMult), 0.02f, v);
      break;

    case WHEEL5_BODY:
      m_pCurrentBody->MakeWheel(5, (int)(120 * g_fSizeMult), 0.5f, v);
      m_pCurrentBody2->MakeWheel(5, (int)(120 * g_fSizeMult), 0.1f, v);
      break;

    case WHEEL6_BODY:
      m_pCurrentBody->MakeWheel(6, (int)(120 * g_fSizeMult), 0.5f, v);
      m_pCurrentBody2->MakeWheel(6, (int)(120 * g_fSizeMult), 0.1f, v);
      break;

    case RAGDOLL_BODY:
      m_pCurrentBody->MakeRagdoll(v);  
      m_pCurrentBody2 = nullptr;
      break;
  } //switch

  //make room if there are 2 bodies
  if(m_pCurrentBody2){
    m_pCurrentBody->Teleport(200.0f, 50.0f);
    m_pCurrentBody2->Teleport(-200.0f, -50.0f);
  } //if
} //CreateBody

CParticle* CObjectWorld::create(SpriteType sprite, Vector2 position){
  CParticle* p = new CParticle;
  p->m_nSpriteType = sprite;
  p->m_vPos = p->m_vOldPos = position;
  m_stdPts.push_back(p);
  return p;
} //create

/// \param v0 Particle at one end of spring.
/// \param v1 Particle at other end of spring.
/// \param c Particle at center of spring.
/// \param r Restitution.

CSpring* CObjectWorld::create(CParticle* v0, CParticle* v1, CParticle* c, float r){
  CSpring* p = new CSpring; //new spring
  m_stdSpr.push_back(p); //insert into spring list
  p->m_pV0 = v0; //set one end
  p->m_pV1 = v1; //set the other end
  p->m_pCenter = c; //set center

  //current length is rest length, so compute it and store
  Vector2 vDelta = v0->m_vPos - v1->m_vPos;
  p->m_fRestLength = vDelta.Length();

  p->m_fRestitution = r; //set restitution
  return p;
} //create

  /// Perform Gauss-Seidel relaxation on a collection of springs. The more
  /// iterations, the more stick-like the springs will be.
  /// \param iterations Number of iterations of relaxation to perform.

void CObjectWorld::Relax(int iterations){ 
  for(int i=0; i<iterations; i++) //more iterations means more like a stick
    for(auto j=m_stdSpr.begin(); j!=m_stdSpr.end(); j++) //for each spring 
      (*j)->Relax();
} //Relax