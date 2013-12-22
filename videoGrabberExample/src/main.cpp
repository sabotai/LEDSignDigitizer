#include "ofMain.h"
//#include "ofApp.h"
//#include "ofAppNoWindow.h"
#include "testApp.h"
#include <stdio.h> 

extern "C" {
#include <wiringPi.h>
}  

//========================================================================
int main( ){

if (wiringPiSetup() == -1)
return 1 ;


//ofAppNoWindow w;
ofSetupOpenGL(160,120, OF_WINDOW);	 // <-------- setup the GL context

// this kicks off the running of my app
// can be OF_WINDOW or OF_FULLSCREEN
// pass in width and height too:
ofRunApp( new testApp());

}
