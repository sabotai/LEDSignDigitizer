/*

LED SIGN DIGITIZER

This code uses wiringPi to control a 32x16 P10 LED sign (ebay) and display a webcam image.  I have to give tons of credit and thanks to Jonathan 
Andrews for his code for interfacing the sign and clocking in the pixels.  Much of the code logic was ported from his p10 test image code.  Also, thanks
to Daniel Shiffman for his video threshold Processing example.  Understanding that is what helped me get this project started.





*/

#include "testApp.h"

#include <stdio.h> 
extern "C" {
#include <wiringPi.h>
} 
#include "ledHashTable.h" 
#include "testImage.h"

#define CLK	11
#define SCLK	10
#define A	4
#define B	1
#define	OE	5
#define PIXEL	6

//--------------------------------------------------------------
void testApp::setup(){

   pinMode (CLK, OUTPUT); //INITIALIZE WIRING PI PINS
   pinMode (SCLK, OUTPUT); //INITIALIZE WIRING PI PINS
   pinMode (A, OUTPUT); //INITIALIZE WIRING PI PINS
   pinMode (B, OUTPUT); //INITIALIZE WIRING PI PINS
   pinMode (OE, OUTPUT); //INITIALIZE WIRING PI PINS
   pinMode (PIXEL, OUTPUT); //INITIALIZE WIRING PI PINS

camWidth = 160;	// try to grab at this size.
camHeight = 120;

threshold = 150;

//myfont.loadFont("arial.ttf", 12);

vector<ofVideoDevice> devices = vidGrabber.listDevices();

   for(int i = 0; i < devices.size(); i++){
cout << devices[i].id << ": " << devices[i].deviceName;
       if( devices[i].bAvailable ){
           cout << endl;
       }else{
           cout << " - unavailable " << endl;
       }
}

vidGrabber.setDeviceID(0);
vidGrabber.setDesiredFrameRate(60);
vidGrabber.initGrabber(camWidth,camHeight);

verticalFlip = new unsigned char[camWidth*camHeight*3]; //setup to flip the image vertically
//horizontalFlip = new unsigned char[camWidth*camHeight*3

//videoInverted = new unsigned char[camWidth*camHeight*3];
videoTexture.allocate(camWidth,camHeight, GL_LUMINANCE);
ofSetVerticalSync(true);

ballX = 5;
ballY = 5;
velX = 1;//ofRandom(-5,5);
velY = velX;

start = true;
}


//--------------------------------------------------------------
void testApp::update(){
vidGrabber.update();
}



void select_row(int row){

digitalWrite(SCLK, 1); // SCLK starts in an idle state
digitalWrite(SCLK, 1);
if (row==0)
{
digitalWrite(A, 0);
digitalWrite(B, 0);
}
if (row==1)
{
digitalWrite(A, 1);
digitalWrite(B, 0);
}
if (row==2)
{
digitalWrite(A, 0);
digitalWrite(B, 1);
}
if (row==3)
{
digitalWrite(A, 1);
digitalWrite(B, 1);
}

   digitalWrite(SCLK, 0); //move to the next hash table address in the active row (next chunk)
   digitalWrite(SCLK, 0);
   //digitalWrite(SCLK, 0);
digitalWrite(SCLK, 1); //return to idle
//digitalWrite(SCLK, 1);
//digitalWrite(SCLK, 1);



}

void update_leds()
{
// all 512 pixels working, now reset to move on to the next frame
digitalWrite(OE, 0);
   digitalWrite(OE, 0);
   digitalWrite(OE, 0);
   digitalWrite(OE, 0);
   digitalWrite(OE, 1);
   digitalWrite(OE, 1);
   digitalWrite(OE, 1);
   digitalWrite(OE, 1);

}

void clock_pixels(unsigned char *ptr,int numpixels)
{
 	int color;
int offset;


offset=0;
digitalWrite(CLK, 1); // begin in the idle state (clock ON)
digitalWrite(CLK, 1);
 	for (int i = 0;i<numpixels;i++)	 // cycles through each of the 8 pixels per chunk
{
color=*(ptr+offset); //the offset moves the pointer through the pixelsB array
if (color==0)
{
digitalWrite(PIXEL, 1);	 // pixel off
}
else
{
digitalWrite(PIXEL, 0);	//pixel on
}
// clock the pixels.  it gets weird if you dont do it off a few times and then on a few times

digitalWrite(CLK, 0);
digitalWrite(CLK, 0);
digitalWrite(CLK, 0);
digitalWrite(CLK, 0);
digitalWrite(CLK, 0);
digitalWrite(CLK, 0);


digitalWrite(CLK, 1);
digitalWrite(CLK, 1);
digitalWrite(CLK, 1);
digitalWrite(CLK, 1);
digitalWrite(CLK, 1);
digitalWrite(CLK, 1);


offset++;
  }
}

void display_ascii_image(unsigned char *ptr) //ascii version of the image in the console
{
int offset;
int x,y;
int color;

offset=0;
for (x=0;x<16;x++)
{
for (y=0;y<32;y++)
{
//color=*ptr+offset;
color=*(ptr+offset);
if (color==1)
printf("1 ");
else	printf("  ");
offset++;
}
printf("\n");
}
fflush(stdout);
printf("\n");
}

//--------------------------------------------------------------
void testApp::draw(){

ofBackground(100,100,100);
//ofSetHexColor(0xffffff);
//vidGrabber.draw(20,20);
   vidGrabber.update();


       




 int row;//=0;
 int chunk;
 
 if (ascii == true) { //if the button has been pressed, display the ascii version of the pixelsB array in the console
display_ascii_image(pixelsB); //running this slows down the framerate quite a bit
}

 // idle state for all lines
   digitalWrite(SCLK, 1);
   digitalWrite(CLK, 1);
   digitalWrite(A, 0);
   digitalWrite(B, 0);
   digitalWrite(PIXEL, 0);
   digitalWrite(OE, 0);






for (row=0;row<=3;row++)	 // each row is 4 lines at a time
{
      for (chunk=0;chunk<16;chunk++)	 // 16 8-pixel chunks
{
// clock pixels tells the function to use the indexes locations for the regular ordered pixelsB array

clock_pixels(pixelsB + (indexes[row][chunk]),8); //send the pixels in the array in the correct order/location according to the hash table


}


update_leds();	 // reset the pixels
       select_row(row);	 // select the next row

//cout.flush();
usleep(2000);  //sleep is important so the board doesnt overheat
}


////////////// WEBCAM VIDEO PROCESSOR	

if (start == true) {  //once Q has been pressed, begins processing video

videoScale = 5;  //divide the video dimensions (currently 160) by 5 to get 32 pixels wide, the same as the LED sign
       cols = camWidth / videoScale;  //160 / 5 = 32
       rowsB = camHeight / videoScale; //120 / 5 = 20 (which we later limit to 16 rows)
unsigned char * pixels = vidGrabber.getPixels(); //begin grabbing pixels
       //int threshold = 100;
       int counts = 0;
       
       //instructions for flipping the video signal before processing it
       for (int s = 0; s < 3 * camWidth; s+=3) {
for (int t = 0; t < camHeight; t++){

verticalFlip[(camHeight - t - 1) * 3 * camWidth + s] = pixels[t * 3 * camWidth + s];
verticalFlip[(camHeight - t - 1) * 3 * camWidth + s + 1] = pixels[t * 3 * camWidth + s + 1];
verticalFlip[(camHeight - t - 1) * 3 * camWidth + s + 2] = pixels[t * 3 * camWidth + s + 2];
}}
       
       

for (int i = 0; i < rowsB; i++) { //cycle through the right pixels in the original RGB image
           for (int j = 0; j < cols; j+=1) {
               if (i>4){
               xPos = j*videoScale*3;  //all the *3 stuff is because the RGB image is 3x as wide as a BW image
               yPos = i*videoScale;
               
               int loc = xPos + yPos * camWidth*3;

               if (verticalFlip[loc] > threshold) {

                    pixelsB[counts] = 1; //counts cycles through the pixels up to 512-1, assigns ON to those pixels
                    //pixelsC[i*j] = '1';
                    //dotColor = 255;
               } else {
                    pixelsB[counts] = 0;
                    //pixelsC[i*j] = '0';
                    //dotColor = 0;
               }
               
               if (counts < 512) {
counts++;
}
               
               //ofSetColor(dotColor);
               //ofCircle(xPos/3 +25, yPos+25, videoScale/4, videoScale/4); //ellipse matrix
               
               //ofSetColor(255);
               // myfont.drawString(ofToString(pixelsC[i*j]), xPos+15,yPos*3+365);
               
               //printf("%d\n",loc/15);  //showing the order of the array assignments
               }

           }
       }
       
       if (ballState) {
ballX = ballX + velX;
ballY = ballY + velY;

if (ballX >= 31 || ballX <= 1) {

velX *= (-1);
}

if (ballY >= 15 || ballY <= 1) {

velY *= (-1);
}

ballLoc = ballY * 32 + ballX;

if (pixelsB[ballLoc]) {
velX *= (-1);
velY *= (-1);

}

pixelsB[ballLoc] = 1;
pixelsB[ballLoc - 1] = 1;
pixelsB[ballLoc + 1] = 1;
pixelsB[ballLoc + 32] = 1;
pixelsB[ballLoc - 32] = 1;

pixelsB[oldBallLoc] = 0;
pixelsB[oldBallLoc - 1] = 0;
pixelsB[oldBallLoc + 1] = 0;
pixelsB[oldBallLoc + 32] = 0;
pixelsB[oldBallLoc - 32] = 0;


oldBallLoc = ballLoc;


}
}

  }








//--------------------------------------------------------------
void testApp::keyPressed  (int key){


   if (key == 'z' || key == 'Z'){ //decrease the video threshold
if (threshold > 0) {
threshold -= 5;
printf("threshold = %d\n", threshold );
}
}
   if (key == 'x' || key == 'X'){ //increase the video threshold
if (threshold < 255) {
threshold += 5;
printf("threshold = %d\n", threshold );
}
}

if (key == 'b' || key == 'B'){
//ballX = ofRandom (1,31);
//ballY = ofRandom (1, 15);
//velX = ofRandom (0.5, 0.7);
//velY = velX + ofRandom (-0.1, 0.1);
ballState = !ballState;
}

   
if (key == 'q' || key == 'Q'){ //press q to initiate video processing
start = !start;
}

if (key == 'w' || key == 'W') { //press w to display the ascii version of the matrix
ascii = !ascii;
}

if (key == 's' || key == 'S'){ //press s to display the test image
for (int pass = 0; pass < 512; pass++) {
pixelsB[pass] = image_data[pass];
}
}

if (key == 'd' || key == 'D'){ //press d to clear the pixels
for (int pass = 0; pass < 512; pass++) {
pixelsB[pass] = 0;
}
}
if (key == 'f' || key == 'F'){ //press f to display all pixels
for (int pass = 0; pass < 512; pass++) {
pixelsB[pass] = 1;
}
}

   if (key == 'a' || key =='A'){ //press a to cycle on/off through the pixels in order
if (counter <511) {
if (pixelsBoolean) {
pixelsB[counter] = 1;
}
else {
pixelsB[counter] = 0;
}
counter++;
}
if (counter == 511) {
pixelsBoolean = !pixelsBoolean;
counter = 0;
}
}


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
