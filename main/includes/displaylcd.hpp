
/** 
*  @file displayTFT.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Codigo displyTFT da placa de telemetria do DT2
 * @date 08-08-2923
 * @copyright MIT License (c) 2023
 * @details o endereco encontrado no módulo i2c foi 0x27, e display utilizado e de 16x2
 * 
 * 
 * 
*/


#ifndef DISPLAYLCD_H
#define DISPLAYLCD_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);



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
    
    
};

#endif
