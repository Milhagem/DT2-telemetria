#include "velocidade_motor.hpp"

Velocidade_Motor* Velocidade_Motor::instance = nullptr;

void Velocidade_Motor :: calculo_velocidade (){
    tempo_atual = micros();
    if (conta_motor == HIGH){
        // Utilizando a formula 3.6*(Circunferencia/Periodo), podemos encontrar a velocidade instantanea em Km/h
        velocidade_instantanea_motor = 3.6*(circunferencia_do_motor/diferenca_pulsos_motor*10e-6*imas_motor);
        
        // Atualizar o tempo anterior para o atual para termos uma maior precisao no proximo calculo
        tempo_anterior_motor = tempo_atual;

        // Mudar o estado do conta_motor e o contagem_motor para 0 
        // para que o calculo_intervalo_hall seja feito corretamente
        conta_motor = LOW;
        contagem_motor = 0;
    }
}

void Velocidade_Motor::hallInterruptHandler() {
    if (instance) {
        instance->calculo_intervalo_hall();
    }
}

void Velocidade_Motor :: calculo_intervalo_hall (){
    // Se o codigo entrou aqui, eh porque o estado do hall mudou, entao aumentamos em 1 a contagem do motor
    contagem_motor++;

    // Se o hall teve uma sequencia, de por exemplo 1 0 1, ele entra na funcao para permitir o calculo da velocidade
    if (conta_motor % 2 == 0){
        
        // Coloca o tempo atual na variavel para que o delta t seja calculado
        tempo_anterior1_motor = micros();

        // Calculo do delta t ou do 1/4 do periodo 
        // Utiliza o tempo anterior que foi o tempo atual quando o if do conta motor nao foi satisfeito 
        diferenca_pulsos_motor = tempo_anterior1_motor - tempo_anterior_motor;

        // Permite que o calculo da velocidade seja feito 
        conta_motor = HIGH;

    } else {
        // Marca o tempo quando a sequencia nao eh satisfeita
        tempo_anterior_motor = micros();
    }
}

void Velocidade_Motor :: motor_setup(){
    
    // Cria um serial com sua "velocidade de leitura"
    Serial.begin(115200);

    // Inicia o tempo atual como 0
    tempo_atual = 0;

    // Inicia a contagem dos intervalos de, por exemplo, 0 e 1 como 0 
    contagem_motor = 0;

    // Inicia o tempo anterior do motor como 0
    tempo_anterior_motor = 0;

    // Inicia a velocidade instantanea como 0
    velocidade_instantanea_motor = 0;

    // Le o estado logivco do pino do hall
    pinMode(hall_motor, INPUT);

    instance = this;

    // Funcao que fica vigiando a mudanca de estado do hall para a entrada na função do calculo do intervalo
    attachInterrupt(digitalPinToInterrupt(hall_motor), hallInterruptHandler, CHANGE);
}

void Velocidade_Motor :: imprimir (){
    Serial.print ("Velocidade:  ");
    Serial.print (this->calculo_velocidade_instantanea_motor);
    Serial.println (" km/h ");
    Serial.print ("Estado logico: ");
    Serial.println (digitalRead(hall_motor));
}

