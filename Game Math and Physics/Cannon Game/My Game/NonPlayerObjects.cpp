/// \file NonPlayerObjects.cpp
/// \brief Code for the creation of nonplayer objects.
///
/// Creating things with a physics engine means a lot of tedious,
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

extern b2World g_b2dPhysicsWorld; 
extern CRenderWorld g_cRenderWorld;
extern CObjectWorld g_cObjectWorld;

/// Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// top, bottom, right, and left edges of the screen in Render World, and a
/// ledge for the cannon to sit upon. The left and  right edges continue upwards
/// for a distance. There is no top to the world.

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

  //ledge
  const float lh = RW2PW(62); //ledge height
  shape.Set(b2Vec2(0, lh), b2Vec2(w/2, lh));
  edge->CreateFixture(&shape, 0);
} //CreateWorldEdges

/// Place a crate in Physics World and Object World.
/// \param x Horizontal coordinate in Physics World units.
/// \param y  Vertical coordinate in Physics World units.
/// \param fd Crate fixture definition.

void PlaceCrate(float x, float y, const b2FixtureDef& fd){  
  //Object World
  CObject* pObj = g_cObjectWorld.create(CRATE_OBJECT);

  //Physics World
  b2BodyDef bd; //Physics World body definition
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  b2Body* p = g_b2dPhysicsWorld.CreateBody(&bd);
  pObj->SetPhysicsBody(p); //tell object world body about physics world body
  p->CreateFixture(&fd);
} //PlaceCrate

/// Create a tower of crates at the default location.

void CreateTower(){
  float w, h; //Object World width and height of world
  g_cObjectWorld.GetWorldSize(w, h);

  int cw, ch; //Render World crate width and height
  g_cRenderWorld.GetSize(cw, ch, CRATE_OBJECT);

  const float cw2 = RW2PW(cw - 4.0f)/2.0f; //crate half width in Physics World
  const float ch2 = RW2PW(ch - 4.0f)/2.0f; //crate half height in Physics World

  //Box2D crate shape and fixture
  b2PolygonShape s;
  s.SetAsBox(cw2, ch2);

  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 1.0f;
  fd.restitution = 0.3f;

  //build tower
  for(int i=0; i<12; i++){
    float x = RW2PW(0.7f*w);
    float y = ch2 + 2.0f*ch2*i;
    if(i&1) //single block at odd-numbered layers   
      PlaceCrate(x + cw2, y, fd);
    else{ //pair of blocks at even-numbered layers
      const float d = RW2PW(2);
      PlaceCrate(x - d, y, fd);
      PlaceCrate(x + 2.0f*cw2 + d, y, fd);
    } //else
  } //for
} //CreateTower