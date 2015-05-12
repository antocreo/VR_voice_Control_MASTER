#pragma once

#include "ofMain.h"
#include "ofxOculusDK2.h"
#include "Mfcc_obj.h"
#include "LeapClass.h"
//#include "FloatingObject.h"
#include "ofxBullet.h"

//typedef struct{
//	ofColor color;
//	ofVec3f pos;
//	ofVec3f floatPos;
//	float radius;
//    bool bMouseOver;
//    bool bGazeOver;
//} DemoSphere;

class testApp : public ofBaseApp
{
  public:
	
	void setup();
	void update();
	void draw();
    void exit();
	
	void drawScene();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
    void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */

    void onCollision(ofxBulletCollisionData& cdata);
    void mousePickEvent( ofxBulletMousePickEvent &e );
    void populateObjects();
    void createBox();
    void createSphere();
    void voicePush();
    
    //--------------OCULUS-----------//

    
	ofxOculusDK2		oculusRift;

	ofLight				light;
    ofLight             pointlight;
    ofLight             directionalLight;

    ofMaterial          material;
    ofVec3f             center;
    
	ofCamera			cam;
	bool showOverlay;
	bool predictive;
    
//	vector<DemoSphere> demos;
    
    ofVec3f cursor2D;
    ofVec3f cursor3D;
    
    ofVec3f cursorRift;
    ofVec3f demoRift;
    
    ofVec3f cursorGaze;
    
    float camHeigth;
    
    GLUquadricObj *quadric;
    
    vector<ofMeshFace>      triangles;

    

    //--------------TEXTURE-----------//

    
    int 			w, h;
    
    unsigned char 	* colorPixels;
    
    ofTexture        texColor;
    ofShader          shader;
    
    float newColor[4];
    
    //--------------MFCC-----------//

    ofSoundStream soundStream;
    Mfcc_obj myMfcc;
    
    double rms;
    double *mfccs;
    
    double *audioData;
    
    
//    float 	* lAudioOut; /* outputs */
//    float   * rAudioOut;
//    
//    float * lAudioIn; /* inputs */
//    float * rAudioIn;
//    
//    int		initialBufferSize; /* buffer size */
//    int		sampleRate;

    
    //--------------DISPLACEMENT-----------//

    
    ofMesh meshOriginal;
    ofMesh meshWarped;
    ofIcoSpherePrimitive    around;
    ofPlanePrimitive        plane;
    ofPlanePrimitive        grid;
//    ofSpherePrimitive        around;
    float                   radius;
    
    
    int                     numOfVerts;
    
    
    //--------------LEAP--------------//
    
    
    LeapClass myLeap;
    
   
    //-------STRIP------//
    
    ofLight l1;
    ofLight l2;
    ofMaterial m1;
    
    map <int, ofPolyline> fingerTrails;
    
    bool seeHands,isDrawing;
    

    
    
    //--------------BULLET--------------//

    
    //bullet globals
    
    vector <ofxBulletSphere> mySpheres;
    
//    FloatingObject          floatingObj;
    
    shared_ptr<ofxBulletTriMeshShape> myFloor;
    ofxBulletSphere         sky;
    
    ofxBulletWorldRigid     thisWorld;

    vector <ofxBulletBox*>		bounds;
    float						boundsWidth;
    
    ofLight						lightBull;
    
    btBoxShape*					boxShape;
    btSphereShape*				sphereShape;
    vector<ofxBulletBaseShape*>	shapes;
    vector<bool>				bColliding;
    int							mousePickIndex;
    ofVec3f						mousePickPos;
    ofVec3f                     shapeBirthLoc;
    float                       worldBullDist;
    
    bool						bDrawDebug;
    bool						bRenderShapes;
    bool						bAddCenterAttract;
    bool						bSpacebar;
    
    bool                        objDone;
    ofCamera    bullCam;
    
    //--------------------------------------------------------------
    
    
   

    

};
