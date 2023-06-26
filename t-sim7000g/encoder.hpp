#ifndef ENCODER
#define ENCODER_H

#define MEASURE_PIN            25
#define WHEEL_CIRCUMFERANCE 1.596/12
#define SAMPLES                25

class encoder {
private:
    double speed;           // km/h
    double average_speed;   // km/h
    double distancia_total; // km
    double tempo_total;     // segundos
    bool ja_andou;          
      
public:
    /**
     * @brief Calcula voltas totais e voltas por segundo da roda
    */
    void amostraVoltas();
     /**
     * @brief Calcula a velocidade com base nas rotacoes por segundo e diametro da roda
    */
    void calculaVelocidade(double rps, double wheel_diameter);
    void imprimir();
}; 

#endif