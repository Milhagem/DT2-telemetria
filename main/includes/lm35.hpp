/**
 * @file lm35.hpp
 * @author Milhagem UFMG (contatomilhagem@ufmg.com)
 * @brief Codigo referente ao sensor de tempetura LM35 da placa de telemetria do DT2
 * @details 
 * Esse codigo roda  eh referente ao sensor de temperatura LM35 e roda na placa t-sim7000g,
 * que por sua vez vai na placa da telemetria do prototipo eletrico DT2. Os componentes 
 * da placa da telemetria referentes ao sensor de temperatura LM35 sao um Amplificador Operaticonal,
 * e alguns resistores.
 * 
 * O LM35 tem um output de tensao linear com a temperatura em graus celcius: sao 10mV para cada
 * grau celcius, ou seja, em temperaturada ambiente (25 graus celcius) o LM35 teria um output
 * de 250mV. Todavia, a porta ADC do ESP32 tem uma leitura especialmente ruim de 0 a 100mV e não 
 * se comporta lienarmente. Por isso colocamos o AmpOp com um ganho calculado para tira-lo dessa 
 * faixa de operacao.
 * 
 * Veja https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/ para saber mais
 * Há também uma discussão sobre isso no github https://github.com/espressif/arduino-esp32/issues/92
 * @date 08-08-20
 * @version 1.0
 * @date 2023-08-07
 * 
 */

#ifndef LM35_H
#define LM35_H

#include <Arduino.h>

#define ADC_VREF_MV    3300.0
#define ADC_RESOLUTION 4096
#define AMPOP_OUT      32  // conferir esse pino
#define GAIN           3.89
#define LM35_SCALE     10  // 10 mV / 1 °C

class LM35 {
private:
    float temperatura;

public:
    /**
     * @brief Atualiza o atributo temperatura do sensor LM35
     * @details Le o valor da saida do AmpOp, divide pelo ganho para saber o valor real de 
     * saida do sensor LM35 e entao converte o valor de tensao para graus celcius 
     *
    */
    void atualizaLM35();
    /**
     * @brief Imprime a temperatura na saida Serial;
    */
    void imprimir();

    float getTemperatura() { return this->temperatura; }
};

#endif
