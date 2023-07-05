#ifndef ENCODER
#define ENCODER_H

#define MEASURE_PIN         25
#define WHEEL_CIRCUMFERANCE 1.596/12
#define SAMPLES             25
#define ms_TO_min           0.00001666666 // milisegundos para minutos

class Encoder {
private:
    double speed;           // km/h
    double average_speed;   // km/h
    double distancia_total; // km
    double tempo_total;     // segundos

    /**
     * Variaveis que serao usadas em void calculaVelocidade(double rps, double wheel_diameter)
    */
    double tempo_inicio;      // segundos
    double tempo_speed_old;   // segundos
    double tempo_speed_atual; // segundos
    double tempo_delta;       // segundos
    double tempo_total;       // segundos
    double speed_old;         // m/s
    double distancia_trecho;  // metros
    bool ja_andou;
    
        
public:
    /**
     * @brief Esta funcao deve ser chamada no setup() do t-sim7000g para iniciar o Encoder
    */
    void encoderSetup();
    /**
     * @brief Calcula voltas totais e voltas por segundo da roda
     * 
     * @return rps
    */
    double amostraVoltas();
     /**
     * @brief Calcula a velocidade com base nas rotacoes por segundo e diametro da roda
    */
    void calculaVelocidade(double rps, double wheel_diameter);
    /**
     * @brief Imprime os atributos do Encoder (speed, average_speed, distancia_total e tempo_total) na saida Serial
    */
    void imprimir();
    // km/h
    double getSpeed() { return this->speed; }
    // km/h
    double getAverageSpeed() { return this->average_speed; }
}; 

#endif