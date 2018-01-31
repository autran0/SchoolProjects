/// \file nonplayerobjects.cpp
/// \brief Code for the creation of nonplayer objects.
///
/// Making things with a physics engine means a lot of tedious,
/// intricate initialization code. It makes sense to hide it in a file of
/// its own. It's not that it's so difficult, but that there is so much of it.
/// There are so many niggly little details to get right.
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

#include "GameDefines.h"
#include "Box2D\Box2D.h"
#include "ObjectWorld.h"
#include "RenderWorld.h"

#include "debug.h"

extern b2World g_b2dPhysicsWorld; 
extern CObjectWorld g_cObjectWorld;
extern CRenderWorld g_cRenderWorld;

/// Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// top, bottom, right, and left edges of the screen in Render World. The left and
/// right edges continue upwards for a distance. There is no top to the world.

void CreateWorldEdges(){
  float w, h;
  g_cObjectWorld.GetWorldSize(w, h);
  w = RW2PW(w);
  h = RW2PW(2.0f * h);

  const b2Vec2 vBottomLeft = b2Vec2(0, 0);
  const b2Vec2 vBottomRight = b2Vec2(w, 0);
  const b2Vec2 vTopLeft = b2Vec2(0, h);
  const b2Vec2 vTopRight = b2Vec2(w, h);

  //Box2D ground
  b2BodyDef bd;
	b2Body* edge = g_b2dPhysicsWorld.CreateBody(&bd);
  b2EdgeShape shape;
	shape.Set(vBottomLeft, vBottomRight);
	edge->CreateFixture(&shape, 0);

  //Box2D left edge of screen
  shape.Set(vBottomLeft, vTopLeft);
	edge->CreateFixture(&shape, 0);

  //Box2D right edge of screen
  shape.Set(vBottomRight, vTopRight);
	edge->CreateFixture(&shape, 0); 
} //CreateWorldEdges

/// Place a crate in Physics World and Object World.
/// \param x Horizontal coordinate in Physics World units.
/// \param y  Vertical coordinate in Physics World units.

void CreateCrate(float x, float y){ 
  CObject* pObj = g_cObjectWorld.create(CRATE_OBJECT); //Object World

  //Physics World
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  //shape
  int w, h; //crate width and height in Render World
  g_cRenderWorld.GetSize(w, h, CRATE_OBJECT);
  b2PolygonShape s;
  s.SetAsBox(RW2PW(w)/2.0f, RW2PW(h)/2.0f);

  //fixture
  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 1.0f;
  fd.restitution = 0.3f;

  //body
  b2Body* pCrate = g_b2dPhysicsWorld.CreateBody(&bd);
  pObj->SetPhysicsBody(pCrate); //tell object world body about physics world body
  pCrate->CreateFixture(&fd);
} //CreateCrate

/// Place a tire in Physics World and Object World.
/// \param x Horizontal coordinate in Physics World units.
/// \param y  Vertical coordinate in Physics World units.

void CreateTire(float x, float y){ 
  CObject* pGameObject = g_cObjectWorld.create(TIRE_OBJECT); //Object World

  //Physics World
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  //shape
  b2CircleShape s;
	s.m_radius = RW2PW(g_cRenderWorld.GetWidth(TIRE_OBJECT))/2.0f;

  //fixture
	b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.8f;
  fd.restitution = 0.8f;

  //body
  b2Body* pTire = g_b2dPhysicsWorld.CreateBody(&bd);
  pGameObject->SetPhysicsBody(pTire); //tell object world body about physics world body
  pTire->CreateFixture(&fd);
} //CreateTire

/// Place the windmill base in Physics World and Object World
/// \param x Horizontal coordinate in Physics World units.
/// \param y  Vertical coordinate in Physics World units.

void CreateWindmill(float x, float y) {

	// Windmill Blades 
	CObject* windmillBlades = g_cObjectWorld.create(WINDMILLBLADES_OBJECT);

	b2BodyDef bd2;
	bd2.type = b2_dynamicBody;
	bd2.position.Set(70.0f, 20.0f);

	//shape
	b2CircleShape s;
	s.m_radius = RW2PW(g_cRenderWorld.GetWidth(WINDMILLBLADES_OBJECT)) / 2.0f;

	//fixture
	b2FixtureDef fd;
	fd.shape = &s;
	fd.density = 1.0f;

	//body
	b2Body* pWindmillBlades = g_b2dPhysicsWorld.CreateBody(&bd2);
	windmillBlades->SetPhysicsBody(pWindmillBlades);
	pWindmillBlades->CreateFixture(&fd);


	// Windmill Base
	CObject* pGameObject = g_cObjectWorld.create(WINDMILLBASE_OBJECT); //Object World

    //Physics World
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(x, y);

	// Vertices
	b2Vec2 vertices1[3];	// Top Triangle
	vertices1[0].Set(-15.5f, 27.0f);
	vertices1[1].Set(-15.9f, 17.3f);
	vertices1[2].Set(10.1f,  20.5f);

	b2Vec2 vertices2[3];	// Base Triangle
	vertices2[0].Set(1.5f,  -27.0f);
	vertices2[1].Set(14.1f, -27.0f);
	vertices2[2].Set(7.0f,   25.5f);

	b2Vec2 vertices3[3];	// Center Triangle
	vertices3[0].Set(0.0f,   1.7f);
	vertices3[1].Set(15.6f,  1.2f);
	vertices3[2].Set(3.7f,   4.3f);

	//shapes
	b2PolygonShape s1;
	s1.Set(vertices1, 3);
	
	b2PolygonShape s2;
	s2.Set(vertices2, 3);
	
	b2PolygonShape s3;
	s3.Set(vertices3, 3);

	//fixtures
	b2FixtureDef fd1;
	fd1.shape = &s1;
	fd1.density = 1.0f;

	b2FixtureDef fd2;
	fd2.shape = &s2;
	fd2.density = 1.0f;

	b2FixtureDef fd3;
	fd3.shape = &s3;
	fd3.density = 1.0f;
	
	//body
	b2Body* pWindmillBase = g_b2dPhysicsWorld.CreateBody(&bd);
	pGameObject->SetPhysicsBody(pWindmillBase); //tell object world body about physics world body
	pWindmillBase->CreateFixture(&fd1);
	pWindmillBase->CreateFixture(&fd2);
	pWindmillBase->CreateFixture(&fd3);


	// Joint
	b2RevoluteJointDef rjd;
	rjd.collideConnected = false;
	rjd.bodyA = pWindmillBase;
	rjd.bodyB = pWindmillBlades;
	rjd.localAnchorA.Set(6.0f, 20.0f);
	rjd.localAnchorB.Set(0.0f, 0.0f);
	rjd.enableMotor = true;
	rjd.maxMotorTorque = 6000;
	rjd.motorSpeed = -10.0f;
	b2RevoluteJoint* joint = (b2RevoluteJoint*)g_b2dPhysicsWorld.CreateJoint(&rjd);
	
} //CreateWindmill