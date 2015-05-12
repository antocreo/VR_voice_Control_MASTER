//
//  LeapClass.h
//  OculusRenderingBasic
//
//  Created by Let it Brain on 04/05/2015.
//
//

#ifndef __OculusRenderingBasic__LeapClass__
#define __OculusRenderingBasic__LeapClass__


#pragma once

#include "ofxLeapMotion2.h"
#include "ofxStrip.h"
//#include "FloatingObject.h"

class LeapClass {
    
public:
    
    LeapClass();
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    void showHands();
    void refresHands();
    void handDrawing();
    void drawingMesh();
    void drawGestureInfo( float x, float y );
    void activateGesture();
    
    
    ofxLeapMotion leap;
    
    vector <ofxLeapMotionSimpleHand> simpleHands;
    vector <Hand> hands;
    
    
    vector <int> fingersFound;
    ofCamera leapCam;
//    ofEasyCam leapCam;

    ofLight l1;
    ofLight l2;
    ofMaterial m1;
    
    map <int, ofPolyline> fingerTrails;
    
    bool seeHands,isDrawing;
    
    string msg;
    
     
};









#endif /* defined(__OculusRenderingBasic__LeapClass__) */

