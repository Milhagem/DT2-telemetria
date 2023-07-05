#ifndef DISPLAYTFT_H
#define DISPLAYTFT_H

#define TFT_BL   32            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)
#define TFT_RGB_ORDER TFT_BGR  // color order Blue-Green-Red - for this specific display  

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();


/**
 * some principal color definitions
 * RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
*/

#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define GREEN       0x07E0

#define SCALE0      0xC655  // accent color for unused scale segments                                                     
#define SCALE1      0x5DEE  // accent color for unused scale segments     

#define BLUE2RED   1  // circular scale color scheme  
#define GREEN2BLUE 2  // circular scale color scheme  

class DisplayTFT {
private:

public:
   /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o DisplayTFT
    */
    void setupDisplayTFT();
    /**
     * @brief Cria o texto do Consumo Acumulado (referente ao sensor INA226)
    */
    void mostraConsumo();
    /**
     * @brief Cria o texto da Velocidade Media (referente ao Encoder)
    */
    void mostraVelocidadeMedia();
    /**
     * @brief Desenha o medidor na tela do Display
     * 
     * @return Coordenada x do lado direito
    */
    int ringMeter(double value, double vmin, double vmax, int x, int y, int r, char *units, byte scheme);
    /**
     * @brief Converte o parametro valor para um espectro de cores de 0 = azul até 127 = vermelho, retornando uma cor de arco iris de 16 bits
     * 
     * @param value Espera-se que esteja no intervalo 0-127
     * 
     * @return cor de arco-iris de 16 bits
    */
    unsigned int rainbow(byte value);
    /**
     * @brief Retorna um valor no intervalo -1 a +1 para um determinado ângulo de fase em graus
    */
    float sineWave(int phase) { return sin(phase * 0.0174532925); }
}

#endif