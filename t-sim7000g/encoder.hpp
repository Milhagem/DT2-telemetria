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
    bool ja_andou;

    /**
     * @brief Struct para armazenar variaveis que serao usadas em void calculaVelocidade(double rps, double wheel_diameter)
    */
    typedef struct {
        double tempo_inicio;
        double tempo_speed_old;
        double tempo_speedAtua;
        double tempo_delta;
        double tempo_total;
        double speed_old;
        double distancia_trecho;
        bool ja_andou;
    } calc_vel;
        
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
    void imprimir();
}; 

#endif