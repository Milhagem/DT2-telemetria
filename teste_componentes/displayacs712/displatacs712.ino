// some principal color definitions
// RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0
#define SCALE0      0xC655                                                  // accent color for unused scale segments                                   
#define SCALE1      0x5DEE 
// circular scale color scheme  
#define BLUE2RED 1
#define GREEN2BLUE 2

#define CURRENT 25


double amp = 0.0;
double sum = 0.0;

const double ajuste_corrente = 1.2297601967717135494;

double sensor(double, double, double, double, double);


#define TFT_BL   32            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)
#define TFT_RGB_ORDER TFT_BGR                      // color order Blue-Green-Red - for this specific display  
#include <TFT_eSPI.h>                              // hardware-specific library
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI(); 

#define MEASURE_PIN 25
const double WHEEL_CIRCUMFERANCE = 1.596;
const double pulses_per_revolution = 16;
const double fator_correcao = 0.7;
#define SAMPLES 25


void setup() {

  Serial.begin(115200); 
  pinMode(CURRENT, INPUT);


tft.init();  
  tft.setRotation (2);       // display in portrait
  tft.fillScreen (BLACK); 
  //EH O MILHAS
  tft.setCursor (165,300); tft.setTextSize (1);
  tft.print ("EH O MILHAS"); //ntj
}

// ---------- Display data  ----------
float gasto = 0;
double vel_media = 0;
double vel = 0;   
int gaugeposition_x = 20;  // these two variables govern the position
int gaugeposition_y = 20;  // of the square + gauge on the display


int ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme);

void loop() {
//Cria o primeiro arco-íris
  
  sum = 0.0;

  for (int i = 1000; i>0; i--){

    sum += sensor(analogRead(CURRENT), 0.0, 1023.0, (-30.0), 30.0);

  }
  
  amp = sum/1000;

  Serial.println(amp*ajuste_corrente);
  
  int radius = 80; //upper ring radius
  ringMeter (amp,0,60,(gaugeposition_x+20),(gaugeposition_y+57),radius,"Km/h",GREEN2BLUE);
      
  //Cria o texto do GASTO
  //JOULÍMETRO - Gasto acumulado;
  int xpos = 55, ypos = 25; //text position
  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (xpos,ypos); tft.setTextSize (2);                              // units position relative to scale
  tft.print ("Gasto: ");
  tft.setCursor (xpos+72,ypos); tft.setTextSize (2);                           // units position relative to scale
  tft.print (gasto);  
  tft.setCursor (xpos+115,ypos); tft.setTextSize (2);                          // units position relative to scale
  tft.print ("J");

  //Cria o texto do Vel Med
  //VEL MED - Velocidade media;
  xpos = 40, ypos = 265; //text position
  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (xpos-10,ypos); tft.setTextSize (2);                              // units position relative to scale
  tft.print ("Vel Med: ");
  tft.setCursor (xpos+85,ypos); tft.setTextSize (2);                           // units position relative to scale
  tft.print (amp);  
  tft.setCursor (xpos+125,ypos); tft.setTextSize (2);                          // units position relative to scale
  tft.print ("KM/H");  

}

// ################################################################################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// ################################################################################################################################

int ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme) {
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option

  x += r; y += r;                                                             // calculate coordinates of center of ring
  int w = r / 3;                                                              // width of outer ring is 1/4 of radius
  int angle = 150;                                                            // half the sweep angle of the meter (300 degrees)
  int v = map(value, vmin, vmax, -angle, angle);                              // map the value to an angle v
  byte seg = 3;                                                               // segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 6;                                                               // draw segments every 3 degrees, increase to 6 for segmented ring
  int colour = BLUE;                                                          // variable to save "value" text color from scheme and set default


  for (int i = -angle+inc/2; i < angle-inc/2; i += inc)                       // draw color blocks every increment degrees
    {           
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

    if (i < v) 
    {                                                                     // fill in coloured segments with 2 triangles
      switch (scheme)
      {
        case 1: colour = rainbow(map(i, -angle, angle, 0, 127)); break;  // full spectrum blue to red
        case 2: colour = rainbow(map(i, -angle, angle, 70, 0)); break;   // verde para azul (velocidade dt2)
        default: colour = BLUE; break;                                   // fixed color
      }
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    }else                                                                    // fill in blank segments
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, SCALE1);                // color of the unoccupied ring scale 
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, SCALE0);                // color of the unoccupied ring scale
    }
  }

  char buf [10];                                                              // convert value to a string
  byte len = 3; if (value > 999) len = 4;
  dtostrf (value, len, 0, buf);
  buf[len] = ' '; buf[len] = 0;                                               // add blanking space and terminator, helps to centre text too!

  tft.setTextSize (2);
  if (value>9)
  {
    tft.setTextColor (WHITE,BLACK);
    tft.setCursor (x-55,y-10);tft.setTextSize (5);
    tft.print (buf);
  }

  if(value<10)
  {
    tft.setTextColor (WHITE,BLACK);
    tft.setCursor (x-55,y-10);tft.setTextSize (5);
    tft.print (buf);
  }

  tft.setTextColor (WHITE,BLACK);
  tft.setCursor (x-23,y+55);tft.setTextSize (2);                              // units position relative to scale
  tft.print (units);                                                          // units display = celsius                                        
  return x + r;                                                               // calculate and return right hand side x coordinate
}

// ################################################################################################################################
// Return a 16 bit rainbow colour
// ################################################################################################################################

unsigned int rainbow(byte value) {                                             // value is expected to be in range 0-127
                                                                               // value is converted to a spectrum color from 0 = blue through to 127 = red
  byte red = 0;                                                               // red is the top 5 bits of a 16 bit colour value
  byte green = 0;                                                             // green is the middle 6 bits
  byte blue = 0;                                                              // blue is the bottom 5 bits
  byte quadrant = value / 32;

  if (quadrant == 0)
  {
    blue = 0; //31;
    green = 2 * (value % 32);
    red = 31; //0;
  }
  if (quadrant == 1)
  {
    blue = 0; //31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2)
  {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3)
  {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}

// ################################################################################################################################
// Return a value in range -1 to +1 for a given phase angle in degrees
// ################################################################################################################################

float sineWave(int phase) {
  return sin(phase * 0.0174532925);
}

double sensor(double a, double in_min, double in_max, double out_min, double out_max) {
  return (a - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}