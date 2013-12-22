#pragma once

#include "ofMain.h"

#include <stdio.h> 
#include <wiringPi.h> 

class testApp : public ofBaseApp{

public:

void setup();
//void select_row();
void update();
void draw();

void keyPressed(int key);
void keyReleased(int key);
void mouseMoved(int x, int y );
void mouseDragged(int x, int y, int button);
void mousePressed(int x, int y, int button);
void mouseReleased(int x, int y, int button);
void windowResized(int w, int h);
void dragEvent(ofDragInfo dragInfo);
void gotMessage(ofMessage msg);

ofVideoGrabber vidGrabber;
unsigned char * videoInverted;
ofTexture	 videoTexture;
int camWidth;
int camHeight;
int                 xPos;
int                 yPos;
int                 whiteX;
int                 whiteY;
int                 dotColor;
int                 cols;
int                 rowsB;
int                 videoScale;

unsigned char	 pixelsTest[512];
bool	 pixelsBoolean;
bool	 start;
bool	 ascii;

       unsigned char       pixelsB[512];
       unsigned char	 pixelsC[512];
       
       int	 counter;
       int	 threshold;

ofTrueTypeFont myfont;

unsigned char *	 horizontalFlip;
unsigned char *	 verticalFlip;


bool	ballState;
float	ballX, ballY;
float	velX, velY;
int	 ballLoc, oldBallLoc;
};
