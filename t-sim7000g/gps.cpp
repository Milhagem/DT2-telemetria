#include "gps.hpp"

void GPS::setupGPS() {
  Serial.println("Place your board outside to catch satelite signal");

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
  Serial.println("Initializing modem...");
  if (!modem.init()) {
    Serial.println("Failed to init modem, attempting to continue");
  }
}

void GPS::atualizaGPS {
  // Set SIM7000G GPIO4 HIGH ,turn on GPS power
  // CMD:AT+SGPIO=0,4,1,1
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,1");
  if (modem.waitResponse(10000L) != 1) {
    SerialMon.println(" SGPIO=0,4,1,1 false ");
  }

  this->modem.enableGPS();

  for (int8_t i = 15; i; i--) {
    SerialMon.println("Requesting current GPS/GNSS/GLONASS location");
    if (this->modem.getGPS(&(this->lat), &(this->lon), &(this->speed), &(this->alt), &(this->vsat), &(this->usat), &(this->accuracy),
                     &(this->year), &(this->month), &(this->day), &(this->hour), &(this->min), &(this->sec))) {
      break;
    } 
    else {
      SerialMon.println("Couldn't get GPS/GNSS/GLONASS location, retrying in 15s.");
      delay(15000L);
    }
  }  
}

void GPS::imprimir() {
  Serial.println("Latitude: " + String(this->lat, 8) + "\tLongitude: " + String(this->lon, 8));
  Serial.println("Speed: " + String(speed) + "\tAltitude: " + String(alt));
  Serial.println("Visible Satellites: " + String(this->vsat) + "\tUsed Satellites: " + String(this->usat));
  Serial.println("Accuracy: " + String(this->accuracy));
  Serial.println("Year: " + String(this->year) + "\tMonth: " + String(this->month) + "\tDay: " + String(this->day));
  Serial.println("Hour: " + String(this->hour) + "\tMinute: " + Stringthis->(min) + "\tSecond: " + String(this->sec));
}

