#include "../includes/lte_connection.hpp"

void LTE_Connection::modemPowerOn(){
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(PWR_PIN, HIGH);
}

void LTE_Connection::modemPowerOff(){
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, LOW);
    vTaskDelay(1500 / portTICK_PERIOD_MS);
    digitalWrite(PWR_PIN, HIGH);
}

void LTE_Connection::modemRestart(){
    modemPowerOff();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    modemPowerOn();
}

// ---------------------------------------------------------------------------------------------------
void LTE_Connection::setupLTE() {
  // Set console baud rate
  //SerialMon.begin(115200);

  //delay(10);

  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  modemPowerOn();

  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);


  Serial.println("/**********************************************************/");
  Serial.println("To initialize the network test, please make sure your LTE ");
  Serial.println("antenna has been connected to the SIM interface on the board.");
  Serial.println("/**********************************************************/\n\n");

  vTaskDelay(10000 / portTICK_PERIOD_MS);

  String res;

  Serial.println("========INIT========");

  if (!modem.init()) {
    modemRestart();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("Failed to restart modem, attempting to continue without restarting");
    return;
  }

  Serial.println("========SIMCOMATI======");
  modem.sendAT("+SIMCOMATI");
  modem.waitResponse(1000L, res);
  res.replace(GSM_NL "OK" GSM_NL, "");
  Serial.println(res);
  res = "";
  Serial.println("=======================");

  Serial.println("=====Preferred mode selection=====");
  modem.sendAT("+CNMP?");
  if (modem.waitResponse(1000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  res = "";
  Serial.println("=======================");


  Serial.println("=====Preferred selection between CAT-M and NB-IoT=====");
  modem.sendAT("+CMNB?");
  if (modem.waitResponse(1000L, res) == 1) {
    res.replace(GSM_NL "OK" GSM_NL, "");
    Serial.println(res);
  }
  res = "";
  Serial.println("=======================");


  String name = modem.getModemName();
  Serial.println("Modem Name: " + name);

  String modemInfo = modem.getModemInfo();
  Serial.println("Modem Info: " + modemInfo);

  // Unlock your SIM card with a PIN if needed
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }

  for (int i = 0; i <= 4; i++) {
    uint8_t network[] = {
        2,  /*Automatic*/
        13, /*GSM only*/
        38, /*LTE only*/
        51  /*GSM and LTE only*/
    };
    Serial.printf("Try %d method\n", network[i]);
    modem.setNetworkMode(network[i]);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    bool isConnected = false;
    int tryCount = 60;
    while (tryCount--) {
      int16_t signal =  modem.getSignalQuality();
      Serial.print("Signal: ");
      Serial.print(signal);
      Serial.print(" ");
      Serial.print("isNetworkConnected: ");
      isConnected = modem.isNetworkConnected();
      Serial.println( isConnected ? "CONNECT" : "NO CONNECT");
      if (isConnected) {
        break;
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    if (isConnected) {
        break;
    }
  }
  digitalWrite(LED_PIN, HIGH);

  Serial.println();
  Serial.println("Device is connected .");
  Serial.println();
}

// ---------------------------------------------------------------------------------------------------
void LTE_Connection::connectGRPS() {
  // GPRS connection parameters are usually set after network registration
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  if (modem.isGprsConnected()) { SerialMon.println("GPRS connected"); }
  else { SerialMon.print("GPRS not connected"); }
}

// ---------------------------------------------------------------------------------------------------
void LTE_Connection::connectServer() {
  SerialMon.print("Connecting to ");
  SerialMon.println(server);
  if (!client.connect(server, port)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
}

// ---------------------------------------------------------------------------------------------------
void LTE_Connection::postRequest(String postData) {
  SerialMon.println("Performing HTTP POST request...");
  
  client.print(String("POST ") + resource + " HTTP/1.1\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print(String("Content-Type: application/x-www-form-urlencoded\r\n"));
  client.print(String("Content-Length: ") + String(postData.length()) + "\r\n\r\n");
  client.print(postData);
  client.print(String("Connection: close\r\n\r\n"));
  client.println();
  
  // Wait for the server's response
  uint32_t timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      SerialMon.print(c);
      timeout = millis();
    }
  }
  SerialMon.println();
}