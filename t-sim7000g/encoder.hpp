#ifndef ENCODER
#define ENCODER_H

#define MEASURE_PIN            25
#define WHEEL_CIRCUMFERANCE 1.596 / 12
#define SAMPLES                25

class velocidade {
private:

    volatile sample_t* pSample;
    uint8_t n;
    uint8_t totalRevolutions;
    uint8_t totalSamples;
    uint16_t minMs;
    uint16_t maxMs;
    double rps;
    double rpm;
    double wheel_diameter; 

    double tempo_Inicio = 0;
    double tempo_speedVelho = 0;
    double tempo_speedAtual = 0;
    double tempo_dif = 0;
    double tempo_total = 0;
    double speed = 0;
    double speed_mpm = 0;
    double speed_velho = 0;
    double average_speed = 0;
    double distancia_trecho = 0;
    double distancia_total = 0;
    double ms_to_min =0.00001666666;
    bool andou = 0;
      
public:
    float resolucao();
    void param_velo();
}; 

#endif