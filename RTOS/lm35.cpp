#include "lm35.hpp"

void LM35::atualizaLM35(){
    // Le o valor de na saida do AmpOp e divide pelo ganho
    int adcValAmpOp = analogRead(AMPOP_OUT);
    float  adcVal = adcValAmpOp / GAIN;
    // Converte o valor analogico p/ milivolt
    float milliVolt = adcVal * (ADC_VREF_MV / ADC_RESOLUTION);
    // Converte a tensao para temperatura em °C
    this->temperatura = milliVolt / LM35_SCALE;
}

void LM35::imprimir(){
    Serial.print("Temperatura: ");
    Serial.print(this->getTemperatura());
    Serial.println("°C");
}
