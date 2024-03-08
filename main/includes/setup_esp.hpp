#ifndef _SETUP_ESP_H_
#define _SETUP_ESP_H_



//-----------------------------------------SD PINS
#define SD_MISO 2 
#define SD_CS   13
#define SD_MOSI 15
#define SD_SCLK 14
#define PWR_PIN 4
#define LED_PIN 12

#define ONE_BIT_MODE  true



//-----------------------------------------Display Definitons
// Some principal color definitions
// RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0

#define SCALE0      0xC655  // accent color for unused scale segments                                                     
#define SCALE1      0x5DEE  // accent color for unused scale segments     

#define BLUE2RED   1  // circular scale color scheme
#define GREEN2BLUE 2  // circular scale color scheme

#define GAUGEPOSITION_X  20  // govern the position of the square + gauge on the display
#define GAUGEPOSITION_Y  20  // govern the position of the square + gauge on the display
#define RADIUS           80  // upper ring radius



//-----------------------------------------ENCODER DEFINITIONS
#define MEASURE_PIN         25            // pino D0 encoder
#define WHEEL_CIRCUMFERANCE 1.50          // m     
#define SAMPLES             16            // numero de pontos medidos no disco de freio
#define TIME_INTERVAL       500           // ms (intervalo de tempo para amostragem de pulsos)
#define MINIMAL_SPEED       2.77          // m/s (velocidade minima para trocar metodo de amostraVoltas())
#define ms_TO_min           0.001666666 // milisegundos para minutos
#define min_TO_ms           60000.0    // minutos para milisegundos


#endif