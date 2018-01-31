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
#include "Sndlist.h"

#include "debug.h"

extern CRenderWorld g_cRenderWorld;
extern CTimer g_cTimer; 
extern CSoundManager* g_pSoundManager;

extern int g_nShowTrack;
extern BOOL g_bShowHitPt;
extern BOOL g_bShowImpacts;
extern BOOL g_bShowArrows;

extern int g_nMIterations;
extern int g_nCIterations;

CObjectWorld::CObjectWorld(){  
  m_fLHPAngle = 0.0f;
  m_nNumReticles = m_nFirstReticle = m_nNextReticle = 0;
  m_nCollisionCnt = m_nCollisionTestCnt = m_nAABBTestCnt = 0;
  m_cAABB = CAabb2D(0.0f, 0.0f, 0.0f, 0.0f);
} //constructor

CObjectWorld::~CObjectWorld(){
 clear();
} //destructor

/// Make all of the static shapes.

void CObjectWorld::MakeShapes(){ 
  const float MARGIN = 32.0f;

  //outside edges
  m_stdShape.push_back(new CLine(Vector2(1024.0f - MARGIN, 768.0f - MARGIN), Vector2(1.0f, 0.0f), 0.9f));
  m_stdShape.push_back(new CLine(Vector2(1024.0f - MARGIN, 768.0f - MARGIN), Vector2(0.0f, 1.0f), 0.9f));
  m_stdShape.push_back(new CLine(Vector2(MARGIN, MARGIN), Vector2(1.0f, 0.0f), 0.9f));
  m_stdShape.push_back(new CLine(Vector2(MARGIN, MARGIN), Vector2(0.0f, 1.0f), 0.9f));

  //diagonal lines
  m_stdShape.push_back(new CLine(Vector2(0.0f, 2.5f*768.0f/4.0f), Vector2(1.0f, 1.0f), 0.9f));
  m_stdShape.push_back(new CLine(Vector2(700.0f, 1024.0f), Vector2(0.77f, -1.0f), 0.9f));

  //line segments
  m_stdShape.push_back(new CLineSeg(Vector2(3.0f*1024.0f/4.0f, 3.0f*768.0f/4.0f), Vector2(1024.0f/2.0f, 768.0f/2.0f), 1.5f));
  m_stdShape.push_back(new CLineSeg(Vector2(1024.0f/4.0f, 3.0f*768.0f/4.0f), Vector2(1024.0f/4.0f, 768.0f/2.0f), 1.5f));
  m_stdShape.push_back(new CLineSeg(Vector2(3.0f*1024.0f/8.0f, 3.0f*768.0f/4.0f), Vector2(1024.0f/2.0f, 3.0f*768.0f/4.0f), 1.5f));
  
  //line segments to protect new ball
  m_stdPoint.push_back(new CPoint(Vector2(920.0f, 0.0f)));
  m_stdPoint.push_back(new CPoint(Vector2(920.0f, 610.0f)));
  m_stdPoint.push_back(new CPoint(Vector2(920.0f, 768.0f)));
  m_stdPoint.push_back(new CPoint(Vector2(1024.0f, 610.0f)));
  
  m_stdShape.push_back(new CLineSeg(m_stdPoint[0], m_stdPoint[1])); //vertical line segment 
  m_stdShape.push_back(new CLineSeg(m_stdPoint[1], m_stdPoint[2], Vector2(-1.0f, 0.0f))); //vertical one-way line segment 
  m_stdShape.push_back(new CLineSeg(m_stdPoint[1], m_stdPoint[3], Vector2(0.0f, 1.0f))); //horizontal one-way line segment 

  // Triangle
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 4.0f) - 50, (768.0f / 4.0f) - 30), Vector2((1024.0f / 4.0f) + 50, (768.0f / 4.0f) - 30), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 4.0f) + 53, (768.0f / 4.0f) - 30), Vector2((1024.0f / 4.0f), (768.0f / 4.0f) + 60), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 4.0f) - 53, (768.0f / 4.0f) - 30), Vector2((1024.0f / 4.0f), (768.0f / 4.0f) + 60), 0.9f));

  // Rectangle
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 2.0f) - 40, (768.0f / 4.0f) - 42), Vector2((1024.0f / 2.0f) + 40, (768.0f / 4.0f) - 42), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 2.0f) + 40, (768.0f / 4.0f) - 42), Vector2((1024.0f / 2.0f) + 40, (768.0f / 4.0f) + 42), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 2.0f) + 40, (768.0f / 4.0f) + 42), Vector2((1024.0f / 2.0f) - 40, (768.0f / 4.0f) + 42), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((1024.0f / 2.0f) - 40, (768.0f / 4.0f) + 42), Vector2((1024.0f / 2.0f) - 40, (768.0f / 4.0f) - 42), 0.9f));

  // Pentagon
  m_stdShape.push_back(new CLineSeg(Vector2((3.0f*1024.0f / 4.0f) - 33, (768.0f / 4.0f) - 50), Vector2((3.0f*1024.0f / 4.0f) + 33, (768.0f / 4.0f) - 50), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((3.0f*1024.0f / 4.0f) + 33, (768.0f / 4.0f) - 50), Vector2((3.0f*1024.0f / 4.0f) + 57, (768.0f / 4.0f) + 17), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((3.0f*1024.0f / 4.0f) + 57, (768.0f / 4.0f) + 17), Vector2((3.0f*1024.0f / 4.0f), (768.0f / 4.0f) + 60), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((3.0f*1024.0f / 4.0f), (768.0f / 4.0f) + 60), Vector2((3.0f*1024.0f / 4.0f) - 57, (768.0f / 4.0f) + 17), 0.9f));
  m_stdShape.push_back(new CLineSeg(Vector2((3.0f*1024.0f / 4.0f) - 57, (768.0f / 4.0f) + 17), Vector2((3.0f*1024.0f / 4.0f) - 33, (768.0f / 4.0f) - 50), 0.9f));

  //circle
  m_stdShape.push_back(new CCircle(Vector2(3.0f*1024.0f/4.0f, 768.0f/2.0f), 40.5f, 20.0f));
  
  //AABB around main play area
  m_cAABB = CAabb2D(MARGIN, 1024.0f - MARGIN, MARGIN, 768.0f - MARGIN);
} //constructor

/// Create an object in the Object World.
/// \param t Object type.
/// \param c Object type for breadcrumb.
/// \param s Initial position.
/// \param v Initial velocity.
/// \param r Radius.
/// \param e Elasticity.
/// \param m Magnification factor.
/// \return Pointer to the object created.

void CObjectWorld::create(ObjectType t, ObjectType c, const Vector2& s, const Vector2& v, float r, float e, float m){
  CObject* b = new CObject(t, c, s, v, r, e, m); //conjure an object
  m_stdList.push_back(b); //place in object list
} //create

/// Delete all of the objects in the object list. This involves deleting
/// all of the CObject instances pointed to by the object list, then
/// clearing the object list itself.

void CObjectWorld::clear(){
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++) //for each object
    delete *i; //delete object

  for(auto i=m_stdShape.begin(); i!=m_stdShape.end(); i++) //for each shape
    delete *i; //delete shape
  
  for(auto i=m_stdPoint.begin(); i!=m_stdPoint.end(); i++) //for each point
    delete *i; //delete point

  m_stdList.clear(); //clear the object list
  m_stdShape.clear(); //clear the shape list
  m_stdPoint.clear(); //clear the point list
  
  m_nNumReticles = m_nFirstReticle = m_nNextReticle = 0; //clear reticles
} //clear

/// Set Object World's width and height.
/// \param s World width and height.

void CObjectWorld::SetWorldSize(const Vector2& s){
  m_vSize = s;
  m_cAABB = CAabb2D(s.x/2.0f, s.x/2.0f, s.y/2.0f, s.y/2.0f);
} //SetWorldSize

/// Get the Object World's width and height.
/// \param w Calling parameter will be set to Object World width.
/// \param h Calling parameter will be set to Object World height.

void CObjectWorld::GetWorldSize(float& w, float& h){
  w = m_vSize.x; h = m_vSize.y;
} //GetWorldSize

/// Draw all of the objects in the object list.

void CObjectWorld::draw(){
  const float STARTDEPTH = 100000.0f;
  int depth = 0; //z depth

  //draw breadcrumb trails
  if(g_nShowTrack >= 1) 
    for(int j=0; j<NUMCRUMBS; j++)
      for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){
        const CObject* p = *i; //handy object pointer
        int ix = (p->m_nCurCrumb + j)%NUMCRUMBS; //breadcrumb index
        const Vector2 u = p->m_vCrumbPos[ix];
        if(u != Vector2(0.0f) && u != p->m_cShape.m_vPos){ //if in use
          const Vector3 v = Vector3(u.x, u.y, STARTDEPTH - 0.1f*depth++); //fake z value
          ObjectType ct = p->m_eCrumbType;
          float scale = p->m_cShape.m_fScale;
          if(g_nShowTrack == 2){ //draw dots
            ct = (ObjectType)(ct - GRAYCIRCLE_OBJECT + GRAYDOT_OBJECT); //map to correct color dot
            scale = 0.35f; //default scale for dots
          } //if
          g_cRenderWorld.draw(ct, v, 0.0f, scale, scale); //draw crumb in Render World
        } //if
      } //for

  //draw objects
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){ //for each object
    const CObject* p = *i; //handy object pointer
    const Vector2 u = p->m_cShape.m_vPos; //object position 
    Vector3 v = Vector3(u.x, u.y, STARTDEPTH - 0.1f*depth++); //fake z value

    if(g_bShowArrows && p->m_cShape.m_vVel != Vector2(0.0f)){
      const float theta = atan2(p->m_cShape.m_vVel.y, p->m_cShape.m_vVel.x);
      const float len = 0.2f + 2.0f*p->m_cShape.m_fRadius/g_cRenderWorld.GetWidth(ARROW_OBJECT) + p->m_cShape.m_vVel.Length()/25.0f;
      g_cRenderWorld.draw(ARROW_OBJECT, v, theta, len, 1.0f);  //draw arrow in Render World
    } //if

    v.z = STARTDEPTH - 0.1f*depth++;
    g_cRenderWorld.draw(p->m_nObjectType, v, 0.0f, p->m_cShape.m_fScale, p->m_cShape.m_fScale); //draw object in Render World
  } //for
  
  //draw reticles
  if(g_bShowHitPt)
    for(int i=0; i<m_nNumReticles; i++){
      const int ix = (m_nFirstReticle + i)%NUMRETICLES;
      const Vector2 p = m_cReticle[ix].m_vPos;
      const Vector3 v = Vector3(p.x, p.y, STARTDEPTH - 0.1f*depth++); //fake z value
      g_cRenderWorld.draw(m_cReticle[ix].m_eObject, v, m_fLHPAngle);
    } //for

  //draw dots at impact points
  if(g_bShowImpacts)
    for(auto i=m_stdDot.begin(); i!=m_stdDot.end(); i++){
      const Vector3 v = Vector3((*i).m_vPos.x, (*i).m_vPos.y, STARTDEPTH - 0.1f*depth++); //fake z value
      g_cRenderWorld.draw((*i).m_eObject, v, 0.0f, 0.7f, 0.7f);
    } //for
} //draw

/// Move all of the objects in the object list and perform collision response.

void CObjectWorld::move(){
  const float MAXSPEED = 15.0f;
  const float MAXSPEEDSQ = MAXSPEED*MAXSPEED;

  const float dt = 1000.0f/60.0f; //(float)g_cTimer.frametime();
  
  for(int j=0; j<g_nMIterations; j++){
    for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++) //for each object
      (*i)->move(dt/(float)g_nMIterations); //move it

    for(int i=0; i<g_nCIterations; i++)
      BroadPhase();
    
    //enforce upper speed limit
    for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++) //for each object
      if((*i)->m_cShape.m_vVel.LengthSquared() > MAXSPEEDSQ){
        (*i)->m_cShape.m_vVel.Normalize();
        (*i)->m_cShape.m_vVel *= MAXSPEED;
      } //if
  } //for
  
  //breadcrumb trails
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){ //for each object
    CObject* p = *i;
    p->m_vCrumbPos[p->m_nCurCrumb] = p->m_cShape.m_vPos;
    p->m_nCurCrumb = (p->m_nCurCrumb + 1)%NUMCRUMBS;
  } //for

  //remove old reticles
  const int RLIFE = 2000; //reticle lifetime in milliseconds
  while(m_nNumReticles > 0 && (g_cTimer.time() - m_cReticle[m_nFirstReticle].m_nBirthTime) > RLIFE){
    m_nFirstReticle = (m_nFirstReticle + 1)%NUMRETICLES;
    --m_nNumReticles;
  } //while

  m_fLHPAngle += dt/300.0f;
} //move

/// Iterate through object list and do narrow phase collision detection
/// and response for each pair of objects, taking care to do each pair no
/// more than once.

void CObjectWorld::BroadPhase(){
  int ix = 0; //index
  Vector2 poi; //point of intersection
  BOOL bHit = FALSE; //whether a collision occurred
  
  CAabb2D screen = CAabb2D(0.0f, 1024.0f, 0.0f, 768.0f);

  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){ //for each object
    CObject* p = *i; //current object
    
    CAabb2D obj = p->m_cShape.m_cAABB;

    for(auto j=m_stdShape.begin(); j!=m_stdShape.end(); j++){ //for each shape
      CShape* s = *j;

      switch(s->m_eShape){
        case CShape::LINE_SHAPE:    
          bHit = p->m_cShape.Collide((CLine*)s, poi);

          if(bHit)
            g_pSoundManager->play(BUMP_SOUND);
          break;

        case CShape::LINESEG_SHAPE:
          bHit = p->m_cShape.Collide((CLineSeg*)s, poi);

          if(bHit)
            g_pSoundManager->play(BOOP_SOUND);
          break;

        case CShape::POINT_SHAPE:
          bHit = p->m_cShape.Collide((CPoint*)s, poi);

          if(bHit)
            g_pSoundManager->play(BEEP_SOUND);
          break;

        case CShape::CIRCLE_SHAPE:
          bHit = p->m_cShape.Collide((CCircle*)s, poi);

          if(bHit)
            g_pSoundManager->play(BLASTER_SOUND);
          break;
      } //switch

      if(bHit){        
        if(g_bShowImpacts)
          m_stdDot.push_back(CReticle((ObjectType)((*i)->m_nObjectType - GRAYBALL_OBJECT + GRAYDOT_OBJECT), 0, poi));

        ObjectType obj = UNKNOWN_OBJECT;

        if(m_nNumReticles < NUMRETICLES){
          if((*i)->m_nObjectType >= GRAYBALL_OBJECT && (*i)->m_nObjectType <= PURPLEBALL_OBJECT)
            obj = (ObjectType)((*i)->m_nObjectType - GRAYBALL_OBJECT + GRAYRETICLE_OBJECT);
        
          m_cReticle[m_nNextReticle] = CReticle(obj, g_cTimer.time(), poi);
          m_nNextReticle = (m_nNextReticle + 1)%NUMRETICLES;
          ++m_nNumReticles;
        } //if
      } //if
    } //for
    ++ix;
  } //for

  //ball to ball collision
  m_nLostBalls = 0; //count out of bounds balls
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){ //for each object
    if(!(*i)->m_cShape.m_cAABB.Intersect(m_cAABB, (*i)->m_cShape.m_vPos)) //outside the object world AABB
      ++m_nLostBalls;

    for(auto j=next(i, 1); j!=m_stdList.end(); j++) //for each other object
      NarrowPhase(*i, *j); //narrow phase collision detection and response
  } //for
} //BroadPhase

/// Check whether a pair of objects collides and make appropriate response.
/// \param p0 Pointer to first object.
/// \param p1 Pointer to second object.

void CObjectWorld::NarrowPhase(CObject* p0, CObject* p1){
  CMovingCircle& s0 = p0->m_cShape; //shape of first object
  CMovingCircle& s1 = p1->m_cShape; //shape of second object
  
  m_nAABBTestCnt++;

  //fast return if AABBs don't collide
  if(!s0.m_cAABB.Intersect(s1.m_cAABB, s0.m_vPos, s1.m_vPos))
    return;
  
  m_nCollisionTestCnt++;

  //AABBs collide, but do the shapes?
  Vector2 poi; //for point of intersection
  BOOL bHit = s0.Collide(&s1, poi); //is there a collision?

  if(bHit){ //there's a collision
    m_nCollisionCnt++;
    g_pSoundManager->play(FIRE_SOUND);

    //record POI in reticle list
    if(m_nNumReticles < NUMRETICLES){
      const int t = g_cTimer.time();
      m_cReticle[m_nNextReticle] = CReticle(BIGREDRETICLE_OBJECT, t, poi);
      m_nNextReticle = (m_nNextReticle + 1)%NUMRETICLES;
      ++m_nNumReticles;
    } //if
  } //if
} //NarrowPhase

void CObjectWorld::ClearDots(){
  m_stdDot.clear();
} //ClearDots

/// Get the number of collisions since the count was last reset
/// and reset count to zero.
/// \return Number of collisions.

 int CObjectWorld::GetCollisionCnt(){
   const int n = m_nCollisionCnt;
   m_nCollisionCnt = 0;
   return n;
 } //GetCollisionCnt

/// Get the number of collision tests since the count was last reset
/// and reset count to zero.
/// \return Number of tests.

 int CObjectWorld::GetCollisionTestCnt(){
   const int n = m_nCollisionTestCnt;
   m_nCollisionTestCnt = 0;
   return n;
 } //GetCollisionTestCnt

/// Get the number of AABB collision  tests since the count was last reset
/// and reset count to zero.
/// \return Number of tests.

 int CObjectWorld::GetAABBTestCnt(){
   const int n = m_nAABBTestCnt;
   m_nAABBTestCnt = 0;
   return n;
 } //GetAABBTestCnt

/// \return Number of moving objects in the object list.

int CObjectWorld::GetSize(){
  return m_stdList.size();
} //GetSize

/// \return Number of balls that have strayed outside m_cAABB.

int CObjectWorld::GetLostBallCnt(){
  return m_nLostBalls;
} //GetLostBallCnt