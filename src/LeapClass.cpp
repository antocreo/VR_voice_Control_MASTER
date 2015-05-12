//
//  LeapClass.cpp
//  OculusRenderingBasic
//
//  Created by Let it Brain on 04/05/2015.
//
//

#include "LeapClass.h"

//--------------------------------------------------------------


LeapClass::LeapClass(){

}


//--------------------------------------------------------------


void LeapClass::setup(){
    
//    ofSetFrameRate(60);
//    ofSetVerticalSync(true);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    leap.open();
    leap.setupGestures();
  
    
    l1.setPosition(200, 300, 50);
    l2.setPosition(-200, -200, 50);
    
    //    leapCam.disableMouseInput();
    
    leapCam.setPosition(ofVec3f(0, 10.f, 500.f));
    leapCam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
    leapCam.setOrientation(ofPoint(0, 0, 0));
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    seeHands=true;
    isDrawing =true;
    
    
}


//--------------------------------------------------------------
void LeapClass::update(){
    
    

    //reset fingers at each update
    fingersFound.clear();
    
    //refresh Hands
    refresHands();
    
    ///gesture
    leap.updateGestures();  // check for gesture updates
    
    //drawing meshes update
    drawingMesh();
    
    
    
    //last line important!!!
    leap.markFrameAsOld();	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
    
    
}


//--------------------------------------------------------------
void LeapClass::draw(){
//    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
    
    
    
    leapCam.begin();
    
    //hand drawing
    handDrawing();
    
    //boolian show hands
    showHands();
    
    leapCam.end();
    
    
    
    //activate the gesture
    activateGesture();
    
    
    
}

//--------------------------------------------------------------
void LeapClass::keyPressed(int key){
    
    //    floatingObj.keyPressed(key);
    
    
    if (key=='h') {
        seeHands =! seeHands;
    }
    
    if (key=='d') {
        isDrawing =! isDrawing;
    }
    
}

//--------------------------------------------------------------
void LeapClass::keyReleased(int key){
    
}

//--------------------------------------------------------------
void LeapClass::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void LeapClass::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void LeapClass::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void LeapClass::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void LeapClass::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void LeapClass::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void LeapClass::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void LeapClass::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
//--------------------------------------------------------------
void LeapClass::handDrawing(){
    
    //keyboard 'd'
    
    if (isDrawing) {
        
        m1.begin();
        m1.setShininess(0.6);
        
        
        for(int i = 0; i < fingersFound.size(); i++){
            ofxStrip strip;
            int id = fingersFound[1];
            
            ofPolyline & polyline = fingerTrails[id];
            strip.generate(polyline.getVertices(), 2, ofPoint(0, 0.5, 0.5) );
            
            ofSetColor(255, 0, id * 25);
            strip.getMesh().draw();
        }
        
        
        for(int i = 0; i < simpleHands.size(); i++){
            //            simpleHands[i].debugDraw();
        }
        
        m1.end();
        
    }
}

//--------------------------------------------------------------

void LeapClass::drawingMesh(){
    
    
    vector <Hand> hands = leap.getLeapHands();
    if( leap.isFrameNew() && hands.size() ){
        
        //leap returns data in mm - lets set a mapping to our world space.
        //you can get back a mapped point by using ofxLeapMotion::getMappedofPoint with the Leap::Vector that tipPosition returns
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < hands.size(); i++){
            for(int j = 0; j < 5; j++){
                ofPoint pt;
                
                const Finger & finger = hands[i].fingers()[ fingerTypes[j] ];
                
                //here we convert the Leap point to an ofPoint - with mapping of coordinates
                //if you just want the raw point - use ofxLeapMotion::getofPoint
                pt = leap.getMappedofPoint( finger.tipPosition() );
                pt = leap.getMappedofPoint( finger.jointPosition(finger.JOINT_DIP) );
                
                //pt = leap.getofPoint( finger.tipPosition() );
                
                ////// drawing meshes///////
                
                //lets get the correct trail (ofPolyline) out of our map - using the finger id as the key
                ofPolyline & polyline = fingerTrails[finger.id()];
                
                //if the distance between the last point and the current point is too big - lets clear the line
                //this stops us connecting to an old drawing
                if( (polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50) || !isDrawing ){
                    polyline.clear();
                }
                
                //add our point to our trail
                polyline.addVertex(pt);
                
                ////// end drawing meshes ///////
                
                
                //store fingers seen this frame for drawing
                fingersFound.push_back(finger.id());
                
                
                
                
            }
        }
    }
    
}


//--------------------------------------------------------------
void LeapClass::refresHands(){
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);
            }
        }
    }
    
    
}


//--------------------------------------------------------------
void LeapClass::showHands(){
    
    //keyboard 'h'
    
    if (seeHands) {
        
        
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            bool isLeft        = simpleHands[i].isLeft;
            ofPoint handPos    = simpleHands[i].handPos;
            ofPoint handNormal = simpleHands[i].handNormal;
            simpleHands[i].direction;
            
            ofSetColor(0, 0, 255);
            ofDrawSphere(handPos.x, handPos.y, handPos.z,   0);
            ofSetColor(255, 255, 0);
            ofDrawArrow(handPos, handPos + 100*handNormal);
            
            for (int f=0; f<5; f++) {
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
                
                ofSetColor(0, 255, 0);
                int jointball = 5;
                ofDrawSphere(mcp.x, mcp.y, mcp.z, jointball);
                ofDrawSphere(pip.x, pip.y, pip.z, jointball);
                ofDrawSphere(dip.x, dip.y, dip.z, jointball);
                ofDrawSphere(tip.x, tip.y, tip.z, jointball);
                //
                
                
                //palm of hand
                //                ofDrawBox(handNormal,150,10,180);
                //                ofSetCylinderResolution(100, 100);
                //                ofDrawCylinder(handPos, 50, 10);
                
                ofPushMatrix();
                ofTranslate(handPos);
                //rotate the hand by the downwards normal
                ofQuaternion q;
                q.makeRotate(ofPoint(0, -1, 0), handNormal);
                ofMatrix4x4 m;
                q.get(m);
                glMultMatrixf(m.getPtr());
                
                
                //scale it to make it not a box
                ofScale(1, 0.35, 1.0);
#if (OF_VERSION_MAJOR == 0) && (OF_VERSION_MINOR < 8)
                ofBox(0, 0, 0, 60);
#else
                ofSetCylinderResolution(100, 100);
                ofDrawCylinder(0,10,0, 50, 10);
#endif
                ofPopMatrix();
                
                
                
                ////
                
                ofSetColor(255, 0, 0);
                ofSetLineWidth(20);
                ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
                ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
                ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
            }
        }
    }
}



//--------------------------------------------------------------


void LeapClass::activateGesture(){
    
    /* Leap iGesture Key
     --------------------------------
     1 = Screen Tap
     2 = Key Tap
     3 = Swipe Right
     4 = Swipe Left
     5 = Swipe Down
     6 = Swipe Up
     7 = Swipe Forward
     8 = Swipe Backward (towards yourself)
     9 = Circle Left (counter-clockwise)
     10 = Circle Right (clockwise)
     --------------------------------
     */
    
    
    msg;
    
    switch (leap.iGestures) {
        case 1:
            msg = "Screen Tap";
            msg += "\nPosition: "+ofToString(leap.screenTapPosition);
            break;
            
        case 2:
            
            //            seeHands =! seeHands;
            //            isDrawing =! isDrawing;
            
            
            msg = "Key Tap";
            msg += "\nPosition: " + ofToString(leap.keyTapPosition);
            break;
            
        case 3:
            msg = "Swipe Right";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 4:
            msg = "Swipe Left";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 5:
            msg = "Swipe Down";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 6:
            
            msg = "Swipe Up";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 7:
            msg = "Swipe Forward";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 8:
            msg = "Swipe Backwards";
            msg += "\nSwipe Speed mm/s: " + ofToString(leap.swipeSpeed) + "\nDuration sec's: "+ofToString(leap.swipeDurationSeconds);
            break;
            
        case 9:
             
            msg = "Circle Left";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);
            break;
            
        case 10:
           
            msg = "Circle Right";
            msg += "\nCircle Radius: " + ofToString(leap.circleRadius) + "\nCircle Center: "+ofToString(leap.circleCenter);
            break;
            
        default:
            msg = "Waiting for hand movement...";
            break;
    }
    
    
    
}



//--------------------------------------------------------------


void LeapClass::drawGestureInfo( float x, float y ){
    
    //    glDisable(GL_DEPTH_TEST);
    
//        ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 150);
    ofRect(0, 0, 450, 250);
    ofSetColor(255, 255, 255);
    
    ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), x, y);
    ofDrawBitmapString(msg, x, y+40);  // let's draw our message to the screen
//    ofDrawBitmapString(ofToString(floatingObj.newObjDone), x, y+60);  // let's draw our message to the screen
//    ofDrawBitmapString("spacebar " + ofToString(floatingObj.bSpacebar), x, y+80);  // let's draw our message to the screen
//    ofDrawBitmapString("add center attract " + ofToString(floatingObj.bAddCenterAttract), x, y+100);  // let's draw our message to the screen
    
    
    
//        ofDisableAlphaBlending();
    
    
    
}