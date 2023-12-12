### Telemetria

Repositorio criado para teste de sensores de tensão da telemetria
- Utilizei sensor acs712 para fazer testes com codigo do display 
- O sensor que irá ser utilizado é o encoder que lê tensao por rpm

------------------------------------------------------------------------

### Sensor ACS712
## Funcionalidades:

- mede corrente corrente alternada e contínua (AC ; DC) por meio do efeito hall
- Este CI possui isolamento de tensão de 2,1kV RMS juntamente com um condutor de corrente de baixa resistência
- Ele Possui recursos de cancelamento de ruído e tempo de resposta muito alto.
- utiliza seu condutor para calcular e medir a quantidade de corrente aplicada, alcançando excelentes resultados

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/b1718b30-6b37-4b94-b7b4-f314ca08a26d/f9118c87-c495-41c2-a18f-8e59cd807c4a/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/b1718b30-6b37-4b94-b7b4-f314ca08a26d/c0df525b-1443-4d61-9eb8-26bac4a5365a/Untitled.png)

| Pino | Descrição |
| --- | --- |
| Vcc | A tensão de entrada é +5V para aplicações típicas |
| Out | Saídas Tensão analógica proporcional à corrente |
| Gnd | Conectado ao terra do circuito |
| Terminal de conexão | O fio onde passa a ser medida deve ser conectado aqui |

## Característica do Sensor de Corrente ASC712

- Largura de banda de 80kHz;
- Sensibilidade de saída de 66 a 185 mV/A;
- Sinal analógico de baixo ruído;
- Resistência interna do condutor de 1,2 mΩ;
- Erro total de saída de 1,5% em TA = 25°C;
- Tensão de compensação de saída estável;
- Histerese magnética próxima de zero.

---

---

| Tipo de ACS712 | TA (°C ) | Faixa Otimizada, I P                   (A) | Sensibilidade, Sens(Typ) (mV/A) |
| --- | --- | --- | --- |
| 3 | https://www.usinainfo.com.br/sensor-de-corrente-arduino/sensor-de-corrente-acs712-30a-ac-dc-com-efeito-hall-2952.html | -40 a 85 | ±30 |

## Principais Aplicações do Sensor ACS712

- Controle de velocidade do motor por meio do consumo;
- Detecção e gerenciamento de carga elétrica;
- Fontes de alimentação de modo comutado (SMPS);
- Proteção contra sobrecorrente.

## Funcionamento do Sensor de Corrente ACS712

O sensor de corrente acs712 consiste em um circuito de efeito hall linear junto com terminais de condução de cobre. O caminho de condução em cobre está localizado ao redor da superfície da matriz. Quando a corrente alternada ou contínua passa pelos condutores de cobre, produz um campo magnético, é assim é possível medir o consumo.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/b1718b30-6b37-4b94-b7b4-f314ca08a26d/fbfffb5f-fabd-40f9-9f4f-e7404100abff/Untitled.png)

Este campo eletromagnético interage com o sensor de efeito hall. O circuito de efeito Hall converte este campo eletromagnético em tensão proporcional CA ou CC, dependendo do tipo de corrente de entrada
