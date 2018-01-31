/// \file Cannon.cpp 
/// \brief Code for the cannon class CCannon.
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

#include "Cannon.h"
#include "ObjectWorld.h"
#include "RenderWorld.h"

extern CObjectWorld g_cObjectWorld;
extern b2World g_b2dPhysicsWorld; 
extern CRenderWorld g_cRenderWorld;
extern CTimer g_cTimer; 

CCannon::CCannon(){
  m_pBarrel = m_pBase = nullptr;
  m_pWheel1 = m_pWheel2 = nullptr;
  m_pWheelJoint1 = m_pWheelJoint2 = nullptr;
  Reset();
} //constructor

/// Create a cannon mount in Physics World.
/// \param x Cannon mount's x coordinate in Render World.
/// \param y Cannon mount's y coordinate in Render World.
/// \param nIndex Cannon mount's collision group index.
/// \return Pointer to cannon mount body in Physics World.

b2Body* CCannon::CreateMount(int x, int y, int nIndex){
  int w, h;
  g_cRenderWorld.GetSize(w, h, MOUNT_OBJECT);
  const float w2 = RW2PW(w)/2.0f;
  const float h2 = RW2PW(h)/2.0f;

  //shape
  b2Vec2 vertices[3];
  vertices[0].Set(-w2, -h2);
  vertices[1].Set(w2, -h2);
  vertices[2].Set(0.0f, h2);

  b2PolygonShape shape;
  shape.Set(vertices, 3);

  //fixture
  b2FixtureDef fd;
  fd.shape = &shape;
  fd.density = 1.0f;
  fd.restitution = 0.4f;
  fd.filter.groupIndex = nIndex;    

  //body definition
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y) - h2);
  
  //body
  b2Body* body = g_b2dPhysicsWorld.CreateBody(&bd);
  body->CreateFixture(&fd);
  return body;
} //CreateMount

/// Create a cannon barrel in Physics World.
/// \param x Cannon barrel's x coordinate in Render World.
/// \param y Cannon barrel's y coordinate in Render World.
/// \param nIndex Cannon barrel's collision group index.
/// \return Pointer to cannon barrel body in Physics World.

b2Body* CCannon::CreateBarrel(int x, int y, int nIndex){ 
  //shape
  b2PolygonShape shape;
  shape.SetAsBox(RW2PW(67), RW2PW(22));

  //fixture
  b2FixtureDef fd;
  fd.shape = &shape;
  fd.density = 1.0f;
  fd.restitution = 0.2f;  
  fd.filter.groupIndex = nIndex;
  
  //body definition
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));

  //body
  b2Body* body = g_b2dPhysicsWorld.CreateBody(&bd);
  body->CreateFixture(&fd);
  return body;
} //CreateBarrel

/// Create a cannon wheel  in Physics World.
/// \param x Cannon wheel's x coordinate in Render World.
/// \param y Cannon wheel's y coordinate in Render World.
/// \param nIndex Cannon wheel's collision group index.
/// \return Pointer to cannon wheel body in Physics World.

b2Body* CCannon::CreateWheel(int x, int y, int nIndex){
  int w = g_cRenderWorld.GetWidth(WHEEL_OBJECT); //wheel diameter

  //shape
  b2CircleShape shape;
  shape.m_radius = RW2PW(w)/2.0f;

  //fixture
  b2FixtureDef fd;
  fd.shape = &shape;
  fd.density = 0.8f;
  fd.restitution = 0.6f;
  fd.filter.groupIndex = nIndex;

  //body definition
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));

  //body
  b2Body* body;
  body = g_b2dPhysicsWorld.CreateBody(&bd);
  body->CreateFixture(&fd);
  return body;
} //CreateWheel

/// Create a cannon in Physics World and Object World, linking
/// the two representations together.

void CCannon::create(){
  const int nIndex = -42; //Collision index. Negative means don't collide.
  const int nX = 300; 
  const int nY = 62;

  //Create cannon parts in Object World
  CObject* pBarrel = g_cObjectWorld.create(BARREL_OBJECT);
  CObject* pMount = g_cObjectWorld.create(MOUNT_OBJECT);
  CObject* pWheel1 = g_cObjectWorld.create(WHEEL_OBJECT);
  CObject* pWheel2 = g_cObjectWorld.create(WHEEL_OBJECT);

  //Create cannon parts in Physics World

  //base, barrel, and wheels
  m_pBase = CreateMount(nX, nY + 84, nIndex);
  m_pBarrel = CreateBarrel(nX, nY + 72, nIndex);
  m_pWheel1 = CreateWheel(nX - 30, nY + 16, nIndex);
  m_pWheel2 = CreateWheel(nX + 30, nY + 16, nIndex);
  
  //wheel joint definition
  b2WheelJointDef wd;
  b2Vec2 axis(0.0f, 1.0f); //vertical axis for wheel suspension
  wd.Initialize(m_pBase, m_pWheel1, m_pWheel1->GetPosition(), axis);
  wd.dampingRatio = 0.9f;
  wd.motorSpeed = 0.0f;
  wd.maxMotorTorque = 1000.0f;
  wd.enableMotor = true;

  //create wheel joint for wheel 1
  m_pWheelJoint1 = (b2WheelJoint*)g_b2dPhysicsWorld.CreateJoint(&wd);

  //create wheel joint for wheel 2
  wd.Initialize(m_pBase, m_pWheel2, m_pWheel2->GetPosition(), axis);
  m_pWheelJoint2 = (b2WheelJoint*)g_b2dPhysicsWorld.CreateJoint(&wd);

  //revolute joint definition
  b2RevoluteJointDef jd;
  jd.Initialize(m_pBarrel, m_pBase, m_pBarrel->GetPosition());
  jd.maxMotorTorque = 1000.0f;
  jd.motorSpeed = 0.0f;
  jd.enableMotor = true;

  //create revolute joint
  m_pBarrelJoint = (b2RevoluteJoint*)g_b2dPhysicsWorld.CreateJoint(&jd);
  
  //tell Object World cannon parts about Physics World counterparts
  pBarrel->SetPhysicsBody(m_pBarrel);
  pMount->SetPhysicsBody(m_pBase);
  pWheel1->SetPhysicsBody(m_pWheel1);
  pWheel2->SetPhysicsBody(m_pWheel2);
} //create

/// Rotate the cannon barrel up or down by a small increment.
/// \param a Angle increment in radians. Positive means up, negative means down.

void CCannon::BarrelUp(float a){
  m_pBarrel->SetTransform(m_pBarrel->GetPosition(), 
    m_pBarrel->GetAngle() + a);
} //BarrelUp

/// Start the cannon moving using motors in the wheels.
/// \param speed Speed of cannon. Positive means left, negative means right.

void CCannon::StartMovingLeft(float speed){
  if(m_pWheelJoint1){
    m_pWheelJoint1->SetMotorSpeed(speed);
    m_pWheelJoint1->EnableMotor(TRUE);
  } //if

  if(m_pWheelJoint2){
    m_pWheelJoint2->SetMotorSpeed(speed);
    m_pWheelJoint2->EnableMotor(TRUE); 
  } //if
} //StartMovingLeft

/// Stop the cannon by setting motor speed to zero and disabling the motors.

void CCannon::Stop(){
  if(m_pWheelJoint1){
    m_pWheelJoint1->SetMotorSpeed(0.0f);
    m_pWheelJoint1->EnableMotor(FALSE);
  } //if

  if(m_pWheelJoint2){
    m_pWheelJoint2->SetMotorSpeed(0.0f);
    m_pWheelJoint2->EnableMotor(FALSE); 
  } //if
} //Stop

/// Apply an impulse to part of the cannon, which is made up of many bodies in Physics World.
/// \param b Body to apply impulse to. 
/// \param v Vector direction and magnitude of impulse.
/// \param ds Vector dispolacement to point at which impulse is applied.

void CCannon::Impulse(b2Body* b, const b2Vec2& v, const b2Vec2& ds){
  b->ApplyLinearImpulse(v, b->GetPosition() + ds, true);
} //Impulse

/// Reset the collision group index for part of cannon, which is made up of many bodies in Physics World.
/// \param b Body to reset collision group index of. 

void CCannon::MakeCollide(b2Body* b){
  b2Filter f = b->GetFixtureList()->GetFilterData(); 
  f.groupIndex = 0;
  b->GetFixtureList()->SetFilterData(f); 
} //MakeCollide

/// Make the cannon explode by destroying the joints, applying impulses to cannon parts so that they
/// fly apart, and resetting their collision group indices so that they can collide.

void CCannon::Explode(){
  //break joints
  if(m_pWheelJoint1){ 
    g_b2dPhysicsWorld.DestroyJoint(m_pWheelJoint1);
    m_pWheelJoint1 = nullptr;
  } //if

  if(m_pWheelJoint2){
    g_b2dPhysicsWorld.DestroyJoint(m_pWheelJoint2);
    m_pWheelJoint2 = nullptr;
  } //if

  if(m_pBarrelJoint){
    g_b2dPhysicsWorld.DestroyJoint(m_pBarrelJoint);
    m_pBarrelJoint = nullptr;     
  } //if

  //apply impulses to cannon parts so that they fly apart
  Impulse(m_pBase, b2Vec2(0, 50), b2Vec2(80, 80));
  Impulse(m_pBarrel, b2Vec2(0, 100), b2Vec2(40, 40));
  Impulse(m_pWheel1, b2Vec2(-50, 200), b2Vec2(1, 1));
  Impulse(m_pWheel2, b2Vec2(50, 220), b2Vec2(-1, -1));

  //allow cannon components to collide with each other
  MakeCollide(m_pBase);
  MakeCollide(m_pBarrel);
  MakeCollide(m_pWheel1);
  MakeCollide(m_pWheel2);

  m_bExploded = TRUE;
} //Explode 

/// If enough time has passed for reloading, create a cannonball
/// in both Render World and Physics World and send it on its way. Apply a recoil
/// impulse to the cannon in Physics World. Increment the cannon's temperature.
/// All this is assuming that the cannon isn't nose down and hasn't already exploded.

BOOL CCannon::Fire(){   
  const int w = g_cRenderWorld.GetWidth(BALL_OBJECT); //cannonball diameter
  static int nLastFireTime = 0;

  if(!m_bExploded && g_cTimer.elapsed(nLastFireTime, 250)){
    //m_fTemp += 50.0f;

    //body definition
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    b2Vec2 v = m_pBarrel->GetPosition() +
      b2Mul(b2Rot(m_pBarrel->GetAngle()), b2Vec2(RW2PW(70),0));
    bd.position.Set(v.x, v.y);


    CObject* pObj = g_cObjectWorld.create(BALL_OBJECT);
  
    //shape
    b2CircleShape s;
    s.m_radius = RW2PW(w)/2.0f;

    //fixture
    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 0.5f;
    fd.restitution = 0.3f;
      
    //Physics World body for cannonball
    b2Body* pCannonball = g_b2dPhysicsWorld.CreateBody(&bd);
    pObj->SetPhysicsBody(pCannonball); //tell object world body about physics world body
    pCannonball->CreateFixture(&fd);

    //apply impulses to ball and cannon barrel
    Stop();
    b2Vec2 vImpulse = b2Mul( //impulse vector 
      b2Rot(m_pBarrel->GetAngle()), 
      b2Vec2(200, 0)); //200 kluged for fPhysicsRescaleValue=10
    Impulse(pCannonball, vImpulse); //fire ball   
    Impulse(m_pBarrel, -4.0f * vImpulse); //recoil
    m_nBallsFired++;
    return TRUE;
  } //if
  return FALSE;
} //Fire

/// Return the number of cannonballs fired in this level of the game.
/// \return Number of cannonballs fired.

int CCannon::BallsFired(){
  return m_nBallsFired;
} //BallsFired

/// Cool the cannon's temperature, unless it is too high already,
/// in which case mark it for exploding on the next fire.

void CCannon::CoolDown(){
  if(m_fTemp > m_fMaxTemp) //recompute max temp
    m_fMaxTemp = m_fTemp;
  if(m_fTemp > 0) //cool off
    m_fTemp -= m_fTemp/64.0f;
  if(m_fTemp >= CANNONEXPLODETEMP && !m_bExploded) //too hot
    Explode();
} //CoolDown

/// Determine whether the cannon has exploded already.
/// \return TRUE If it's dead, Dave.

BOOL CCannon::IsDead(){
  return m_bExploded;
} //IsDead

/// Reset the cannon to its initial conditions ready for a new level to begin.

void CCannon::Reset(){
  m_fTemp = m_fMaxTemp = 0.0f;
  m_nBallsFired = 0;
  m_bExploded = FALSE;
} //Reset