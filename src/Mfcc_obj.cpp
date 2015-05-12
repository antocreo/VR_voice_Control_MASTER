//
//  Mfcc_obj.cpp
//  OculusRenderingBasic
//
//  Created by Let it Brain on 28/03/2015.
//
//


#include "Mfcc_obj.h"
#include "maximilian.h"/* include the lib */
#include "time.h"

//-------------------------------------------------------------
Mfcc_obj::Mfcc_obj() {
    
}

//-------------------------------------------------------------
Mfcc_obj::~Mfcc_obj() {
    
}


//--------------------------------------------------------------
void Mfcc_obj::setup(){
    /* some standard setup stuff*/
    
    //    ofEnableAlphaBlending();
    //    ofSetupScreen();
    //    ofBackground(0, 0, 0);
    //    ofSetFrameRate(60);
    //    ofSetVerticalSync(true);

    
    /* This is stuff you always need.*/
    
    sampleRate 			= 44100; /* Sampling Rate */
    initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
    lAudioOut			= new float[initialBufferSize];/* outputs */
    rAudioOut			= new float[initialBufferSize];
    lAudioIn			= new float[initialBufferSize];/* inputs */
    rAudioIn			= new float[initialBufferSize];
    
    
    /* This is a nice safe piece of code */
    memset(lAudioOut, 0, initialBufferSize * sizeof(float));
    memset(rAudioOut, 0, initialBufferSize * sizeof(float));
    
    memset(lAudioIn, 0, initialBufferSize * sizeof(float));
    memset(rAudioIn, 0, initialBufferSize * sizeof(float));
    
    
    
    fftSize = 1024;
    mfft.setup(fftSize, 512, 256);
    ifft.setup(fftSize, 512, 256);
    
    
    
    nAverages = 13;
    oct.setup(sampleRate, fftSize/2, nAverages);
    
    mfccs = (double*) malloc(sizeof(double) * nAverages);
    vowel = (double*) malloc(sizeof(double) * nAverages);
    
    mfcc.setup(512, 56, nAverages, 20, 20000, sampleRate);
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    //    ofSoundStreamSetup(2,2, this, sampleRate, initialBufferSize, 4);/* Call this last ! *///this is the original one contains "this" that is not accepted here.... why???
    //i repleaced it with this line.
    ofSoundStreamSetup(2,2, sampleRate, initialBufferSize, 4);/* Call this last ! */
    
    //
    
    mousePos.set(ofGetMouseX(), ofGetMouseY());
    
    //setting boolean all to false
    recording, recordingEnded = false;
    recordingA,recordingE,recordingI,recordingO,recordingU =false;
    
    
    
    //GL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    
    ofVec3f dummyPos(0,0,0);
    for (int i=0; i<nAverages; i++) {
        mesh.addVertex(dummyPos);
        mesh2.addVertex(dummyPos);
    }
    
    //    strip.getMesh() = meshWarped;
    //    = ofMesh::sphere(200,300);
    
    
    centroid2 = mesh2.getCentroid();
    centroid1 = mesh.getCentroid();


    
}

//--------------------------------------------------------------
void Mfcc_obj::update(){
    
    
    for (int i=0; i<meshPoints.size(); i++) {
        if (mfccs[0]>1.2  ) {
            
            
            //this is the displacement factor vector that I will multiply on the original vector strip.getMesh.getVert
            displacementVec.set(mfccs[i+0], mfccs[i+1], mfccs[i+2]);
            
            //once I get the displacement vec I need to multiply (getCrossed) with the original vector.getMesh.getVert
            
            ofVec3f rmsvec(rms,rms,rms);
            
            meshPoints[i] = (meshPoints[i] + displacementVec + rms);
            //            meshPoints[i] +=  displacementVec.getCrossed(rmsvec);
            
            //            meshPoints[i] = meshPoints[i] + mesh.getVertex(i);
            //            meshPoints[i] =  meshPoints[i].interpolate((meshPoints[i].getCrossed(displacementVec)),1);
            
            newCentr1 = centroid1;
            newCentr2 = centroid2;
            
            
        } else  {
            
            meshPoints[i] = originalMesh[i];
            
            
        }
    }
    
}

//--------------------------------------------------------------
void Mfcc_obj::draw(){
    
    
    
    //grid
    //    ofPushStyle();
    //    ofSetColor(100, 0, 0,100);
    //    ofDrawGrid(ofGetHeight(), 50.0f, false, false, false, true);
    //    ofPopStyle();
    //end grid
    
    
    //show bitmap info
    //    bitmapInfo();
    
    
    // draw MFcc bar chart
    //     drawMfccBars( 10, 100);
    
    
    // draw RMS
    //    drawRms(10, 50);
    
    
        
    
    /*---------------------------*/
    
    
    //polygon and centroid
    if (mfccs[0]>1.2) {
        
        distancentroids1 = centroid1.distance(newCentr1);
        distancentroids2 = centroid2.distance(newCentr2);


        
        // doodle lines
        ofPushStyle();
        ofNoFill();
        ofPushMatrix();
        mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        
        int scaleFactor = 50;
        for (int i=0; i<nAverages; i++) {
            mesh.addVertex(ofVec2f(mfccs[i+1]*scaleFactor,mfccs[i]*scaleFactor));
            mesh.addColor(255);
        }
        
        mesh.draw();
        
        //centroid
        centroid1 = mesh.getCentroid();
        ofDrawBitmapStringHighlight("centroid1 - " + ofToString( centroid1 ), 100,70);
        
        ofSetColor(255,0,0);
        ofCircle(centroid1.x, centroid1.y, mfccs[0]*2);
        mesh.clear();
        ofPopMatrix();
        ofPopStyle();
        
        /*---------------------------*/
        
        //graph
        ofPushStyle();
        ofPushMatrix();
        //        mesh2.setMode(OF_PRIMITIVE_LINE_LOOP);
        //        mesh2.setMode(OF_PRIMITIVE_TRIANGLE_STRIP   );
        mesh2.setMode(OF_PRIMITIVE_TRIANGLE_FAN   );
        
        
        ofTranslate(0, ofGetHeight()/2-100);
        
        int scaleFactor2 = 30;
        for (int i=0; i<nAverages; i++) {
            mesh2.addVertex(ofVec2f(mfccs[0]*scaleFactor2*i,mfccs[i]*scaleFactor2));
            mesh2.addColor(255);
        }
        //        mesh2.addVertex(ofVec2f(mfccs[nAverages]*scaleFactor2,mfccs[0]*scaleFactor2*nAverages));
        
        mesh2.draw();
        
        //centroid
        centroid2 = mesh2.getCentroid();
        ofDrawBitmapStringHighlight("centroid2 - " + ofToString( centroid2 ), 100,70);
        
        ofSetColor(255,0,0);
        ofSetCircleResolution(100);
        ofCircle(centroid2.x, centroid2.y, mfccs[0]*scaleFactor/4);
        mesh2.clear();
        ofPopMatrix();
        ofPopStyle();
        
        
    }
    
    
    
    /*---------------------------*/
    
    
    glEnable(GL_DEPTH_TEST);
    
    for(int i = 0; i < meshPoints.size(); i++){
        ofDrawBitmapStringHighlight(ofToString( meshPoints.size() ), 10,50);
        ofDrawBitmapStringHighlight(ofToString( sizeof(mfccs) ), 10,80);
        ofDrawBitmapStringHighlight(ofToString( ofToString( mfccs[i] ) ), 10,110);
        ofDrawBitmapStringHighlight(ofToString( ofToString( displacementVec ) ), 10, 140);
        ofDrawBitmapStringHighlight(ofToString( ofToString( mesh.getNumVertices() ) ), 10, 160);
        
        glDisable(GL_DEPTH_TEST);
        
        
        strip.generate(meshPoints, 5+mfccs[i%nAverages], ofPoint (0,0.5,0.5) );
        
        float colorvoice = 100*(meshPoints.size()%255 * mfccs[i%nAverages]);
        
        ofSetColor(200, 0,0 );
        
        //        ofQuaternion orientation(90, 0, 0, 0);
        //        cam.setOrientation(orientation);
        //        cam.setTarget(meshPoints[i]);
        //        cam.setPosition(cam.worldToScreen(meshPoints[i]));
        
        
    }
    //        cam.begin();
    strip.getMesh().draw();
    //        cam.end();
    
    
}

//--------------------------------------------------------------
void Mfcc_obj::audioRequested 	(float * output, int bufferSize, int nChannels){
    //	static double tm;
    for (int i = 0; i < bufferSize; i++){
        wave = lAudioIn[i];
        //get fft
        if (mfft.process(wave)) {
            
            mfcc.mfcc(mfft.magnitudes, mfccs);
            
            //cout << mfft.spectralFlatness() << ", " << mfft.spectralCentroid() << endl;
        }
        
    }
    
    getVowel();
    
}

//--------------------------------------------------------------
void Mfcc_obj::audioReceived 	(float * input, int bufferSize, int nChannels){
    
    
    //rms
    //    calculate a rough volume
    
    /* You can just grab this input and stick it in a double, then use it above to create output*/
    
    for (int i = 0; i < bufferSize; i++){
        
        
        /* you can also grab the data out of the arrays*/
        
        lAudioIn[i] = input[i*2];
        rAudioIn[i] = input[i*2+1];
        
        //        cout << lAudioIn[i] << endl;
        
        
        rms =sqrt( (lAudioIn[i]*lAudioIn[i])+(lAudioIn[i]*lAudioIn[i])/bufferSize );
        
//        cout << rms << endl;
        
    }
    
    
}

//--------------------------------------------------------------
void Mfcc_obj::getRms(){
    
    
    // alternative code
    
    rms = 0.0;
    int numCounted = 0;
    
    for (int i = 0; i < initialBufferSize; i++){
        float leftSample = lAudioIn[i] * 0.5;
        float rightSample = rAudioIn[i] * 0.5;
        
        rms += leftSample * leftSample;
        rms += rightSample * rightSample;
        numCounted += 2;
    }
    
    rms /= (float)numCounted;
    rms = sqrt(rms);
    // rms is now calculated
    
    
    
}
//--------------------------------------------------------------
void Mfcc_obj::getVowel(){
    
    
    //checking that the coeff[0] > 0 and therefore there is someone speaking
    //until there is voice assign the values of mfcc to vowel
    
    for (int i=0; i<nAverages; i++) {
        if (mfccs[0]>=1.2) {
            vowel[i]=mfccs[i];
            storing.push_back(mfccs[i]);
            
            //            vowel.push_back(mfccs[i]);
            
        }
    }
    
}

//--------------------------------------------------------------

void Mfcc_obj::drawMfccBars( float x, float y){
    
    ofPushMatrix();
    ofTranslate( x, y);
    
    
    //	cout << "\nMFCCS: ";
    ofSetColor(255, 0, 0,100);
    float xinc = 900.0 / nAverages;
    for(int i=0; i < nAverages; i++) {
        float height = mfccs[i]*250;
        height = ofClamp(height, 0, 250);
        ofRect(10 + (i*xinc),600 - height,40, height);
        //		cout <<  mfccs[i] << ",";
        
        if (vowel[0]>1) {
            //            ofRect(ofGetWidth()/2, ofGetHeight()/2, 100, 100);
        }
        
    }
    //	cout << "\n-------" << endl;
    
    //	cout << callTime << endl;
    ofPopMatrix();
}

//--------------------------------------------------------------


void Mfcc_obj::drawRms(float x, float y){
    
    // rms peak
   	for(int i=0; i < initialBufferSize; i++) {
        
        ofRect(x, y, rms*1000.0f, 100);
        
    }
}

//--------------------------------------------------------------


void Mfcc_obj::bitmapInfo(){
    
    /// bitmap info
    ofPushStyle();
    
    for(int i=0; i < nAverages; i++) {
        ofDrawBitmapStringHighlight("rms " + ofToString( rms ),10,30);
        
        ofDrawBitmapStringHighlight("mfcc elem [" +ofToString(i) +"] " + ofToString( mfccs[i] ),300,50+20*i);
        
        //        ofDrawBitmapStringHighlight("vowel elem [" +ofToString(i) +"] " + ofToString( vowel[i] ),10,50+20*i);
        //        if (storing.size()>0) {
        //            ofDrawBitmapStringHighlight("storing elem [" +ofToString(i) +"] " + ofToString( storing[i] ),10,50+20*i);
        //            ofDrawBitmapStringHighlight("storing size" + ofToString(storing.size()),10,50+20*i+20);
        //        }
        
        if (mfccComponent.size()>0) {
            ofDrawBitmapStringHighlight("component elem [" +ofToString(i) +"] " + ofToString( mfccComponent[i] ),10,50+20*i);
            ofDrawBitmapStringHighlight("component size" + ofToString(mfccComponent.size()),10,50+20*i+20);
        }
    }
    
    ofPopStyle();
}



//--------------------------------------------------------------
void Mfcc_obj::tempStorage(vector<double> tempVec){
    
}


//--------------------------------------------------------------
void Mfcc_obj::keyPressed(int key){
    
    
    if (key == ' ') {
        recording = ! recording;
        if (recording==true) {
            
        }
    }
    
    
    if (key ==OF_KEY_BACKSPACE) {
        meshPoints.clear();
        polyMesh.clear();
    }
    
    
}

//--------------------------------------------------------------
void Mfcc_obj::keyReleased(int key){
    
}

//--------------------------------------------------------------
void Mfcc_obj::mouseMoved(int x, int y ){
    
    ofVec3f mouseP(x,y);
    if (x>1 && y >1) {
        meshPoints.push_back(mouseP);
        originalMesh = meshPoints;
        
        
    }
    
    
    
    
}

//--------------------------------------------------------------
void Mfcc_obj::mouseDragged(int x, int y, int button){
    
    //    ofVec3f mouseP(x,y);
    //    if (x>1 && y >1) {
    //        meshPoints.push_back(mouseP);
    //        originalMesh = meshPoints;
    //
    //
    //    }
    
}

//--------------------------------------------------------------
void Mfcc_obj::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void Mfcc_obj::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void Mfcc_obj::windowResized(int w, int h){
    
}
//--------------------------------------------------------------
void Mfcc_obj::eucliDistanc(){
    
    /*euclidean distance
     the first value of the mfcc od actually the pwer of the voice. if I say that mfcc[0]< some value (i need to map it) the I can start detecting the single value)
     
     */
    
    //    for(int i=0; i < nAverages; i++) {
    //   eucliDist = sqrt(mfcc[i]     )
    //    
    //    }
    
    
}


