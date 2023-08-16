
/* 
*  @file displayTFT.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Codigo displyTFT da placa de telemetria do DT2
 * @date 08-08-2923
 * @copyright MIT License (c) 2023
 * 
 * 
 * 
 * 
*/


#ifndef DISPLAYLCD_H
#define DISPLAYLCD_H



/**
 * @brief some principal color definitions
 * RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
*/


class DisplayLCD {
private:
  

public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o DisplayTFT
    */
    void setupDisplayLCD();
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
    
};

#endif
