#include "displayTFT.hpp"

// Method to set up the TFT display
void DisplayTFT::setupDisplayTFT() {
    this->tft.init();
    this->tft.setRotation(2);       // Display in portrait
    this->tft.fillScreen(BLACK);
    this->tft.setCursor(165, 300);
    this->tft.setTextSize(1);
    this->tft.print("EH O MILHAS"); //ntj
}

// Method to display the fuel consumption on the TFT
void DisplayTFT::showConsumption(float consumption) {
    int xpos = 55, ypos = 25;                            // Text position
    this->tft.setTextColor(WHITE, BLACK);
    this->tft.setCursor(xpos, ypos);
    this->tft.setTextSize(2);                           // Units position relative to scale
    this->tft.print("Gasto: ");
    this->tft.setCursor(xpos + 72, ypos);
    this->tft.setTextSize(2);                           // Units position relative to scale
    this->tft.print(consumption);
    this->tft.setCursor(xpos + 115, ypos);
    this->tft.setTextSize(2);                           // Units position relative to scale
    this->tft.print("J");
}

// Method to display the average speed on the TFT
void DisplayTFT::showAverageSpeed(double average_speed) {
    int xpos = 40, ypos = 265;                          // Text position
    this->tft.setTextColor(WHITE, BLACK);
    this->tft.setCursor(xpos - 10, ypos);
    this->tft.setTextSize(2);                           // Units position relative to scale
    this->tft.print("Vel Med: ");
    this->tft.setCursor(xpos + 85, ypos);
    this->tft.setTextSize(2);                           // Units position relative to scale
    this->tft.print(average_speed);
    this->tft.setCursor(xpos + 125, ypos);
    this->tft.setTextSize(2);                           // Units position relative to scale
    this->tft.print("KM/H");
}

// Method to draw a ring meter on the TFT and return the right-hand side x coordinate
int DisplayTFT::ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme) {
    // Minimum value of r is about 52 before value text intrudes on the ring
    // Drawing the text first is an option

    x += r; y += r;                                     // Calculate coordinates of the center of the ring
    int w = r / 3;                                      // Width of the outer ring is 1/4 of the radius
    int angle = 150;                                    // Half the sweep angle of the meter (300 degrees)
    int v = map(value, vmin, vmax, -angle, angle);      // Map the value to an angle v
    byte seg = 3;                                       // Segments are 3 degrees wide = 100 segments for 300 degrees
    byte inc = 6;                                       // Draw segments every 3 degrees, increase to 6 for segmented ring
    int colour = BLUE;                                  // Variable to save "value" text color from scheme and set default

    for (int i = -angle + inc / 2; i < angle - inc / 2; i += inc) {  // Draw color blocks every increment degrees        
        float sx = cos((i - 90) * 0.0174532925);                    // Calculate pair of coordinates for segment start
        float sy = sin((i - 90) * 0.0174532925);
        uint16_t x0 = sx * (r - w) + x;
        uint16_t y0 = sy * (r - w) + y;
        uint16_t x1 = sx * r + x;
        uint16_t y1 = sy * r + y;

        float sx2 = cos((i + seg - 90) * 0.0174532925);   // Calculate pair of coordinates for segment end
        float sy2 = sin((i + seg - 90) * 0.0174532925);
        int x2 = sx2 * (r - w) + x;
        int y2 = sy2 * (r - w) + y;
        int x3 = sx2 * r + x;
        int y3 = sy2 * r + y;

        if (i < v) {                                      // Fill in colored segments with 2 triangles
            switch (scheme) {
                case 1: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
                case 2: colour = rainbow(map(i, -angle, angle, 70, 0)); break;  // Green to blue (velocity dt2)
                default: colour = BLUE; break;                                  // Fixed color
            }
            this->tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
            this->tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
        } else {                                       // Fill in blank segments
            this->tft.fillTriangle(x0, y0, x1, y1, x2, y2, SCALE1);        // Color of the unoccupied ring scale 
            this->tft.fillTriangle(x1, y1, x2, y2, x3, y3, SCALE0);        // Color of the unoccupied ring scale
        }
    }

    char buf[10];                                        // Convert value to a string
    byte len = 2; if (value > 999) len = 4;
    dtostrf(value, len, 0, buf);
    buf[len] = ' '; buf[len] = 0;                        // Add blanking space and terminator, helps to center text too!

    this->tft.setTextSize(2);
    if (value > 9) {
        this->tft.setTextColor(colour, BLACK);
        this->tft.setCursor(x - 25, y - 10); this->tft.setTextSize(5);
        this->tft.print(buf);
    }

    if (value < 10) {
        this->tft.setTextColor(colour, BLACK);
        this->tft.setCursor(x - 25, y - 10); this->tft.setTextSize(5);
        this->tft.print(buf);
    }

    this->tft.setTextColor(WHITE, BLACK);
    this->tft.setCursor(x - 23, y + 55); this->tft.setTextSize(2);     // Units position relative to scale
    this->tft.print(units);                                            // Units display = Celsius
    return x + r;                                                      // Calculate and return the right-hand side x coordinate
}

// Method to generate a rainbow color based on the input value
unsigned int DisplayTFT::rainbow(byte value) {
    byte red = 0;                          // Red is the top 5 bits of a 16-bit color value
    byte green = 0;                        // Green is the middle 6 bits
    byte blue = 0;                         // Blue is the bottom 5 bits
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
