#include "gps.hpp"

void GPS::setupGPS() {
  SerialMon.println("Place your board outside to catch satelite signal");

  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // Turn on the modem
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);

  delay(1000);
  
  // Set module baud rate and UART pins
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  if (!modem.init()) {
    SerialMon.println("Failed to init modem, attempting to continue");
  }
}

void GPS::liberaGPS() {

  // Set SIM7000G GPIO4 HIGH ,turn on GPS power
  // CMD:AT+SGPIO=0,4,1,1
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,1");
  if (modem.waitResponse(10000L) != 1) {
    SerialMon.println(" SGPIO=0,4,1,1 false ");

  }

  modem.enableGPS();
}

void GPS::atualizaGPS() {

  for (int8_t i = 15; i; i--) {
    SerialMon.println("Requesting current GPS/GNSS/GLONASS location");
    if (modem.getGPS(&(this->lat), &(this->lon), &(this->speed), &(this->alt), &(this->vsat), &(this->usat), &(this->accuracy),
                     &(this->year), &(this->month), &(this->day), &(this->hour), &(this->min), &(this->sec))) {
      this->imprimir();
      break;
    } 
    else {
      SerialMon.println("Couldn't get GPS/GNSS/GLONASS location, retrying in 1s.");
      // Wait 1ms
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
  }  
}

void GPS::imprimir() {
  SerialMon.println("Latitude: " + String(this->lat, 8) + "\tLongitude: " + String(this->lon, 8));
  SerialMon.println("Speed: " + String(speed) + "\tAltitude: " + String(alt));
  SerialMon.println("Visible Satellites: " + String(this->vsat) + "\tUsed Satellites: " + String(this->usat));
  //SerialMon.println("Accuracy: " + String(this->accuracy));
  SerialMon.println("Year: " + String(this->year) + "\tMonth: " + String(this->month) + "\tDay: " + String(this->day));
  SerialMon.println("Hour: " + String(this->hour) + "\tMinute: " + String(this->min) + "\tSecond: " + String(this->sec));
}

String GPS::getTimestamp() {
  String timestamp = String(this->year) + "-" + String(this->month) + "-" + String(this->day) + " " +
                     String(this->hour) + ":" + String(this->min) + ":" + String(this->sec);
  return timestamp;
}
