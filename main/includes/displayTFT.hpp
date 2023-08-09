
/** 
 * @file displayTFT.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Codigo displayTFT da placa de telemetria do DT2. 
 * Esse Display exibe os dados de velocidade para o piloto
 * 
 * @details
 * ####################
 * ##    ATENCAO     ##
 * ####################
 * 
 * !! Antes de usar esse codigo, eh necessario configurar o arquivo <User_Setup.h> em seu PC. !!
 * !! Veja o arquivo README.md contido em ../../teste_componentes/displayTFT                  !!
 * 
 * @date 2023-09-09
*/

#ifndef DISPLAYTFT_H
#define DISPLAYTFT_H

#define TFT_BL   32            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)
#define TFT_RGB_ORDER TFT_BGR  // color order Blue-Green-Red - for this specific display  

#include <TFT_eSPI.h>
#include <SPI.h>


/**
 * @brief some principal color definitions
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
  TFT_eSPI tft = TFT_eSPI();

public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o DisplayTFT
    */
    void setupDisplayTFT();
    /**
     * @brief Cria o texto do Consumo Acumulado (referente ao sensor INA226)
    */
    void mostraConsumo(float consumo);
    /**
     * @brief Cria o texto da Velocidade Media (referente ao Encoder)
    */
    void mostraVelocidadeMedia(double vel_media);
    /**
     * @brief  Desenha o medidor na tela do Display
     * @details Minimum value of r is about 52 before value text intrudes on ring
     *          drawing the text first is an option.
     *          we will need half the sweep angle of the meter (300 degrees)
     *          map the value to an angle v
     *
     * 
     * @param value variable to save "value" text color from scheme and set default
     * @param units 
     * @param scheme 
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
};

#endif
