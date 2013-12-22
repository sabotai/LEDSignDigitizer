// This tells the code the order in which to send the pixels.  the first 4 numbers in each array are the locations (within the total 512) 
// of the pixels in the first "column".  The actual rows in the sign alternate between the 4, so the cartesian order would be ---------
// row0, row1, row2, row3, row0, row1, row2, etc.. After the first 4 pixels in the row, the next 4 locations are 8 pixels over (since we're 
// clocking in 8 pixels in each chunk.



unsigned int indexes[4][16]={

//{0,128,256,384,8,136,264,392,16,144,272,400,24,152,280,408}, //row 0
{384,256,128,0,392,264,136,8,400,272,144,16,408,280,152,24},

//{32,160,288,416,40,168,296,424,48,176,304,432,56,184,312,440}, //row 1
{416,288,160,32,424,296,168,40,432,304,176,48,440,312,184,56},

//{64,192,320,448,72,200,328,456,80,208,336,464,88,216,344,472}, //row 2
{448,320,192,64,456,328,200,72,464,336,208,80,472,344,216,88},


{480,352,224,96,488,360,232,104,496,368,240,112,504,376,248,120}};
//{96,224,352,480,104,232,360,488,112,240,368,496,120,248,376,504}}; //row 3

