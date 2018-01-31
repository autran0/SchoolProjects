/// \file Hud.cpp
/// \brief Code for the heads-up display class CHeadsUpDisplay.
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

#include "Hud.h"

#include "GameDefines.h"
#include "RenderWorld.h"

extern CRenderWorld g_cRenderWorld;

  /// Create an object in the HUD.
  /// \param t Object type.
  /// \return Pointer to the object created.

CObject* CHeadsUpDisplay::create(ObjectType t){ 
  CObject* p = new CObject(t);
  m_stdList.push_back(p);
  return p;
} //create

/// Draw HUD, which includes a clock and a thermometer.
/// \param secs Number of seconds to display on clock.
/// \param temp Temperature to display on thermometer.
/// \param mt Maximimum temperature that thermometer can display.
    
void CHeadsUpDisplay::draw(int secs, float temp, float mt){
  CObject* p = nullptr; //handy object pointer
  secs = secs%60; //to be sure
  Vector3 s; //sprite position

  for(auto i=m_stdList.begin(); i!=m_stdList.end(); i++){
    p = *i; //shorthand
    BOOL bDraw = FALSE; //don't draw unless we're sure
    float a = 0.0f; //angle

    switch(p->m_nObjectType){ //depending on what type it is
      //temperature guage parts
      /*case TEMPGUAGE_OBJECT:
        bDraw = TRUE; s = Vector3(140.0f, 70.0f, -200.0f);
        break;  

      case TEMPMAXNEEDLE_OBJECT:
        bDraw = TRUE; s = Vector3(25 + min(300.0f, 256.0f * mt/178.0f), 40.0f, -220.0f);
        break;   

      case TEMPNEEDLE_OBJECT:
        bDraw = TRUE; s = Vector3(25 + min(300.0f, 256.0f * temp/178.0f), 40.0f, -210.0f);
        break;

      //clock parts
      case CLOCKFACE_OBJECT:
        bDraw = TRUE; s = Vector3(350.0f, 70.0f, -230.0f);
        break;  

      case CLOCKNEEDLE_OBJECT:
        bDraw = TRUE; 
        s = Vector3(350.0f, 70.0f, -240.0f);
        a = 2.0f*b2_pi*secs/60.0f;
        break; */

      case PIRATE_OBJECT: //easter egg
        if(m_bEasterEgg)
          g_cRenderWorld.draw(p->m_nObjectType, Vector3(80.0f,  128.0f, -256.0f));
        return; //bail 
    } //switch object type

    //now draw with y measured down from the top of the screen
    if(bDraw)g_cRenderWorld.drawtop(p->m_nObjectType, s, a);
  } //for each object
} //draw