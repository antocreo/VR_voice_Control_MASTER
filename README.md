{\rtf1\ansi\ansicpg1252\cocoartf1265\cocoasubrtf210
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural

\f0\fs24 \cf0  //=================================================================================//\
 //================== V R    V O I C E    C O N T R O L  ===========================//\
 //=================================================================================//\
\
# VR Voice Control\
### Anto+Creo\
\
\
Exploring the possibilities of VR (Oculus Rift and Leap Motion) applied to experimental tools for expression of the self.\
End of term assignment: Creative Coding Workshop ::: MFA Computational arts 2014 -15\
\
\
#### AIM\
\
 The aim of this project is exploring some technical possibilities that devices like Oculus Rift and Leap Motion can bring to the design of interactive systems for the expression of the self.\
 Besides, this is an occasion to explore C++ and Openframeworks at a more deep level by dealing with addons, pointers and references, vector lists, vector forces and more complex algorithms.\
 One of the main goals of the overall project is to facilitate the user to connect with his/her own voice.\
 Being totally surrounded may help the user to disconnect from the reality and to unlock the voice, mostly if that is the only tool for the user to interact with the environment.\
 \
 \
#### PROCESS\
\
 The experience is approached as a sort of a game in which the user has to use the voice to repel the objects that otherwise will crash against him/her.\
 The interactive solution takes inspiration from the everyday life reality. We are target of a myriad of tiny little problems, that metaphorically fall on our head,  sometimes they chase us, and often we feel totally unable to react to them.\
 I think that introducing the user into a safe and alternative reality will help to stimulate a reaction.\
 \
 The process I followed to make the final app was to approach each element as single module. At first I created an app that uses the Mfcc and gets the values of the elements and calculates an average value for the voice input (by Using centroids).\
 Then I started working on the Leap Motion addons. At the moment there are 2 addons and I had to merge some functionalities for my needs.\
 Then I integrated the Mfcc app wrapping it into a class. Later I introduced the Bullet physcis objects into the Leap environment.\
 \
 In the meanwhile I started working on the Oculus Rift app and understanding the code. Finally I started merging the 3 different apps.\
 It has not been an easy process to integrate the 3 apps. The Oculus SDK and Leap Motion Libraries had to be readdressed manually.\
 \
 Also integrating ofxMaxim in the Oculus has been problematic as there was a naming conflict with a type unsigned long UInt32 included in both addons.\
 A note when you use ofxMaxim in a class is to make it "public ofBaseSoundOutput", otherwise it will not work.\
 \
 Other issues started when I had to integrate the Bullet into the OR world. The Bullet objects were included originally in my Leap app but this didn;t work well so I had to take them out.\
 Also the Leap motion integration has not been very straight forward. I didn't find any proper support online for Openframewroks and at today I am not aware of anyone that used both addons together on OF. I now understand that this is not the best framework to develop for these devices. Unity seems to be more appropriate.\
\
\
#### HOW to USE IT\
* step 1 - wear your Oculus Rift\
* step 2 - push \'91r\'92 if you need to reset the view of the Oculus camera\
* step 3 - left or right circle hand gesture gesture if you need more shapes. \
* step 4 - press spacebar to activate the attraction of the objects towards your face.\
* step 5 - start using your voice to control the objects and avoid them.\
\
When the objects start chasing you the speed will gradually increase. The only way you can stop them is using your voice!\
\
#### KEYBOARD COMMAND AND OPTIONS\
\
# Main Keyboard Commands\
 ----------\
 *r              - reset the view of the oculus\
 *p              - repopulate with new objects\
 *b              - create box\
 *s              - create sphere\
 *o		 - show overlay info\
 *spacebar       - toggle the gravity center (objects start chasing you!)\
 *v              - toggle the force (this will give the objects a force to follow -  and they chase you)\
 *backspace      - delete all the objects\
 *d		 - toggle hand drawing\
 *h		 - toggle hands view\
\
there are few more key commands that can be checked in the void testApp::keyPressed(int key)\
\
 \
 \
# Main gestures (Leap)\
 ----------\
 *draw circle left       - create a box\
 *draw a circle right    - creat a sphere\
 *swipe up               - delete all the objects\
 *swipe left/right       - move the objects away\
 \
# Main command : *your voice!\
\
#### INSTALLATION\
\
#OS\
this app is built for Mac OS X 10.9.5\
\
#Xcode version 6.1.1\
It hasn\'92t been tested for previous versions or other platforms.\
\
The easier way is creating a new project with \'91project generator\'92 and adding the following addons.\
\
#Addons\
 *ofxLeapMotion2\
 *ofxBullet\
 *ofxMaxim\
 *ofxOculusDK2\
\
##troubleshooting\
There might be problems with the libraries (Leap SDK and Oculus SDK) in which case I suggest to manually readdress them in the Build tab.\
Another issue that might come up is a naming conflict regarding an unsigned long UInt32. This is something involving ofxMaxim and Oculus SDK.\
The best solution I found so far is commenting one of the 2 lines. that will solve the thing.\
Any comment or suggestion regarding this problem is very much appreciated.\
\
#### STRUCTURE\
\
 The code counts 2 main classes\
 Leap Class - this wraps the basic project I've created for controlling the spheres with the Leap Motion\
 Mfcc Class - this wraps the Mfcc tool to detect the voice data.\
 \
 FloatingObjects Class - this wraps the Bullet physics app\
 ofxStrip Class - is a class made by Theo Watson that is included into the Leap Motion addon.\
\
\
#### NOTES\
 The program is working well and as expected.\
 I had to focus on some functionalities rather than other (e.g., voice rather than drawing).\
 While testing it with other users I noticed how Leap motion can distract the user from the main goal which is using the voice.\
 Leap motion is a very useful tool when you are immerse into a VR however it can drive the attention towards things which are not the main subject of this project (i.e, gesture or drawing)\
\
#### BUGS\
Bugs: I haven't noticed any major bug at today.\
 The main issue is the distortion of the Leap hands which is not adapted to the Oculus yet. This means that the sight of the virtual hands in the virtual environment can somehow have an aberration problem.\
Also the leap hands keep on following the oculus orientation. This needs to be improved.\
\
\
#### CONTACTS\
feel free to add, remove, edit, improve, destroy, munch, scratch the code as much as you like it.\
Please let me know if you need help, or if you find solutions to the bugs or any eventual improvement.\
\
\
//=================================================================================//\
//================== V R    V O I C E    C O N T R O L  ===========================//\
//=================================================================================//\
\
\
}