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

CObjectWorld::CObjectWorld(){
  m_fCueAngle = 0.0f;
  m_fCircleAngle = 0.0f;
  m_pCueBall = m_p8Ball = nullptr;
  m_bDrawImpulseVector = TRUE;
  m_nIterations = 2;
} //constructor

CObjectWorld::~CObjectWorld(){
 clear();
} //destructor

/// Create an object in the Object World.
/// \param t Object type.
/// \param v Initial position.
/// \param s Size.
/// \return Pointer to the object created.

void CObjectWorld::create(ObjectType t, const Vector2& v, int s){
  CObject* b = new CObject(t, v, s); //conjure a ball
  m_stdList.push_back(b); //place in object list
  if(t == CUEBALL_OBJECT) m_pCueBall = b; //save cue-ball pointer
  else if(t == EIGHTBALL_OBJECT) m_p8Ball = b; //save 8-ball pointer  
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
/// \param w Calling parameter will be set to Object World width.
/// \param h Calling parameter will be set to Object World height.

void CObjectWorld::GetWorldSize(float& w, float& h){
  w = m_vSize.x; h = m_vSize.y;
} //GetWorldSize

/// Draw an arrow at the cue-ball's location representing the impulse vector, 
/// then draw the balls themselvest

void CObjectWorld::draw(){
  //draw arrow on cue ball
  if(m_bDrawImpulseVector && m_pCueBall){ 
	const Vector2 v2 = OW2RW(m_pCueBall->m_vPos); //ball position in Render World
	const Vector3 v3 = Vector3(v2.x, v2.y, 1001.0f); //fake z value to be behind cue ball
    g_cRenderWorld.draw(ARROW_OBJECT, v3, m_fCueAngle);  //draw arrow in Render World
  } //if

  // Draw circle on 8 ball
  if (m_p8Ball && m_pCueBall && m_bDrawImpulseVector)
  {
	  float vol;	// Impact sound volume
	  const float r = (m_p8Ball->m_nSize + m_pCueBall->m_nSize) / 2;	// Ball Radius
	  Vector2 aCueVelocity = 30.0f * Vector2(cos(m_fCueAngle), sin(m_fCueAngle));	// Takes the velocity that will be added and multiplies it in the direction of the player's vector to find the location it will be
	  Vector2 a8Velocity = Vector2(0.0f, 0.0f);		// The 8 ball still is not moving after the initial shot and before the balls collide
	  Vector2 cuePos = m_pCueBall->m_vPos;		// Gets the current cue ball position
	  Vector2 eightPos = m_p8Ball->m_vPos;		// Gets the current eight ball position

	  if (CreateCircle(cuePos, aCueVelocity, eightPos, a8Velocity, r, vol))
	  {
		  Vector2 circlePos = OW2RW(cuePos);	// Gets the position of the circle in RenderWorld
		  Vector3 circleV3Dir = Vector3(circlePos.x, circlePos.y, 1002.0f);

		  g_cRenderWorld.draw(CIRCLE_OBJECT, circleV3Dir);		// Draws the Circle in the game

		  float eightAngle = atan2(a8Velocity.y, a8Velocity.x);	// Gets the angle where the eight ball will go
		  Vector2 eightRWDir = OW2RW(m_p8Ball->m_vPos);			// Gets the velocity of the eight ball in Render World
		  Vector3 eightV3Dir = Vector3(eightRWDir.x, eightRWDir.y, 1003.0f);	// The vector3 direction of the eight ball

		  g_cRenderWorld.draw(ARROW_OBJECT, eightV3Dir, eightAngle);	// Draws the arrow for the eight ball
	  }

  } // if

  int depth = 0; //z depth
  int c = 0; // Count of balls out of pockets
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){ //for each ball
    CObject* p = *i; //handy object pointer
    if(!p->m_bInPocket){ //if there's an object and it's not in a pocket
      Vector2 u = OW2RW(p->m_vPos); //ball position in Render World
      Vector3 v = Vector3(u.x, u.y, 1000.0f - (float)depth++); //fake z value
      g_cRenderWorld.draw(p->m_nObjectType, v); //draw ball in Render World
	  c++;
    } //if
  } //for
} //draw

/// Move all of the objects in the object list and perform collision response.

void CObjectWorld::move(){
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++) //for each ball
    (*i)->move(); //move it
  Collision(); //collision response
} //move

/// Make the impulse vector point from the center of the cue-ball 
/// to the center of the 8-ball.

void CObjectWorld::ResetImpulseVector(){
  m_bDrawImpulseVector = TRUE;
  Vector2 v = m_p8Ball->m_vPos - m_pCueBall->m_vPos; //difference in positions
  m_fCueAngle = atan2(v.y, v.x);
} //ResetImpulseVector

/// Adjust the angle of the impulse vector.
/// \param a Amount to increment angle by.

void CObjectWorld::AdjustImpulseVector(float a){
  m_fCueAngle += a;
} //AdjustImpulseVector

/// Adjust the cue ball up or down.  This is to be used at the start of the game when the
/// cue ball is on the baseline.
/// \param d Distance to move by.

void CObjectWorld::AdjustCueBall(float d){
  if(m_pCueBall){ //safety
    const float r = m_pCueBall->m_nSize/2.0f; //ball radius 
    float& y = m_pCueBall->m_vPos.y; //shorthand
    y += d; //move it vertically  
    y = max(min(y, m_vSize.y - r), r); //clamp between top and bottom of Object World
  } //if
} //AdjustCueBall

/// Shoot the cue-ball by giving it a fixed impulse. Also play a sound that is panned
/// left or right depending on where the cue-ball is on the table.

void CObjectWorld::Shoot(){
  m_bDrawImpulseVector = FALSE; //turn off the impulse vector arrow

  if(m_pCueBall){ //safety
    m_pCueBall->DeliverImpulse(m_fCueAngle, 30.0f); //deliver impulse to cue-ball

    g_pSoundManager->play(CUE_SOUND); //play sound of cue hitting ball
    Vector2 u = OW2RW(m_pCueBall->m_vPos); //position in Render World
    Vector3 v = Vector3(u.x, u.y, 0.0f); //fake a z coordinate
    g_pSoundManager->move(v); //pan sound
  } //if
} //Shoot

/// Check whether the cue-ball or the 8-ball is in a pocket.
/// \return TRUE If one of the balls is in a pocket.

BOOL CObjectWorld::BallDown(){
  return m_pCueBall->m_bInPocket || m_p8Ball->m_bInPocket;
} //BallDown

/// Check whether the cue-ball is in a pocket.
/// \return TRUE If the cue-ball is in a pocket.

BOOL CObjectWorld::CueBallDown(){
  return m_pCueBall->m_bInPocket;
} //CueBallDown

/// Check whether both the cue-ball and the 8-ball have stopped moving.
/// \return TRUE If both balls have stopped moving.

BOOL CObjectWorld::AllStopped(){
  int nMoving = 0; //number of moving balls
  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++)
    if((*i)->m_vVel != Vector2(0.0f)) //if moving
      nMoving++;
  return nMoving <= 0;
} //AllStopped

/// See Section 2.2 for an explanation of the math behind this function.
/// Begin by computing velocities relative to b1. Calculate the relative
/// displacement c and the distance cdotvhat along the normal to common 
/// tangent vhat. Calculate d1 and d2, the  distances moved back by b1
/// and b2 (respectively) to their positions at TOI. Calculate time elapsed
/// since TOI, tdelta = d2/s2. Move balls back to their positions at TOI,
/// and compute their new velocities. Move both balls after impact using
/// their new velocities and tdelta.
/// \param b1Pos Ball 1 position.
/// \param b1Vel Ball 1 velocity.
/// \param b2Pos Ball 2 position.
/// \param b1Vel Ball 2 velocity.
/// \param r Average ball radius.
/// \param s [OUT] Collision speed.

BOOL CObjectWorld::BallCollision(Vector2& b1Pos, Vector2& b1Vel, 
  Vector2& b2Pos, Vector2& b2Vel, const float r, float& s)
{
  const Vector2 v = b2Vel - b1Vel; //relative velocity
  const float speed = v.Length(); //relative speed
  if(speed == 0.0f)return FALSE; //bail
  Vector2 vhat; //normalized version of v
  v.Normalize(vhat); //normalize v into vhat

  //calculate relative displacement and distance along normal to common tangent
  const Vector2 c = b1Pos - b2Pos; //vector from b2 to b1
  const float cdotvhat = c.Dot(vhat); //relative distance along normal to tangent

  float d; //distance moved back by b2 to position at TOI
  const float delta = cdotvhat*cdotvhat - c.LengthSquared() + r*r; //discriminant
  if(delta >= 0.0f) //guard against taking the square root of a negative number
    d = -cdotvhat + sqrt(delta); //collision really did occur
  else return FALSE; //fail, no collision, function should not have been called

  const float tdelta = d/speed; //time elapsed since time of impact

  //move balls back to position at TOI 
  b1Pos -= tdelta*b1Vel; 
  b2Pos -= tdelta*b2Vel; 

  //compute new velocities after impact
  Vector2 nhat; //normal to tangent
  Vector2 n = b1Pos - b2Pos; //vector joining centers at TOI
  n.Normalize(nhat); //normalize n into nhat

  s = v.Dot(nhat); //difference in speed
  const Vector2 vDiff = s*nhat; //difference in velocity
  b1Vel += vDiff; //what one ball gains
  b2Vel -= vDiff; //the other one loses

  //move by the correct amount after impact
  b1Pos += tdelta*b1Vel;
  b2Pos += tdelta*b2Vel;

  return TRUE;
} //BallCollision

/// Collision response for two balls, playing an impact sound
/// if appropriate.
/// \param b1 Pointer to the first ball.
/// \param b2 Pointer to the second ball.
/// \return TRUE iff there was a collision.

BOOL CObjectWorld::BallCollision(CObject* b1, CObject* b2){
  //average radius, with a little extra for a collision buffer
  const float r = (b1->m_nSize + b2->m_nSize)/2.0f + 1.0f;

  float vol; //impact sound volume
  BOOL result = BallCollision(b1->m_vPos, b1->m_vVel, b2->m_vPos, b2->m_vVel, r, vol);

  //sound
  if(result){ //if there was a collision
    g_pSoundManager->play(BALLCLICK_SOUND); 
    Vector2 u = OW2RW(b1->m_vPos); //position in Render World
    g_pSoundManager->move(Vector3(u.x, u.y, 0.0f)); //pan sound
    g_pSoundManager->volume(vol/10.0f); //muffle sound
  } //if

  return result;
} //BallCollision

/// Collision detection and response for a single ball against the other balls.
/// \param i Handle response for collisions with ball at index i in the object list.

void CObjectWorld::BallCollision(OLit i){
  CObject* b1 = *i; //shorthand
  if(b1->m_bInPocket)return; //bail if ball 1 is not on the table

  //Compare against only higher-indexed balls to avoid processing each collision twice.
  for(auto j=++i; j!=m_stdList.end(); j++){
    CObject* b2 = *j; //shorthand
    if(!b2->m_bInPocket){ //if ball 2 is on the table   
      Vector2 v = b1->m_vPos - b2->m_vPos; //position difference
      float d = (b1->m_nSize + b2->m_nSize)/2.0f; //separation distance
      if(v.LengthSquared() < d*d) //if close enough, then it collides
        BallCollision(b1, b2); //collision response for two balls
    } //if 
  } //for 
} //BallCollision

/// Creates circle at TOI
/// \param b1Pos Ball 1 position.
/// \param b1Vel Ball 1 velocity.
/// \param b2Pos Ball 2 position.
/// \param b1Vel Ball 2 velocity.
/// \param r Average ball radius.
/// \param s [OUT] Collision speed.

BOOL CObjectWorld::CreateCircle(Vector2& b1Pos, Vector2& b1Vel,
	Vector2& b2Pos, Vector2& b2Vel, const float r, float& s)
{
	const Vector2 v = b2Vel - b1Vel; //relative velocity
	const float speed = v.Length(); //relative speed
	if (speed == 0.0f)return FALSE; //bail
	Vector2 vhat; //normalized version of v
	v.Normalize(vhat); //normalize v into vhat

					   //calculate relative displacement and distance along normal to common tangent
	const Vector2 c = b1Pos - b2Pos; //vector from b2 to b1
	const float cdotvhat = c.Dot(vhat); //relative distance along normal to tangent

	float d; //distance moved back by b2 to position at TOI
	const float delta = cdotvhat*cdotvhat - c.LengthSquared() + r*r; //discriminant
	if (delta >= 0.0f) //guard against taking the square root of a negative number
		d = -cdotvhat + sqrt(delta); //collision really did occur
	else return FALSE; //fail, no collision, function should not have been called

	const float tdelta = d / speed; //time elapsed since time of impact

									//move balls back to position at TOI 
	b1Pos -= tdelta*b1Vel;
	b2Pos -= tdelta*b2Vel;

	//compute new velocities after impact
	Vector2 nhat; //normal to tangent
	Vector2 n = b1Pos - b2Pos; //vector joining centers at TOI
	n.Normalize(nhat); //normalize n into nhat

	s = v.Dot(nhat); //difference in speed
	const Vector2 vDiff = s*nhat; //difference in velocity
	b1Vel += vDiff; //what one ball gains
	b2Vel -= vDiff; //the other one loses

	return TRUE;
} //CreateCircle

/// Collision detection and response for ball hitting a single rail. 
/// \param s Single coordinate of ball position.
/// \param v Single coordinate of ball velocity.
/// \param r Ball position at rail collision.
/// \param hit Comparison (either greater or less than) for ball overlap with rail.
/// \return TRUE if there was a collision.

BOOL CObjectWorld::RailCollision(float& s, float& v, float r, function<bool(float, float)> hit){   
  if(hit(s, r)){ //ball has gone over rail
    s += 2.0f*(r - s); //move ball back to correct position
    v *= -0.55f; //flip ball velocity and slow down
    return TRUE; //return hit
  } //if 

  return FALSE; //return miss
} //RailCollision

/// Collision detection and response for ball hitting any rail. Checks for a collision, and
/// does the necessary housework for reflecting the ball if it hits a rail. If there is
/// a collision, a sound is played at a volume proportional to the speed of collision
/// and panned to the left or right according to where the collision occurred.
/// \param b Pointer to a ball object to collide with rails.

void CObjectWorld::RailCollision(CObject* b){ 
  const float radius = b->m_nSize/2.0f; //ball radius
  
  //ball center at rail collision for each of the 4 rails
  const float TOP = m_vSize.y - radius;
  const float BOTTOM = radius;
  const float LEFT = radius;
  const float RIGHT = m_vSize.x - radius;

  Vector2& p = b->m_vPos; //ball position
  Vector2& v = b->m_vVel; //ball velocity

  BOOL hit = //did the ball hit a rail?
    RailCollision(p.x, v.x, LEFT,   less<float>())    ||
    RailCollision(p.x, v.x, RIGHT,  greater<float>()) ||
    RailCollision(p.y, v.y, TOP,    greater<float>()) ||
    RailCollision(p.y, v.y, BOTTOM, less<float>());
  
  //sound
  if(hit){   
    g_pSoundManager->play(THUMP_SOUND);
    Vector2 u = OW2RW(p.x, m_vSize.y/2.0f); //position in Render World
    Vector3 v = Vector3(u.x, u.y, 0.0f); //fake a z coordinate
    g_pSoundManager->move(v); //pan sound
    g_pSoundManager->volume(v.Length()/400.0f); //muffle sound
  } //if
} //RailCollision

/// Collision and response for ball-in-pocket. Checks for a collision, and
/// does the necessary housework for disabling a ball if it is in a pocket.
/// If there is a collision, a sound is played at a volume proportional to the
/// speed of collision and panned to the left or right according to where the 
/// pocket is on the table. 
/// \param b Pointer to a ball object to collide with pockets.

void CObjectWorld::PocketCollision(CObject* b){ 
  const float pw = 1.5f*b->m_nSize; //pocket width is 1.5 times ball diameter
  const float hpw = pw/2.0f; //half of that

  //ball center at rail collision for each of the 4 rails
  const float TOP = m_vSize.y - hpw;
  const float BOTTOM = hpw;
  const float LEFT = hpw;
  const float RIGHT = m_vSize.x - hpw;

  Vector2& vVel = b->m_vVel;
  BOOL& bInPocket = b->m_bInPocket;

  //pocket collision calculation

  if(!bInPocket){ 
    float x = b->m_vPos.x; //shorthand
    float y = b->m_vPos.y; //shorthand

    bInPocket = (y < BOTTOM || y > TOP) && //near top or bottom rail
      (x < LEFT || x > RIGHT || //and near left or right rail (that means corner pockets), or
      (fabs(x - m_vSize.x/2) < hpw && //near the center pockets and
      fabs(vVel.y) > fabs(vVel.x))); //moving more vertically than horizontally

    //respond to a collision with the pocket: stop the ball and play a sound
    if(bInPocket){ 
      g_pSoundManager->play(POCKET_SOUND);
      Vector2 u = OW2RW(x, m_vSize.y/2.0f); //position in Render World
      Vector3 v = Vector3(u.x, u.y, 0.0f); //fake a z coordinate
      g_pSoundManager->move(v); //pan sound
      g_pSoundManager->volume(vVel.Length()/10.0f); //muffle sound

      vVel = Vector2(0.0f); //stop the ball
    } //if
  } //if
} //PocketCollision

/// Collision response for all balls against each other and
/// the rails and the pockets. We do pocket collision for all
/// balls first to remove them from the subsequent calculations.
/// \param iterations Number of times to repeat collision test.

void CObjectWorld::Collision(){
  for(int i=0; i<m_nIterations; i++) //repeat a few times
    for(auto j=m_stdList.begin(); j!=m_stdList.end(); j++) //for each ball
      PocketCollision(*j); //collision with pockets

    for(auto j=m_stdList.begin(); j!=m_stdList.end(); j++){ //for each ball
      BallCollision(j); //collisions with other balls
      RailCollision(*j); //collision with rails
    } //for
} //Collision