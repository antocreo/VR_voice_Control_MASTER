#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    
    // -------------GENERAL SETUP-------------//
    
    ofBackground(0);
    //    ofSetLogLevel( OF_LOG_VERBOSE );
    ofSetVerticalSync( true );
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    // -------------OCULUS SETUP-------------//
    
    
    //    ofSetWindowPosition(1920, 0);
    //    ofToggleFullscreen();
    showOverlay = false;
    predictive = false;
    
    ofHideCursor();
    
    camHeigth = 200.f;
    radius = 4000.f;
    
    
    cam.setPosition(0,camHeigth,0);
    oculusRift.baseCamera = &cam;
    oculusRift.setup();
    //    cam.setOrientation(ofVec3f(0,180,0));
    
    //enable mouse;
    cam.begin();
    cam.end();
    
    // -------------LIGHTS SETUP-------------//
    center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    
    ///---point light---///
    
    ofSetSmoothLighting(true);
    pointlight.setDiffuseColor( ofFloatColor(255, 255, 255) );
    pointlight.setSpecularColor( ofFloatColor(255.f, 255.f, 255.f));
    pointlight.setPointLight();
    pointlight.setPosition(0,radius/2,0);
    
    
    
    ///---directional light---///
    
    directionalLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setDirectional();
    // set the direction of the light
    directionalLight.setPosition(cam.getPosition());
    
    // set it pointing from left to right -> //
    directionalLight.setOrientation( ofVec3f(0, 0, 0) );
    
    
    ///---spot light---///
    
    light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    light.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    // turn the light into spotLight, emit a cone of light //
    light.setSpotlight();
    
    // size of the cone of emitted light, angle between light axis and side of cone //
    // angle range between 0 - 90 in degrees //
    light.setSpotlightCutOff( 50 );
    
    // rate of falloff, illumitation decreases as the angle from the cone axis increases //
    // range 0 - 128, zero is even illumination, 128 is max falloff //
    light.setSpotConcentration( 120 );
    light.setOrientation(ofVec3f (0,90,0));
    
    
    ///---bullet light---///
    
    lightBull.setSpotlight();
    lightBull.setPosition(cam.getPosition().x, 0, cam.getPosition().z);
    lightBull.setOrientation(ofVec3f(0,-90,0));
    
    
    
    ///-----material-----///
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 60 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    
    // -------------MFCC SETUP-------------//
    myMfcc.setup();
    
    int bufferSize		= myMfcc.initialBufferSize;
    int sampleRate 		= myMfcc.sampleRate;
    
    soundStream.setup(2, 0, sampleRate, bufferSize, 4);
    soundStream.setOutput(this);
    
    // I am just setup that mfcss will get the values of the double* mfcss // I can avoid this and using myMfcc.mfccs instead but I prefer this way.
    
    mfccs = myMfcc.mfccs;
    memset(audioData, 0, numOfVerts);
    
    
    
    // -------------TEXTURE SETUP-------------//
    
    //texture for the environment
    around.setRadius(radius);
    //    around.setMode(OF_PRIMITIVE_LINES);
    around.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    //allocationg the texture
    
    w = 500 ;
    h = 500 ;
    
    texColor.allocate(w,h,GL_RGB);
    
    colorPixels 		= new unsigned char [w * h * 3];
    
    // color pixels, use w and h to control red and green
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){
            colorPixels[(j * w + i) * 3 + 0] = i;	// r
            colorPixels[(j * w + i) * 3 + 1] = j;   // g
            colorPixels[(j * w + i) * 3 + 2] = 0;   // b
        }
    }
    
    texColor.loadData(colorPixels, w,h, GL_RGB);
    
    
    
    //-----------SHADERS SETUP-------------//
    
#ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
#endif
    
    
    //-----------PRIMITIVES SETUP----------//
    
    //floor
    plane.set(radius, radius, 50, 50, OF_PRIMITIVE_LINE_STRIP);
    plane.setPosition(0, 0, 0);
    plane.enableColors();
    plane.enableTextures();
    
    
    //    //grid
    //    grid.set(3000, 3000, 50, 50, OF_PRIMITIVE_TRIANGLES);
    //    grid.setPosition(0, 0, 0);
    
    //environment sphere
    around.enableTextures();
    around.mapTexCoordsFromTexture(texColor);
    
    
    //I am setting both the original and the warped mesh = to the plane
    
    meshOriginal = meshWarped = plane.getMesh();
    
    // -------------LEAP SETUP-------------//
    
    myLeap.setup();
    //    myLeap.leapCam = cam;
    
    
    //    myLeap.leap.setVRController();
    
    myLeap.leapCam.setPosition(ofVec3f(0, camHeigth, 1500.f));
    myLeap.leapCam.setVFlip(true);
    myLeap.leapCam.tilt(-10);
    myLeap.leapCam.setScale(.50);
    
    
    
    // -------------BULLET SETUP-------------//
    
    
    
    //setting up floating obj
    
    thisWorld.setup();
    
    worldBullDist = ofRandom(10,80); //distance of the object from your face;
    
    
    // enables mouse Pick events //
    thisWorld.enableGrabbing();
    ofAddListener(thisWorld.MOUSE_PICK_EVENT, this, &testApp::mousePickEvent);
    
    thisWorld.enableCollisionEvents();
    ofAddListener(thisWorld.COLLISION_EVENT, this, &testApp::onCollision);
    thisWorld.setCamera(&cam);
    thisWorld.setGravity( ofVec3f(0, 0, 0) );
    thisWorld.enableDebugDraw();
    
    
    //floor
    myFloor = shared_ptr< ofxBulletTriMeshShape >( new ofxBulletTriMeshShape() );
    myFloor->create( thisWorld.world, meshWarped, ofVec3f(0,0,0), 0.f, ofVec3f(-10000, -10000, -10000), ofVec3f(10000,10000,10000) );
    //    myFloor->add();
    myFloor->setProperties(.25, .95);
    myFloor->enableKinematic();
    myFloor->setActivationState( DISABLE_DEACTIVATION );
    
    
    //outer sphere
    sky.create(thisWorld.world, ofVec3f(0, 0, 0),0., radius);
    sky.setProperties(.25, .95);
    sky.enableKinematic();
    sky.setActivationState( DISABLE_DEACTIVATION );
    
    
    
    //objects
    
    
    populateObjects();
    
    
    
    
    //Boolean vars for Physics
    mousePickIndex	= -1;
    bDrawDebug		= false;
    bRenderShapes	= true;
    bAddCenterAttract	= true;
    bSpacebar		= false;
    objDone         = false;
    
    
}


//--------------------------------------------------------------
void testApp::update()
{
    //-----------MFCC UPDATE-----------//
    
    
    // I am just updating that mfcss will get the values of the double* mfcss // I can avoid this and using myMfcc.mfccs instead but I prefer this way.
    myMfcc.update();
    mfccs = myMfcc.mfccs;
    
    //-----------OCULUS UPDATE-----------//
    
    
    
    //    cam.setPosition(0,camHeigth,0);
    //    cam.setOrientation(oculusRift.getOrientationQuat());
    
    
    
    
    //-----------LIGHTS UPDATE-----------//
    
    
    // set the direction of the light
    directionalLight.setPosition(cam.getPosition());
    
    // set it pointing from left to right -> //
    directionalLight.setOrientation( oculusRift.worldToScreen(center) );
    
    
    
    //-----------TEXTURE variation UPDATE-----------//
    
    
    /// texture mfcc
    
    float newCol = ofLerp(0, 1, myMfcc.distancentroids2);
    
    
    
    if (myMfcc.mfccs[0]>1.2) {
        
        // color pixels, use w and h to control red and green
        for (int i = 0; i < w; i++){
            for (int j = 0; j < h; j++){
                colorPixels[(j * w + i) * 3 + 0] = i  ;	// r
                colorPixels[(j * w + i) * 3 + 1] = j ; // g
                colorPixels[(j * w + i) * 3 + 2] = (unsigned char) (newCol * 255 );   // b
                
            }
        }
    } else {
        
        newCol = 0;
    }
    
    
    
    // finally, load those pixels into the texture
    texColor.loadData(colorPixels, w,h, GL_RGB);
    
    
    
    
    
    //-----------DISPLACEMENT UPDATE-----------//
    
    vector<ofVec3f> & vertsOriginal = meshOriginal.getVertices();
    vector<ofVec3f> & vertsWarped = meshWarped.getVertices();
    
    
    numOfVerts = meshOriginal.getNumVertices();
    
    audioData = new double[numOfVerts];
    
    for (int i=0; i<numOfVerts; i++) {
        if (myMfcc.mfccs[0]>1.2 && myMfcc.mfccs[i]>0 && rms>0) {
            audioData[i] = mfccs[i%myMfcc.nAverages] ;
        } else {
            audioData[i] = 0;
            
            meshWarped=meshOriginal;
            
        }
    }
    
    
    
    //    float meshDisplacement = myMfcc.distancentroids2;
    float meshDisplacement = (myMfcc.centroid1.x + myMfcc.centroid1.y) * myMfcc.distancentroids1;
    // smoother alternative
    //    float meshDisplacement = (myMfcc.centroid1.x * myMfcc.centroid1.y) + myMfcc.distancentroids1;
    
    
    for(int i=0; i<numOfVerts; i++) {
        float audioValue = audioData[i];
        ofVec3f & vertOriginal = vertsOriginal[i];
        ofVec3f & vertWarped = vertsWarped[i];
        ofVec3f volume = ofVec3f(rms);
        ofVec3f direction = vertOriginal.getNormalized();
        vertWarped = vertOriginal + direction * meshDisplacement * audioValue + volume;
    }
    myFloor->updateMesh(thisWorld.world, meshWarped);
    
    
    delete[] audioData;
    
    // -------------LEAP UPDATE-------------//
    
    myLeap.update();
    
    if (objDone==true) {
        myLeap.leap.iGestures=0;
        objDone=false;
    }
    
    
    if (myLeap.leap.iGestures==9 && objDone==false) {
        
        createBox();
    }
    
    if (myLeap.leap.iGestures==10  && objDone==false) {
        createSphere();
    }
    
    if (myLeap.leap.iGestures==4 || myLeap.leap.iGestures==3) {
        for (int i = 0; i<shapes.size(); i++) {
            shapes[i]->applyCentralForce(0,myLeap.leap.swipeSpeed,0);
        }
    }
    
    if (myLeap.leap.iGestures==5 || myLeap.leap.iGestures==6) {
        
        if(shapes.size() > 0) {
            shapes.clear();
            bounds.clear();
        }
    }
    
    
    // -------------BULLET UPDATE-------------//
    
    
    
    for(int i = 0; i < shapes.size(); i++) {
        bColliding[i] = false;
    }
    
    
    //--- attraction to center ---//
    
    if(bSpacebar) {
        ofVec3f loc = cam.screenToWorld(center);
        loc.z += 15;
        ofVec3f diff;
        for(int i = 0; i < shapes.size(); i++) {
            diff = loc - shapes[i]->getPosition();
            diff *= .5f;
            if (!bAddCenterAttract) diff *= -.5f;
            shapes[i]->applyCentralForce( diff );
        }
    }
    
    //--- voice interaction ---//
    
    voicePush();
    
    
    thisWorld.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
    
    
}


//--------------------------------------------------------------
void testApp::draw()
{
    //this draw the mfcc on the stage but not visible in OR
    
    if(oculusRift.isSetup()){
        
        
        myMfcc.draw();
        
        
        if(showOverlay){
            
            ///////// info overlay oculus ///////
            
            oculusRift.beginOverlay(-400, 450,450);
            
            ofRectangle overlayRect = oculusRift.getOverlayRectangle();
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(255, 40, 10, 200);
            ofRect(overlayRect);
            ofSetColor(255,255);
            ofFill();
            //            ofScale(5, 5);
            //            ofTranslate(-30, -10);
            myLeap.drawGestureInfo(40, 40);
            
            
            
            ofDrawBitmapStringHighlight("dist2 " + ofToString(myMfcc.distancentroids2), 40,180);
            ofDrawBitmapStringHighlight("dist1 " + ofToString(myMfcc.distancentroids1), 40,160);
            ofDrawBitmapStringHighlight("active force" + ofToString(bAddCenterAttract), 40,140);
            ofDrawBitmapStringHighlight("attract to center " + ofToString(bSpacebar), 40,120);
            ofDrawBitmapStringHighlight("num shapes " + ofToString(shapes.size()), 40,100);
            
            //            ofDrawBitmapStringHighlight("cam pos " + ofToString(cam.getOrientationQuat()), 40,100);
            //            ofDrawBitmapStringHighlight("oculus pos " + ofToString(oculusRift.getOrientationQuat()), 40,60);
            
            
            
            
            //            ofDrawBitmapStringHighlight("cam pos " + ofToString(oculusRift.getOrientationQuat()), 40,160);
            
            
            
            ofDrawBitmapStringHighlight("rms " + ofToString(rms), 40,200);
            for (int i=0; i<myMfcc.nAverages; i++) {
                ofDrawBitmapStringHighlight("mfccs " + ofToString(mfccs[i]), 40,220+15*i);
            }
            
            
            ofSetColor(0, 255, 0);
            ofNoFill();
            ofCircle(overlayRect.getCenter(), 20);
            ofDisableAlphaBlending();
            ofPopStyle();
            oculusRift.endOverlay();
        }
        ///---->end overlay---
        
        //drawing the info for each eye
        ofSetColor(255);
        glEnable(GL_DEPTH_TEST);
        
        
        oculusRift.beginLeftEye();
        drawScene();
        oculusRift.endLeftEye();
        
        oculusRift.beginRightEye();
        drawScene();
        oculusRift.endRightEye();
        
        oculusRift.draw();
        
        
        glDisable(GL_DEPTH_TEST);
        
    }
    //if Oculus not setup
    else{
        glEnable(GL_DEPTH_TEST);
        
        
        cam.begin();
        drawScene();
        cam.end();
        
        glDisable(GL_DEPTH_TEST);
        
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::drawScene()
//here is the scene to be drawn in Oculus

{
    
    //    ofDrawAxis(500);
    
    //////----- LIGHTS DRAW -------//////
    
    
    //LIGHTS ON//
    ofEnableLighting();
    material.begin();
    pointlight.enable();
    directionalLight.enable();
    light.enable();
    //    lightBull.enable();
    
    
    // environment textured icosphere
    
    ofPushStyle();
    ofSetColor(0,0,0,0);
    texColor.bind();
    sky.draw(); //-->this is the solid environment
    ofPopStyle();
    
    texColor.bind();
    ofPushStyle();
    around.draw(); //---> this is the textured sphere
    ofPopStyle();
    texColor.unbind();
    
    
    //solid plane    //normal plane - this is the solid plane - with shaders
    ofPushMatrix();
    ofRotate(90, 0, 0, 0);
    shader.begin();
    plane.draw();
    shader.end();
    ofPopMatrix();
    
    
    //displacement plane - this is the plane that actually get displaced
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(0, 5);
    ofRotate(90, 0, 0, 0);
    ofSetColor(200,200,200,255);
    ofFill();
    meshWarped.drawWireframe();
    ofPopStyle();
    ofPopMatrix();
    
    
    
    //////----- FLOATING OBJECTS DRAW -------//////
    
    
    
    ofPushMatrix();
    ofPushStyle();
    ofSetLineWidth(1.f);
    ofSetColor(255, 0, 200);
    if(bDrawDebug) thisWorld.drawDebug();
    
    
    if(bRenderShapes) {
        
        ofSetColor(100, 100, 100,50);
        for(int i = 0; i < bounds.size(); i++) {
            bounds[i]->draw();
        }
        
        for(int i = 0; i < shapes.size(); i++) {
            if(shapes[i]->getType() == ofxBulletBaseShape::OFX_BULLET_BOX_SHAPE) {
                ofSetColor(200,200,0);
            } else {
                ofSetColor(0, 200, 200);
            }
            if(mousePickIndex == i) {
                ofSetColor(255, 255, 255);
            } else if (bColliding[i] == true) {
                if(shapes[i]->getType() == ofxBulletBaseShape::OFX_BULLET_BOX_SHAPE) {
                    ofSetColor(250, 0, 0);
                } else {
                    ofSetColor(255, 20, 50);
                }
            }
            shapes[i]->draw();
            
        }
        
        
    }
    
    
    ofPopStyle();
    ofPopMatrix();
    
    
    //-----------------------------------------------//
    
    
    
    //////----- LEAP HANDS DRAW -------//////
    
    ofPushMatrix();
    ofPushStyle();
    //        myLeap.leap.setReceiveBackgroundFrames(true);
    myLeap.draw();
    ofPopStyle();
    ofPopMatrix();
    
    //-------------------------------------------------//
    
    
    
    
    //LIGHTS OFF//
    ofDisableLighting();
    material.end();
    pointlight.disable();
    directionalLight.disable();
    light.disable();
    //    lightBull.disable();
    
    
    
    // cursor
    ofPushStyle();
    ofNoFill();
    //billboard and draw the mouse
    if(oculusRift.isSetup()){
        
        ofPushMatrix();
        oculusRift.multBillboardMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(0,0,.5);
        ofPopMatrix();
    }
    
    
    ofPopStyle();
    
    
    
    pointlight.draw();
    //    light.draw();
    //     directionalLight.draw();
    
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    myLeap.keyPressed(key);
    
    
    if( key == 'f' )
    {
        //gotta toggle full screen for it to be right
        ofToggleFullscreen();
    }
    
    if(key == 'k'){
        oculusRift.reloadShader();
    }
    
    if(key == 'l'){
        oculusRift.lockView = !oculusRift.lockView;
    }
    
    if(key == 'o'){
        showOverlay = !showOverlay;
    }
    if(key == 'r'){
        //resetting the camera and oculus at the normal pos.
        cam.resetTransform();
        oculusRift.reset();
        cam.setPosition(0,camHeigth,0);
    }
    if(key == 'h'){
        ofHideCursor();
    }
    if(key == 'H'){
        ofShowCursor();
    }
    
    if(key == 'P'){
        predictive = !predictive;
        oculusRift.setUsePredictedOrientation(predictive);
    }
    
    
    //----BULLET KEY commands----//
    
    switch (key) {
        case OF_KEY_BACKSPACE:
            if(shapes.size() > 0) {
                shapes.clear();
                bounds.clear();
            }
            
            break;
        case 'd':
            bDrawDebug = !bDrawDebug;
            break;
        case 'b':
            createBox();
            
            break;
        case 's':
            createSphere();
            break;
        case 'R':
            bRenderShapes = !bRenderShapes;
            break;
        case ' ':
            bSpacebar = !bSpacebar;
            break;
        case 'v':
            bAddCenterAttract = !bAddCenterAttract;
            break;
            
        case 'p':
            if(shapes.size() > 0) {
                shapes.clear();
                bounds.clear();
            }
            
            populateObjects();
            
        case 'z':
            for(int i = 0; i < shapes.size(); i++) {
                
                //this should freeze the objects but it doesn't work...?!
                shapes[i]->setDamping(0, 0);
                shapes[i]->setActivationState(0);
            }
            
        default:
            break;
    }
    bColliding.clear();
    for(int i = 0; i < shapes.size(); i++) {
        bColliding.push_back( false );
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    myLeap.keyReleased(key);
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
    
}
//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
    myMfcc.audioRequested(output,bufferSize,nChannels);
    
    
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
    
    myMfcc.audioReceived(input,bufferSize,nChannels);
    
    rms = myMfcc.rms * 100;
    
    
}
//--------------------------------------------------------------
void testApp::onCollision(ofxBulletCollisionData& cdata) {
    for(int j = 0; j < bounds.size(); j++) {
        if(*bounds[j] == cdata) {
            return;
        }
    }
    
    for (int i = 0; i < shapes.size(); i++) {
        if(*shapes[i] == cdata) {
            bColliding[i] = true;
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePickEvent( ofxBulletMousePickEvent &e ) {
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

void testApp::exit() {
}

//--------------------------------------------------------------


void testApp::populateObjects(){
    
    
    int ii = 0;
    // generating spheres //
    sphereShape = ofBtGetSphereCollisionShape(ofRandom(1,5)); //here is the radius
    for (int i = 0; i < ofRandom(3, 10); i++) {
        shapeBirthLoc.set(ofRandom(-radius/100, radius/100), camHeigth, ofRandom(-5, 5)-worldBullDist); //position
        shapes.push_back( new ofxBulletSphere() );
        ii = shapes.size()-1;
        ((ofxBulletSphere*)shapes[ii])->init(sphereShape); //
        ((ofxBulletSphere*)shapes[ii])->create(thisWorld.world,shapeBirthLoc , ofRandom (1,2)); //create world, position, mass
        
        shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
        shapes[ii]->add();
        bColliding.push_back( false );
    }
    
    // adding some boxes //
    boxShape = ofBtGetBoxCollisionShape(ofRandom(1,5), ofRandom(1,5), ofRandom(1,5)); //dimension of the boxes
    for (int i = 0; i < ofRandom(3,10); i++) {
        shapeBirthLoc.set(ofRandom(-radius/100, radius/100), camHeigth, ofRandom(-5, 5)-worldBullDist); //position
        shapes.push_back( new ofxBulletBox() );
        ii = shapes.size()-1;
        ((ofxBulletBox*)shapes[ii])->init(boxShape);
        ((ofxBulletBox*)shapes[ii])->create(thisWorld.world, shapeBirthLoc, ofRandom(1,2)); //create world, position, mass
        
        shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
        shapes[ii]->add();
        bColliding.push_back( false );
    }
    
    
    //generating some random walls
    ofVec3f startLoc;
    ofPoint dimens;
    boundsWidth = 100.;
    float hwidth = boundsWidth*.5;
    float depth = 2. ;
    float hdepth = depth*.5 * ofRandom(camHeigth);
    
    for(int i = 0; i < 6; i++) {
        bounds.push_back( new ofxBulletBox() );
        if(i == 0) { // ground //
            startLoc.set( 0., hwidth+hdepth, 0. );
            dimens.set(boundsWidth, depth, boundsWidth);
        } else if (i == 1) { // back wall //
            startLoc.set(0, 0, hwidth+hdepth);
            dimens.set(boundsWidth, boundsWidth, depth);
        } else if (i == 2) { // right wall //
            startLoc.set(hwidth+hdepth, 0, 0.);
            dimens.set(depth, boundsWidth, boundsWidth);
        } else if (i == 3) { // left wall //
            startLoc.set(-hwidth-hdepth, 0, 0.);
            dimens.set(depth, boundsWidth, boundsWidth);
        } else if (i == 4) { // ceiling //
            startLoc.set(0, hwidth-hdepth, 0.);
            dimens.set(boundsWidth, depth, boundsWidth);
        } else if (i == 5) { // front wall //
            startLoc.set(0, 0, -hwidth-hdepth);
            dimens.set(boundsWidth, boundsWidth, depth);
        }
        
        
        
        bounds[i]->create( thisWorld.world, startLoc, 0.,ofRandom(dimens.x),ofRandom(dimens.y),ofRandom(dimens.z) );
        bounds[i]->setProperties(.25, .95);
        bounds[i]->add();
    }
    
    
    
}
//--------------------------------------------------------------


void testApp::createBox(){
    
    int ii = 0;
    
    //        ofVec3f objLoc = cam.screenToWorld(center);
    ofVec3f objLoc = oculusRift.getOrientationQuat().getEuler();
    float rsize = ofRandom(.3, 1.8);
    //          objLoc.z += 15;
    
    
    
    
    shapeBirthLoc.set(ofRandom(-20,20) , camHeigth, objLoc.z  - worldBullDist);
    shapes.push_back( new ofxBulletBox() );
    ii = shapes.size()-1;
    ((ofxBulletBox*)shapes[ii])->create(thisWorld.world, shapeBirthLoc, rsize*.2, rsize*2, rsize*2, rsize*2);
    shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
    shapes[ii]->add();
    bColliding.push_back( false );
    objDone = true;
    
    
}

//--------------------------------------------------------------


void testApp::createSphere(){
    int ii = 0;
    
    //        ofVec3f objLoc = cam.screenToWorld(center);
    ofVec3f objLoc = cam.getPosition();
    float rsize = ofRandom(.3, 1.8);
    //          objLoc.z += 15;
    
    
    shapeBirthLoc.set(ofRandom(-radius/100, radius/100), camHeigth, ofRandom(-5, 5) - worldBullDist);
    shapes.push_back( new ofxBulletSphere() );
    ii = shapes.size()-1;
    ((ofxBulletSphere*)shapes[ii])->create(thisWorld.world, shapeBirthLoc, rsize*.2, rsize);
    shapes[ii]->setActivationState( DISABLE_DEACTIVATION );
    shapes[ii]->add();
    bColliding.push_back( false );
    objDone = true;
    
    
    
}

//--------------------------------------------------------------


void testApp::voicePush(){
    
    
    ////---- BULLET interaction with voice --- /////
    
    for(int i = 0; i < shapes.size(); i++) {
        if (myMfcc.mfccs[0]>1.2 /*&& myMfcc.mfccs[i]>0 && rms>0*/) {
            
            ofVec3f dir(shapes[i]->getPosition() - cam.screenToWorld(center));
            
            
            //            shapes[i]->applyCentralForce(0,0,-shapes[i]->getPosition().y);
            shapes[i]->applyCentralForce(dir+myMfcc.distancentroids1+rms);
            //            shapes[i]->applyCentralForce(dir*myMfcc.distancentroids1+rms);
            
            
        }
    }
}


