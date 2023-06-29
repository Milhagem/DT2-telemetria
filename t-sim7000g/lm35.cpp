#include "lm35.hpp"

float LM35::atualizaLM35(){
    // Le o valor de na saida do AmpOp e divide pelo ganho
    int adcValAmpOp = analogRead(AMPOP_OUT);
    int adcVal = adcValAmpOp / GAIN;
    // Converte o valor analogico p/ milivolt
    float milliVolt = adcVal * (ADC_VREF_MV/ ADC_RESOLUTION);
    // Convete a temsao para temperatura em ºC
    this->temperatura = milliVolt / LM35_SCALE;
}

void LM35::imprimir(){
    Serial.print("Temperatura: ");
    Serial.print(this->getTemperatura());
    Serial.println("°C");
}
