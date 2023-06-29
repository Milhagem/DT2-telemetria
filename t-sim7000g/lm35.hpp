#ifndef LM35_H
#define LM35_H

#include <Arduino.h>

#define ADC_VREF_MV    3300.0
#define ADC_RESOLUTION 4096
#define AMPOP_OUT      35  // conferir esse pino
#define GAIN           3.89
#define LM35_SCALE     10

class LM35 {
private:
    float temperatura;

public:
    /**
     * @brief Atualiza o valor de temperatura do sensor LM35
    */
    float atualizaLM35();
    /**
     * @brief Imprime a temperatura na saida Serial;
    */
    void imprimir();

    float getTemperatura() { return this->temperatura; }
};

#endif
