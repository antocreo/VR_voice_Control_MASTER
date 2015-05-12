//
//  Mfcc_obj.h
//  OculusRenderingBasic
//
//  Created by Let it Brain on 28/03/2015.
//
//

#ifndef __OculusRenderingBasic__Mfcc_obj__
#define __OculusRenderingBasic__Mfcc_obj__

#include <stdio.h>


#include "ofMain.h"
#include "ofxMaxim.h"
#include <sys/time.h>
#include "maxiMFCC.h"
#include "ofxStrip/ofxStrip.h"


class Mfcc_obj: public ofBaseSoundOutput {
    
public:
    Mfcc_obj();
    ~Mfcc_obj();/* deconsructor is very useful */
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
    
    void getVowel();
    void bitmapInfo();
    void drawMfccBars( float x, float y);
    void drawRms(float x, float y);
    
    void eucliDistanc();
    void getRms();
    void tempStorage(vector<double>tempVec);
    
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
    
    float 	* lAudioOut; /* outputs */
    float   * rAudioOut;
    
    float * lAudioIn; /* inputs */
    float * rAudioIn;
    
    int		initialBufferSize; /* buffer size */
    int		sampleRate;
    
    
    /* stick you maximilian stuff below */
    
    double wave,sample,outputs[2], ifftVal;
    maxiMix mymix;
    maxiOsc osc;
    
    ofxMaxiFFTOctaveAnalyzer oct;
    int nAverages;
    float *ifftOutput;
    int ifftSize;
    
    ofxMaxiIFFT ifft;
    ofxMaxiFFT mfft;
    int fftSize;
    int bins, dataSize;
    
    float callTime;
    timeval callTS, callEndTS;
    
    maxiMFCC mfcc;
    double *mfccs;
    
    
    ofVec3f centroid1;
    ofVec3f centroid2;
    
    ofVec3f newCentr1;
    ofVec3f newCentr2;
    
    double distancentroids1, distancentroids2 ;
    
    double eucliDist;
    double rms;
    
    double *vowel;
    
    vector<double> storing;
    vector<int> mfccComponent;
    
    vector <double>aVow;
    vector <double>eVow;
    vector <double>iVow;
    vector <double>oVow;
    vector <double>uVow;
    
    bool recording, recordingEnded;
    bool recordingA,recordingE,recordingI,recordingO,recordingU;
    
    vector<ofVec3f>meshPoints;
    vector<ofVec3f>originalMesh;
    ofVec3f displacementVec;
    
    
    
    ofPolyline shape;
    ofVec3f mousePos;
    ofMesh  mesh,mesh2, drawingMesh, meshWarped;
    ofPolyline polyMesh, polyWarped;
    
    ofxStrip strip;
    
    ofEasyCam cam;
    
    
};

#endif /* defined(__OculusRenderingBasic__Mfcc_obj__) */
