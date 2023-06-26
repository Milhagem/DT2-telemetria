#ifndef ENCODER
#define ENCODER_H

#define MEASURE_PIN            25
#define WHEEL_CIRCUMFERANCE 1.596/12
#define SAMPLES                25

class encoder {
private:

      
public:
    /**
     * @brief Calcula voltas totais da roda e voltas por segundo
    */
    float amostrarVoltas();
    void imprimir();
}; 

#endif