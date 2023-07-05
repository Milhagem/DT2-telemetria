#include "displayTFT.hpp"

void DisplayTFT::setupDisplayTFT() {
  tft.init();  
  tft.setRotation(2);       // display in portrait
  tft.fillScreen(BLACK); 
  tft.setCursor(165,300); 
  tft.setTextSize (1);
  tft.print ("EH O MILHAS"); //ntj
}

void DisplayTFT::mostraConsumo() {
  int xpos = 55, ypos = 25;                            // text position
  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (xpos,ypos); tft.setTextSize (2);      // units position relative to scale
  tft.print ("Gasto: ");
  tft.setCursor (xpos+72,ypos); tft.setTextSize (2);   // units position relative to scale
  tft.print (gasto);  
  tft.setCursor (xpos+115,ypos); tft.setTextSize (2);  // units position relative to scale
  tft.print ("J");
}

void DisplayTFT::mostraVelocidadeMedia() {
  xpos = 40, ypos = 265;                               //text position
  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (xpos-10,ypos); tft.setTextSize (2);   // units position relative to scale
  tft.print ("Vel Med: ");
  tft.setCursor (xpos+85,ypos); tft.setTextSize (2);   // units position relative to scale
  tft.print (vel_media);  
  tft.setCursor (xpos+125,ypos); tft.setTextSize (2);  // units position relative to scale
  tft.print ("KM/H");  
}

int DisplayTFT::ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme) {
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option

  x += r; y += r;                                                             // calculate coordinates of center of ring
  int w = r / 3;                                                              // width of outer ring is 1/4 of radius
  int angle = 150;                                                            // half the sweep angle of the meter (300 degrees)
  int v = map(value, vmin, vmax, -angle, angle);                              // map the value to an angle v
  byte seg = 3;                                                               // segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 6;                                                               // draw segments every 3 degrees, increase to 6 for segmented ring
  int colour = BLUE;                                                          // variable to save "value" text color from scheme and set default


  for (int i = -angle+inc/2; i < angle-inc/2; i += inc) {                    // draw color blocks every increment degrees        
    float sx = cos((i - 90) * 0.0174532925);                                 // calculate pair of coordinates for segment start
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    float sx2 = cos((i + seg - 90) * 0.0174532925);                          // salculate pair of coordinates for segment end
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v) {                                                          // fill in coloured segments with 2 triangles
      switch (scheme){
        case 1: colour = rainbow(map(i, -angle, angle, 0, 127)); break;  // full spectrum blue to red
        case 2: colour = rainbow(map(i, -angle, angle, 70, 0)); break;   // verde para azul (velocidade dt2)
        default: colour = BLUE; break;                                   // fixed color
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    } else {                                                           // fill in blank segments
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, SCALE1);                // color of the unoccupied ring scale 
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, SCALE0);                // color of the unoccupied ring scale
    }
  }

  char buf [10];                                                              // convert value to a string
  byte len = 2; if (value > 999) len = 4;
  dtostrf (value, len, 0, buf);
  buf[len] = ' '; buf[len] = 0;                                               // add blanking space and terminator, helps to centre text too!

  tft.setTextSize (2);
  if (value>9) {
    tft.setTextColor (colour,BLACK);
    tft.setCursor (x-25,y-10);tft.setTextSize (5);
    tft.print (buf);
  }

  if(value<10) {
    tft.setTextColor (colour,BLACK);
    tft.setCursor (x-25,y-10);tft.setTextSize (5);
    tft.print (buf);
  }

  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (x-23,y+55);tft.setTextSize (2);                              // units position relative to scale
  tft.print (units);                                                          // units display = celsius                                        
  return x + r;                                                               // calculate and return right hand side x coordinate
}

unsigned int DisplayTFT::rainbow(byte value) {
  byte red = 0;                                                               // red is the top 5 bits of a 16 bit colour value
  byte green = 0;                                                             // green is the middle 6 bits
  byte blue = 0;                                                              // blue is the bottom 5 bits
  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }

  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }

  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  
  return (red << 11) + (green << 5) + blue;

}

