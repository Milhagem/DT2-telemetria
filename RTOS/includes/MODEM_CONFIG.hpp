/**
 * Arquivo de configuracao do TINY_GSM_MODEM_SIM7000
 * 
 * Este arquivo deve ser incluido onde eh necessario usar o Modem, como por exemplo no GPS na 
 * comunicacao LTE. Isso possibilita a modularizacao pois evita Multiple Definition
 * 
 * Aqui eh setado qual o modem da placa, ou seja, TINY_GSM_MODEM_SIM7000; a pinagem dela; comandos
 * SerialAT
 * 
 * Em MODEM_CONFIG apenas sao criados os objetos TinyGsm modem e TinyGsm client
*/

#ifndef MODEM_CONFIG_H
#define MODEM_CONFIG_H

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#include <TinyGsmClient.h>

// LilyGO T-SIM7000G Pinout
#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands
#define SerialAT  Serial1

#endif
