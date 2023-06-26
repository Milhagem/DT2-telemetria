#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <HTTPClient.h>
#include <WiFi.h>

class httpclient {
private:
    const char* ssid;
    const char* password;
    const char* serverName;
    String apiKeyValue;

public:
    httpclient(const char* ssid, const char* password, const char* serverName, String apiKeyValue);
    void wifiSetup(const char* ssid, const char* password);
    /**
     * @brief Faz a requisicao http POST
     * 
     * @param httpRequestData requisicao no formato: "api_key=" + apiKeyValue + "&lat=" + String(lat, 8) + "&lng=" + "&reading_time=" + reading_time + ""
     * 
     * @return  Codigo de resposta HTTP
    */
    String sendData(String httpRequestData);
};

#endif