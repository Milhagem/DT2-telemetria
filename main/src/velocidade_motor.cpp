// #include "velocidade_motor.hpp"

// /**
//  * @note Esse é o arquivo .cpp vinculado à classe velocidade_motor
//  * Praticamente todos os métodos responsáveis pelo calculo da velocidade estão localizados aqui
//  * em vista o motivo já declarado no arquivo .cpp
// */

// #define circunferencia_do_motor 0.05135  // Circunferencia do motor em m

// volatile bool pulsos;         // Serve para contar se houve uma sequencia efetiva de, por exemplo, 1 0 1
// bool conta = false;           // Controla quando o calculo da velocidade eh realizado
// unsigned long tempNow = 0;    // Tempo em microsegundos medido no operador da velocidade
// unsigned long temp1 = 0;      // (Momento anterior ao pulso anterior)
// unsigned long temp2 = 0;      // Atualiza toda vez que um pulso eh detectado (momento anterior ao pulso atual)
// unsigned long deltaT = 0;     // Delta t
// float velocidade = 0;         // Armazena o valor de velocidade em km/h


// /**
//  * @brief As variáveis a baixo sao responsaveis pelo calculo da média móvel
//  * Esse tipo de filtro dos valores obtidos da velocidade foi necessário, pois, infelizmente, a funcao 
//  * que detecta a variacao dos pulsos nao funciona direito com variacoes quase que instantaneas e alguns
//  * valores saem com uma variacao muito discrepante de outros 
// */
// /**
//  * Essa variavel armazena a quantidade de resultados da velocidade que soferao uma media 
//  * Esse valor pode ser alterado caso necessario, mas se um valor muito grande for colocado,
//  * o valor da velocidade perdera precisao em valor e em tempo, uma vez que os dados demorarao
//  * de ser informados. Esse método ainda não é o mais eficiente, uma vez que os valores quando
//  * passam pela media estao tendo uma variacao relativamente significativa por causa dos valores
//  * fora da realidade causados pelo delay do attachinterrupt
//  * Caso exista algum outro método de filtar o código e acabar com os valores fora da realidade
//  * o código pode ser alterado facilmente mexendo na funcao imprimir e adicionando novos metodos
// */
// const int tamanhoMediaMovel = 14;

// // Armazena os valores da velocidade
// float ultimosValores[tamanhoMediaMovel];

// int indiceAtual = 0;


// /**
//  * @brief Essa funcao é responsavel por fazer a troca dinamica das variaveis que armazenam os valores
//  * de tempo para o calculo do delta t
//  * Ela tambem depende diretamente da funcao que calcula a velocidade
//  * Ela está atrelada ao AttachInterrupt, ou seja, toda vez que uma variacao do sinal é percebida pelos
//  * sensores hall do motor, essa funcao é chamada
//  * E, por causa dela, a maioria dos métodos nao foram declarados no .hpp, pois essa funcao precisava ser
//  * global
// */
// void contador() {
//    pulsos++;
//    if (conta){
//       temp1 = micros();
//       deltaT = temp1 - temp2;
//       conta = false;
//    } else {
//       temp2 = micros();
//    }
// }


// /**
//  * @brief Essa funcao recebe o valor do delta t e com base em calculos fisicos, lanca o valor
//  * da velocidade e habilita os if da funcao contador()
//  * Um problema a ser relatado a respeito do calculo é em relacao ao deltaT; os autores originais
//  * do codigo da velocidade do motor nao especificaram direto que periodo seria o do deltaT, de uma
//  * volta completa ou parcial, por exemplo; Existe uma variavel no codigo original chamada imas motor que perticipava
//  * do calculo da velocidade, mas quando ela foi aplicada nesse codigo, os valores ficaram fora da realidade
//  * e sem ela os resultados foram melhores. É preciso fazer um estudo mais a fundo sobre isso
// */
// void calcularVelocidade() {
//     if (!conta) {
//         velocidade = 3.6 * 10e4 * circunferencia_do_motor * PI / deltaT ;
//         pulsos = false;
//         conta = true;
//     } else {
//         velocidade = 0;
//     }
// }

// /**
//  * Esse método realiza a média móvel
// */
// float mediaMovel() {
//     float soma = 0;
//     for (int i = 0; i < tamanhoMediaMovel; i++) {
//         soma += ultimosValores[i];
//     }
//     return soma / tamanhoMediaMovel;
// }

// /**
//  * Esse método pertence a média movel
// */
// void adicionarValorAoBuffer(float valor) {
//     ultimosValores[indiceAtual] = valor;
//     indiceAtual = (indiceAtual + 1) % tamanhoMediaMovel;
// }

// void Velocidade_Motor::motor_setup() {
//     //Serial.begin(115200);
//     // Le o estado logico do pino do hall
//     pinMode(hall_motor, INPUT_PULLUP);

//     // Funcao que fica vigiando a mudanca de estado do hall para a entrada na função do calculo do intervalo
//     attachInterrupt(digitalPinToInterrupt(hall_motor), contador, FALLING);
// }

// void Velocidade_Motor::imprimir() {
//     calcularVelocidade();
//     // Adiciona o valor atual ao buffer
//     adicionarValorAoBuffer(velocidade);
//     // Calcula a média móvel
//     float media = mediaMovel(); 
//     // Imprime o valor da velocidade
//     if (isfinite(media)) {
//     Serial.print(media, 4);
//     Serial.println(" km/h ");
//   }
// }