#include "httpclient.hpp"

httpclient::httpclient(const char* ssid, const char* password, const char* serverName, String apiKeyValue) {
    this->ssid = ssid;
    this->password = password;
    this->serverName = serverName;
    this->apiKeyValue = apiKeyValue;
}


void httpclient::wifiSetup(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("WL_NOT_CONNECTED");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}


int httpclient::sendData(String httpRequestData) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(client, serverName);

        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);

        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            return httpResponseCode;
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
            return httpResponseCode;
        }
        // Free resources
        http.end();

    } else {
        Serial.println("WiFi Disconnected. Attempting to connect again");
        WiFi.begin(ssid, password);
        Serial.println("Connecting");

        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("WL_NOT_CONNECTED");
        }

        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());;
    }
}
