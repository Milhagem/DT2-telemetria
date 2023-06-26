#ifndef LM35_H
#define LM35_H

#define ADC_VREF_MV    3300.0
#define ADC_RESOLUTION 4096
#define AMPOP_OUT      32
#define GAIN           3.89
#define LM35_SCALE     10

class lm35 {
private:
    float temperatura;

public:
    float medeTemperatura();
    void imprimeTemperatura();

    float getTemperatura() { return this->temperatura; }
};

#endif