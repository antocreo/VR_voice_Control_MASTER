#include "FloatingObject.h"

FloatingObject::FloatingObject(){

};


//--------------------------------------------------------------
void FloatingObject::setup() {
//    ofSetFrameRate(60);
//    ofSetVerticalSync(true);
//    ofBackground( 10, 10, 10);
    
//    camera.setPosition(ofVec3f(0, -4.f, -40.f));
//    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    
    world.setup();
    // enables mouse Pick events //
    world.enableGrabbing();
    ofAddListener(world.MOUSE_PICK_EVENT, this, &FloatingObject::mousePickEvent);
    
    world.enableCollisionEvents();
    ofAddListener(world.COLLISION_EVENT, this, &FloatingObject::onCollision);
//    world.setCamera(&camera);
    world.setGravity( ofVec3f(0, 0, 0) );
    
    
    
//    light.setPosition(camera.getPosition().x, camera.getPosition().y-14, camera.getPosition().z);
    
    mousePickIndex	= -1;
    bDrawDebug		= false;
    bRenderShapes	= true;
    bAddCenterAttract	= false;
    bSpacebar		= false;
    newObjDone      = false;
}

//--------------------------------------------------------------
void FloatingObject::update() {
    for(int i = 0; i < shapes.size(); i++) {
        bColliding[i] = false;
    }
   
    if(bSpacebar) {
        ofVec3f pov = camera.getPosition() ;
            ofVec3f diff;
        
            for(int i = 0; i < shapes.size(); i++) {
                diff = pov - shapes[i]->getPosition();
                diff *= 2.f;
                if (!bAddCenterAttract) diff *= -1.f;
                shapes[i]->applyCentralForce( diff );
            }
    }
    
    
    world.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void FloatingObject::draw() {
//    glEnable( GL_DEPTH_TEST );
    camera.begin();
    
    ofSetLineWidth(1.f);
    ofSetColor(255, 0, 200);
    if(bDrawDebug) world.drawDebug();
    
//    ofEnableLighting();
//    light.enable();
    
    if(bRenderShapes) {
       
        
        for(int i = 0; i < shapes.size(); i++) {
            if(shapes[i]->getType() == ofxBulletBaseShape::OFX_BULLET_BOX_SHAPE) {
                ofSetColor(15,197,138);
            } else {
                ofSetColor(220, 0, 220);
            }
            if(mousePickIndex == i) {
                ofSetColor(255, 0, 0);
            } else if (bColliding[i] == true) {
                if(shapes[i]->getType() == ofxBulletBaseShape::OFX_BULLET_BOX_SHAPE) {
                    ofSetColor(220, 180, 60);
                } else {
                    ofSetColor(255, 20, 50);
                }
            }
            shapes[i]->draw();
        }
    }
//    light.disable();
//    ofDisableLighting();
    
    camera.end();
    
    
//    glDisable(GL_DEPTH_TEST);
    
//    ofEnableAlphaBlending();
//    ofSetColor(0, 0, 0, 150);
//    ofRect(0, 0, 250, 120);
//    ofDisableAlphaBlending();
    
//    ofSetColor(255, 255, 255);
//    stringstream ss;
//    ss << "framerate: " << ofToString(ofGetFrameRate(),0) << endl;
//    ss << "num shapes: " << (shapes.size()+bounds.size()) << endl;
//    ss << "draw debug (d): " << ofToString(bDrawDebug, 0) << endl;
//    ss << "render shapes (r): " << ofToString(bRenderShapes, 0) << endl;
//    ss << "mouse force with spacebar: " << bSpacebar << endl;
//    ss << "force direction(f): " << bAddCenterAttract << endl;
//    ss << "add spherers (s)" << endl;
//    ss << "add boxes (b)" << endl;
//    ofDrawBitmapString(ss.str().c_str(), 10, 10);
}

//--------------------------------------------------------------
void FloatingObject::onCollision(ofxBulletCollisionData& cdata) {
   
    
    for (int i = 0; i < shapes.size(); i++) {
        if(*shapes[i] == cdata) {
            bColliding[i] = true;
        }
    }
}

//--------------------------------------------------------------
void FloatingObject::mousePickEvent( ofxBulletMousePickEvent &e ) {
    mousePickIndex = -1;
    for(int i = 0; i < shapes.size(); i++) {
        if(*shapes[i] == e) {
            mousePickIndex = i;
            mousePickPos = e.pickPosWorld;
            break;
        }
    }
}

//--------------------------------------------------------------
void FloatingObject::keyPressed(int key) {
    int ii = 0;
    
//    ofVec3f mouseLoc = camera.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
//    float rsize = ofRandom(.3, 1.8);
//    mouseLoc.z += 15;
    
    switch (key) {
        case OF_KEY_BACKSPACE:
//            if(mousePickIndex > -1) {
//                if(shapes.size() > 0) {
//                    delete shapes[mousePickIndex];
//                    shapes.erase( shapes.begin()+mousePickIndex );
//                }
//                mousePickIndex = -1;
//            }
            
            shapes.clear();
            
            
            break;
        case 'd':
            bDrawDebug = !bDrawDebug;
            break;
        case 'b':
            createBox(ofVec3f(0,100,0)+camera.getLookAtDir());
            
//            createBox(ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0));
            break;
        
        case 's':
            createSphere(ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0));

            break;
        case 'r':
//            bRenderShapes = !bRenderShapes;
            break;
        case ' ':
            bSpacebar = !bSpacebar;
            break;
        case 'F':
            bAddCenterAttract = !bAddCenterAttract;
            break;
        default:
            break;
    }
    bColliding.clear();
    for(int i = 0; i < shapes.size(); i++) {
        bColliding.push_back( false );
    }
}

//--------------------------------------------------------------
void FloatingObject::keyReleased(int key) {
    //	if(key == ' ')
    //		bSpacebar = false;
}

//--------------------------------------------------------------
void FloatingObject::mouseMoved(int x, int y) {
    
}

//--------------------------------------------------------------
void FloatingObject::createBox(ofVec3f pos) {
//    newObjDone=false;
    int ii = 0;
    
    ofVec3f mouseLoc = camera.screenToWorld( ofVec3f(pos) );
    float rsize = ofRandom(.3, 1.8);
//    mouseLoc.z += 15;

    
    shapes.push_back( new ofxBulletBox() );
    ii = shapes.size()-1;
    ((ofxBulletBox*)shapes[ii])->create(world.world, mouseLoc, rsize*.2, rsize*2, rsize*2, rsize*2);
    shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
    shapes[ii]->add();
    bColliding.push_back( false );
    newObjDone=true;
    
    
}

//--------------------------------------------------------------
void FloatingObject::createSphere(ofVec3f pos) {
//    newObjDone=false;
    int ii = 0;
    
    ofVec3f mouseLoc = camera.screenToWorld( ofVec3f(pos) );
    float rsize = ofRandom(.3, 1.8);
    
//    mouseLoc.z += 15;
    
    
    shapes.push_back( new ofxBulletSphere() );
    ii = shapes.size()-1;
    ((ofxBulletSphere*)shapes[ii])->create(world.world, mouseLoc, rsize*.2, rsize);
    shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
    shapes[ii]->add();
    bColliding.push_back( false );
    newObjDone=true;
    
}




//--------------------------------------------------------------
void FloatingObject::mouseDragged(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void FloatingObject::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void FloatingObject::mouseReleased(int x, int y, int button){
    mousePickIndex = -1;
}

//--------------------------------------------------------------
void FloatingObject::windowResized(int w, int h) {
    
}

//--------------------------------------------------------------
void FloatingObject::gotMessage(ofMessage msg) {
    
}

//--------------------------------------------------------------
void FloatingObject::dragEvent(ofDragInfo dragInfo) {
    
}