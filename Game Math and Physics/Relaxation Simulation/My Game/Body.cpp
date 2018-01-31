/// \file Body.cpp
/// \brief Code for the body class CBody.

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

#include "Body.h"

#include "ObjectWorld.h"
#include "Timer.h"

#include "debug.h"

extern CTimer g_cTimer; 
extern CObjectWorld g_cObjectWorld;
extern float g_fSizeMult;

/// Resize the member vectors to hold the right number of particles and springs,
/// initializing them to NULL.
/// \param p Number of particles.
/// \param s Number of springs.

void CBody::Initialize(int p, int s){
  m_stdPt.resize(p, nullptr);
  m_stdSCtr.resize(s, nullptr);
  m_stdSpr.resize(s, nullptr);
} //Initialize

/// Choose the kind of sprites used for this body, sticks or springs.
/// \param b The sprite to be used for the particles.
/// \param s The sprite to be used for the springs.
/// \param r The spring restitution value, used to decide whether this is a stick or a spring.

void CBody::GetSpriteType(SpriteType& b, SpriteType& s, float r){
  if(r > 0.49f){ //if coefficient of restitution is large, sticks and circles
    b = WOODCIRCLE_SPRITE; 
    s = STICK_SPRITE;
  } //if
  else{ //springs and balls
    b = BALL_SPRITE; 
    s = SPRING_SPRITE;
  } //else
} //GetSpriteType

/// Connect a spring between two particles.
/// \param p0 Index of first particle.
/// \param p1 Index of second particle.
/// \param s The spring.
/// \param r Restitution value for the spring.

void CBody::ConnectSpring(int p0, int p1, int s, float r){
  m_stdSpr[s] = g_cObjectWorld.create(m_stdPt[p0], m_stdPt[p1], m_stdSCtr[s], r);
} //ConnectSpring

/// Create virtual particle for center of spring.
/// \param s Index of spring in question.
/// \param t The type of sprite there.

void CBody::SetSpringSprite(int s, SpriteType t){
  m_stdSCtr[s] = g_cObjectWorld.create(t, Vector2(0.0f));
} //SetSpringSprite

/// Create particle for end of spring.
/// \param p Index of particle in question.
/// \param s Sprite to be drawn there.
/// \param v Location of particle.
/// \return Pointer to particle for end of spring.

CParticle* CBody::CreateParticle(int p, SpriteType s, const Vector2& v){
  CParticle* part = g_cObjectWorld.create(s, v);
  m_stdPt[p] = part;
  return part;
} //CreateParticle

/// Deliver an impulse of a given magnitude at a random angle.
/// This code has changed slightly from the code in the book.
/// The impulse now tends to be generally towards the center of the screen
/// rather than totally random so that you are no longer
/// dashing the body against the floor half the time. This is done
/// by computing the angle of the vector from the body center of mass
/// to the center of the screen and adding a pseudorandom angle in the range
/// plus or minus 0.25 radians.
/// \param magnitude Magnitude of impulse.

void CBody::DeliverImpulse(float m){
  //I'm going to totally fake a pseudorandom number here with a
  //linear congruential generator. It's crummy, but good enough.
  const int P1 = 3617; //a prime
  const int P2 = 2141; //another prime
  float fRand = //a chaotic floating point number between -1.0f and 1.0f
    2.0f*((g_cTimer.time()*P1)%P2)/P2 - 1.0f;
  float a = fRand*XM_PI;

  //deliver impulse to all particles
  int sz = m_stdPt.size();
  for(int i=0; i<sz; i++)
    m_stdPt[i]->DeliverImpulse(a, m);
  
  //deliver sideways impulse to one particle to rotate the body
  m_stdPt[0]->DeliverImpulse(a + XM_PI/2.0f, m);
} //DeliverImpulse

/// Teleport the body without moving any of the particle relative to each other.
/// \param xdelta Amount to move horizontally.
/// \param ydelta Amount to move vertically.

void CBody::Teleport(float xdelta, float ydelta){
  int sz = m_stdPt.size();
  for(int i=0; i<sz; i++){
    m_stdPt[i]->m_vPos.x += xdelta;
    m_stdPt[i]->m_vPos.y += ydelta;
    m_stdPt[i]->m_vOldPos.x += xdelta;
    m_stdPt[i]->m_vOldPos.y += ydelta;
 } //for
} //Teleport

/// Move the body.
/// Since the components of the body (particles and springs) are responsible for moving themselves,
/// all we need to do here is to orient the particles at the ends and middle of the springs correctly.

void CBody::move(){
  int sz = m_stdSpr.size();
  for(int i=0; i<sz; i++)
    if(m_stdSpr[i]->m_pCenter && m_stdSpr[i]->m_pCenter->m_nSpriteType == STICK_SPRITE)
      m_stdSpr[i]->m_pV0->m_fAngle = 
        m_stdSpr[i]->m_pV1->m_fAngle = 
          m_stdSpr[i]->m_pCenter->m_fAngle;
} //move

/// Make a chain of points in a straight line connected by springs.
/// \param count Number of points
/// \param radius Half the length of one of the springs.
/// \param r Coefficient of restitution of the springs.
/// \param angle Angle that the chain makes with the horizontal.
/// \param v Location of chain center.
/// \return Pointer to the chain body.

CParticle* CBody::MakeChain(int n, int r, float s, float a, Vector2 v){
  if(n < 2)return nullptr; //fail and bail

  Initialize(n, n - 1); //reserve space for particles and springs

  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  GetSpriteType(nVertexObject, nEdgeObject, s);

  //x and y offsets between balls
  const float dx = 2.0f*r*cos(a); 
  const float dy = 2.0f*r*sin(a);

  //position for first ball
  v.x -= (float)(n - 1)*r;

  //first ball and spring
  SetSpringSprite(0, nEdgeObject);
  CreateParticle(0, nVertexObject, v);

  for(int i=1; i<n-1; i++){ //for the rest of the springs
    //add a ball and a spring, connect previous spring
    SetSpringSprite(i, nEdgeObject);
    v.x += dx; v.y += dy; //offset position
    CreateParticle(i, nVertexObject, v);
    ConnectSpring(i-1, i, i-1, s);
  } //for

  //last ball
  v.x += dx; v.y += dy;
  m_stdPt[n-1] = g_cObjectWorld.create(nVertexObject, v); 
  ConnectSpring(n-2, n-1, n-2, s);

  //clean up
  m_stdSCtr.clear();
  return m_stdPt[0];
} //MakeChain

/// Make a triangle of points connected by springs.
/// \param r Half the length of one of the springs.
/// \param s Coefficient of restitution of the springs.
/// \param v Location of triangle center.
/// \return Pointer to the triangle body.

CParticle* CBody::MakeTriangle(int r, float s, Vector2 v){ 
  Initialize(3, 3); //reserve space for particles and springs

  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  GetSpriteType(nVertexObject, nEdgeObject, s);

  v.y += r;

  //edge objects
  for(int i=0; i<3; i++)
    SetSpringSprite(i, nEdgeObject);

  //balls
  CreateParticle(0, nVertexObject, v);
  v += (float)r*Vector2(1.0f, -tan(XM_PI/3.0f));
  CreateParticle(1, nVertexObject, v);
  v.x -= 2.0f * r;
  CreateParticle(2, nVertexObject, v);
  
  //tie springs to balls
  ConnectSpring(0, 1, 0, s); 
  ConnectSpring(1, 2, 1, s); 
  ConnectSpring(2, 0, 2, s); 

  //clean up
  m_stdSCtr.clear();
  return m_stdPt[0];
} //MakeTriangle

/// Make a cross-braced square of points connected by springs.
/// \param r Half the length of one of the springs.
/// \param s Coefficient of restitution of the springs.
/// \param v Location of square center.
/// \return Pointer to the square body.

CParticle* CBody::MakeSquare(int r, float s, Vector2 v){
  Initialize(4, 6); //reserve space for particles and springs

  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  GetSpriteType(nVertexObject, nEdgeObject, s);

  v += Vector2(-(float)r, (float)r);

  //edge objects
  for(int i=0; i<6; i++)
    SetSpringSprite(i, nEdgeObject);

  //balls
  CreateParticle(0, nVertexObject, v);
  v.x += 2*r; CreateParticle(1, nVertexObject, v);
  v.y -= 2*r; CreateParticle(2, nVertexObject, v);
  v.x -= 2*r; CreateParticle(3, nVertexObject, v);
  
  //tie springs to balls
  ConnectSpring(0, 1, 0, s); 
  ConnectSpring(1, 2, 1, s); 
  ConnectSpring(2, 3, 2, s); 
  ConnectSpring(3, 0, 3, s); 
  ConnectSpring(0, 2, 4, s); 
  ConnectSpring(1, 3, 5, s);
  
  //clean up
  m_stdSCtr.clear();
  return m_stdPt[0];
} //MakeSquare

/// Make a cross-braced wheel of points connected by springs.
/// \param n Number of sides to the wheel.
/// \param radius Half the length of one of the springs.
/// \param r Coefficient of restitution of the springs.
/// \param v Location of wheel center.
/// \return Pointer to the wheel body.

CParticle* CBody::MakeWheel(int n, int r, float s, Vector2 v){
  Initialize(n + 1, 2*n); //reserve space for particles and springs

  //decide whether to draw springs or sticks
  SpriteType nVertexObject, nEdgeObject;
  GetSpriteType(nVertexObject, nEdgeObject, s);

  v.y += r/2.0f;

  //springs
  for(int i=0; i<2*n; i++)
    SetSpringSprite(i, nEdgeObject);

  v.y += r/2.0f;

  //balls
  CreateParticle(0, nVertexObject, v);
  for(int i=0; i<n; i++){
    float theta = 2.0f*i*XM_PI/n;
    Vector2 vBall = Vector2(v.x + r*cos(theta), v.y + r*sin(theta));    
    CreateParticle(i+1, nVertexObject, vBall);
  } //for

  //springs
  for(int i=0; i<n; i++){
    ConnectSpring(0, i+1, i, s); //spoke
    ConnectSpring(i, i+1, n+i-1, s); //rim
  } //for
  ConnectSpring(n, 1, 2*n-1, s); //rim
  
  //clean up
  m_stdSCtr.clear();
  return m_stdPt[1];
} //MakeWheel

/// Make a cross-braced stick figure with spring constraints.
/// \param v Location of ragdoll center.
/// \return Pointer to the ragdoll body.

CParticle* CBody::MakeRagdoll(Vector2 v){ 
  Initialize(NUM_POINTS, NUM_EDGES); //reserve space for particles and springs

  //Various body measurements. Tune them to your requirements.
  const float TORSO_LENGTH = 170 * g_fSizeMult; //length of torso
  const float SHOULDER_LENGTH = 90 * g_fSizeMult; //width of collar bone
  const float SHOULDER_DROP = 40 * g_fSizeMult; //height of collar bone
  const float HIP_LENGTH = 42 * g_fSizeMult; //width of pelvis
  const float HIP_DROP = 40 * g_fSizeMult; //height of pelvis
  const float LIMB_LENGTH = 100 * g_fSizeMult; //length of limb segments
  const float NECK_LENGTH = 78 * g_fSizeMult; //length of neck, obviously
  const float ELBOW_OFFSET = 15 * g_fSizeMult; //elbows stick out this much
  const float LEG_SPREAD = 5 * g_fSizeMult; //feet spread out this much

  //Store point locations in a handy array for later.
  Vector2 vPointLoc[NUM_POINTS];

  vPointLoc[STERNUM_POINT] = 
    v + Vector2(0, TORSO_LENGTH);
  vPointLoc[HEAD_POINT] = 
    vPointLoc[STERNUM_POINT] + Vector2(0, NECK_LENGTH);
  vPointLoc[LEFT_SHOULDER_POINT] = 
    vPointLoc[STERNUM_POINT] + Vector2(-SHOULDER_LENGTH, -SHOULDER_DROP);
  vPointLoc[LEFT_ELBOW_POINT] = 
    vPointLoc[LEFT_SHOULDER_POINT] + Vector2(-ELBOW_OFFSET, -LIMB_LENGTH);
  vPointLoc[LEFT_HAND_POINT] = 
    vPointLoc[LEFT_ELBOW_POINT] + Vector2(0, -LIMB_LENGTH);  
  vPointLoc[RIGHT_SHOULDER_POINT] = 
    vPointLoc[STERNUM_POINT] + Vector2(SHOULDER_LENGTH, -SHOULDER_DROP);
  vPointLoc[RIGHT_ELBOW_POINT] = 
    vPointLoc[RIGHT_SHOULDER_POINT] + Vector2(ELBOW_OFFSET, -LIMB_LENGTH);
  vPointLoc[RIGHT_HAND_POINT] = 
    vPointLoc[RIGHT_ELBOW_POINT] + Vector2(0, -LIMB_LENGTH);  
  vPointLoc[LEFT_HIP_POINT] = 
    v + Vector2(-HIP_LENGTH, -HIP_DROP);
  vPointLoc[LEFT_KNEE_POINT] = 
    vPointLoc[LEFT_HIP_POINT] + Vector2(-LEG_SPREAD, -LIMB_LENGTH);
  vPointLoc[LFOOT_POINT] = 
    vPointLoc[LEFT_KNEE_POINT] + Vector2(-LEG_SPREAD, -LIMB_LENGTH);
  vPointLoc[RIGHT_HIP_POINT] = 
    v + Vector2(HIP_LENGTH, -HIP_DROP);
  vPointLoc[RIGHT_KNEE_POINT] = 
    vPointLoc[RIGHT_HIP_POINT] + Vector2(LEG_SPREAD, -LIMB_LENGTH);
  vPointLoc[RIGHT_FOOT_POINT] = 
    vPointLoc[RIGHT_KNEE_POINT] + Vector2(LEG_SPREAD, -LIMB_LENGTH);

  for(EdgeType i=NECK_BONE; i<=RIGHT_NECK_BRACER; i=(EdgeType)(i+1))
    SetSpringSprite(i, STICK_SPRITE);
 
  //create sticks with their end points
  //WARNING: Make sure to get the draw order right. It's a bit of a pain
  CreateParticle(LEFT_KNEE_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_KNEE_POINT]);
  CreateParticle(LFOOT_POINT, WOODCIRCLE_SPRITE, vPointLoc[LFOOT_POINT]);
  CreateParticle(RIGHT_KNEE_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_KNEE_POINT]);
  CreateParticle(RIGHT_FOOT_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_FOOT_POINT]);

  //Make sure he'd not a pinhead. Zippy need not apply.
  CParticle* head = CreateParticle(HEAD_POINT, WOODCIRCLE_SPRITE, vPointLoc[HEAD_POINT]);
  const float fBigHead = 1.3f; //his head ball is this much bigger than a normal ball
  head->m_fXScale = head->m_fYScale = fBigHead; //looks bigger
  head->m_fRadius *= fBigHead; //is bigger

  CreateParticle(STERNUM_POINT, WOODCIRCLE_SPRITE, vPointLoc[STERNUM_POINT]);
  CreateParticle(LEFT_HIP_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_HIP_POINT]);
  CreateParticle(RIGHT_HIP_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_HIP_POINT]);
  CreateParticle(LEFT_SHOULDER_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_SHOULDER_POINT]);
  CreateParticle(LEFT_HAND_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_HAND_POINT]);
  CreateParticle(LEFT_ELBOW_POINT, WOODCIRCLE_SPRITE, vPointLoc[LEFT_ELBOW_POINT]);
  CreateParticle(RIGHT_SHOULDER_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_SHOULDER_POINT]);
  CreateParticle(RIGHT_HAND_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_HAND_POINT]);
  CreateParticle(RIGHT_ELBOW_POINT, WOODCIRCLE_SPRITE, vPointLoc[RIGHT_ELBOW_POINT]);

  //connect objects at ends of sticks to center object representing the stick
  ConnectSpring(HEAD_POINT, STERNUM_POINT, NECK_BONE);
  ConnectSpring(STERNUM_POINT, LEFT_SHOULDER_POINT, LEFT_COLLAR_BONE);
  ConnectSpring(STERNUM_POINT, RIGHT_SHOULDER_POINT, RIGHT_COLLAR_BONE);
  ConnectSpring(LEFT_HIP_POINT, LEFT_KNEE_POINT, LEFT_THIGH_BONE);
  ConnectSpring(LEFT_KNEE_POINT, LFOOT_POINT, LEFT_SHIN_BONE);
  ConnectSpring(RIGHT_HIP_POINT, RIGHT_KNEE_POINT, RIGHT_THIGH_BONE);
  ConnectSpring(RIGHT_KNEE_POINT, RIGHT_FOOT_POINT, RIGHT_SHIN_BONE);
  ConnectSpring(LEFT_HIP_POINT, RIGHT_HIP_POINT, HIP_BRACER);
  ConnectSpring(RIGHT_HIP_POINT, STERNUM_POINT, RIGHT_SHOULDER_BRACER);
  ConnectSpring(LEFT_HIP_POINT, STERNUM_POINT, LEFT_SHOULDER_BRACER);
  ConnectSpring(LEFT_SHOULDER_POINT, RIGHT_SHOULDER_POINT, SHOULDER_BRACER);
  ConnectSpring(RIGHT_SHOULDER_POINT, LEFT_HIP_POINT, LEFT_NECK_BRACER);
  ConnectSpring(LEFT_SHOULDER_POINT, RIGHT_HIP_POINT, RIGHT_NECK_BRACER);
  ConnectSpring(LEFT_SHOULDER_POINT, LEFT_ELBOW_POINT, LEFT_HUMERUS_BONE);
  ConnectSpring(LEFT_ELBOW_POINT, LEFT_HAND_POINT, LEFT_ULNUS_BONE);
  ConnectSpring(RIGHT_SHOULDER_POINT, RIGHT_ELBOW_POINT, RIGHT_HUMERUS_BONE);
  ConnectSpring(RIGHT_ELBOW_POINT, RIGHT_HAND_POINT, RIGHT_ULNUS_BONE);

  //springs to keep limbs more or less in the right places
  ConnectSpring(HEAD_POINT, LEFT_SHOULDER_POINT, LEFT_HEAD_SPRING, 0.1f);
  ConnectSpring(HEAD_POINT, RIGHT_SHOULDER_POINT, RIGHT_HEAD_SPRING, 0.1f);

  ConnectSpring(LFOOT_POINT, RIGHT_FOOT_POINT, FEET_SPRING, 0.02f);
  ConnectSpring(LEFT_KNEE_POINT, RIGHT_HIP_POINT, LEFT_KNEE_SPRING, 0.06f);
  ConnectSpring(RIGHT_KNEE_POINT, LEFT_HIP_POINT, RIGHT_KNEE_SPRING, 0.06f);
  ConnectSpring(LFOOT_POINT, RIGHT_HIP_POINT, LEFT_FOOT_SPRING, 0.4f);
  ConnectSpring(RIGHT_FOOT_POINT, LEFT_HIP_POINT, RIGHT_FOOT_SPRING, 0.4f);
  
  ConnectSpring(LEFT_HAND_POINT, RIGHT_HAND_POINT, HANDS_SPRING, 0.01f);
  ConnectSpring(LEFT_ELBOW_POINT, RIGHT_SHOULDER_POINT, LEFT_ELBOW_SPRING, 0.01f);
  ConnectSpring(RIGHT_ELBOW_POINT, LEFT_SHOULDER_POINT, RIGHT_ELBOW_SPRING, 0.01f);
  ConnectSpring(LEFT_HAND_POINT, RIGHT_SHOULDER_POINT, LEFT_HAND_SPRING, 0.1f);
  ConnectSpring(RIGHT_HAND_POINT, LEFT_SHOULDER_POINT, RIGHT_HAND_SPRING, 0.1f);

  //clean up and exit  
  m_stdSCtr.clear();
  return m_stdPt[HEAD_POINT];
} //MakeRagdoll