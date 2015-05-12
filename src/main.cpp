#include "testApp.h"
#include "ofAppGlutWindow.h"
//--------------------------------------------------------------
int main(){
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context

    
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
//	ofSetupOpenGL(1024, 768, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}


/*
 //================================================================================================================================================//
 //==================================================== V R    V O I C E    C O N T R O L  ========================================================//
 //================================================================================================================================================//
 //Author:
 //Antonio Daniele
 //EXPLORING THE POSSIBILITIES OF VR (Oculus Rift and Leap Motion) APPLIED TO EXPERIMENTAL TOOLS FOR THE EXPRESSION OF THE SELF.
 //End of term assignment: Creative Coding Workshop ::: MFA Computational arts 2014 -15
 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
 Aim
 ---
 The aim of this project is exploring some technical possibilites that devices like Oculus Rift and Leap motion can bring to the design of interactive systems for the expression of the self.
 Besides this is an occasion to explore C++ and Openframeworks at a more deep level by dealing with addons, pointers and references, vector lists, vector forces and more complex algorithms.
 One of the main goals of the overall project is to facilitate the user to connect with his/her own voice.
 Being totally surrounded may help the user to disconnect from the reality and to unlock the voice, mostly if that is the only tool for the user to interact with the environment.
 
 
 Process
 -------
 The experience is approached as a sort of a game in which the user has to use the voice to repel the objects that otherwise will crash against him/her.
 The interactive solution takes inspiration from the everyday life reality. We are target of a myriad of tiny little problems, that metaphorically fall on our head,  sometimes they chase us, and often we feel totally unable to react to them.
 I think that introducing the user into a safe and alternative reality will help to stimulate a reaction.
 
 The process I followed to make the final app was to approach each element as single module. At first I created an app that uses the Mfcc and gets the values of the elements and calculates an average value for the voice input (by Using centroids).
 Then I started working on the Leap Motion addons. At the moment there are 2 addons and I had to merge some functionalities for my needs.
 Then I integrated the Mfcc app wrapping it into a class. Later I introduced the Bullet physcis objects into the Leap environment.
 
 In the meanwhile I started working on the Oculus Rift app and understanding the code. Finally I started merging the 3 different apps.
 It has not been an easy process to integrate the 3 apps. The Oculus SDK and Leap Motion Libraries had to be readdressed manually.
 
 Also integrating ofxMaxim in the Oculus has been problematic as there was a naming conflict with a type unsigned long UInt32 included in both addons.
 A note when you use ofxMaxim in a class is to make it "public ofBaseSoundOutput", otherwise it will not work.
 
 Other issues started when I had to integrate the Bullet into the OR world. The Bullet objects were included originally in my Leap app but this didn;t work well so I had to take them out.
 Also the Leap motion integration has not been very straight forward. I didn't find any proper support online for Openframewroks and at today I am not aware of anyone that used both addons together on OF. I now understand that this is not the best framework to develop for these devices. Unity seems to be more appropriate.
 
 
 
 
 Structure
 --------
 The code counts 2 main classes
 Leap Class - this wraps the basic project I've created for controlling the spheres with the Leap Motion
 Mfcc Class - this wraps the Mfcc tool to detect the voice data.
 
 FloatingObjects Class - this wraps the Bullet physics app
 ofxStrip Class - is a class made by Theo Watson that is included into the Leap Motion addon.
 
 Addons used:
 --------
 ofxLeapMotion2
 ofxBullet
 ofxMaxim
 ofxOculusDK2
 
 
 Comments
 --------
 The program is working well and as expected.
 I had to focus on some functionalities rather than other (e.g., voice rather than drawing).
 While testing it with other users I noticed how Leap motion can distract the user from the main goal which is using the voice.
 Leap motion is a very useful tool when you are immerse into a VR however it can drive the attention towards things which are not the main subject of this project (i.e, gesture or drawing)
 
 
 Evaluation
 ----------
 Bugs: I haven't noticed any bugs at today.
 The main issue is the distortion of the Leap hands which is not adapted to the Oculus yet. This means that the sight of the virtual hands in the virtual environment can somehow have an aberration problem.
 
 
Main Keyboard Commands
 ----------
 r              - reset the view of the oculus
 p              - repopulate with new objects
 b              - create box
 s              - create sphere
 o              - show overlay info
 spacebar       - toggle the gravity center (objects start chasing you!)
 v              - toggle the force (this will give the objects a force to follow -  and they chase you)
 backspace      - delete all the objects
 d              - toggle hand drawing
 h              - toggle hands view
 
 there are few more key commands that can be checked in the void testApp::keyPressed(int key)
 
 
 Main gestures (Leap)
 ----------
 draw circle left       - create a box
 draw a circle right    - creat a sphere
 swipe up               - delete all the objects
 swipe left/right       - move the objects away
 
 Main command : your voice!
 
 
 */

//================================================================================================================================================//
//==================================================== V R    V O I C E    C O N T R O L  ========================================================//
//================================================================================================================================================//