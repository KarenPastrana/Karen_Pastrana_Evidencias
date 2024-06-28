#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

#include "index.h"


WebServer server(80);

const char* ssid= "KODA(3)";
const char* password ="1234567890";

const char* apiKey = "6045d9ed8fd2eb77ca8a713b4f2c5ffa";
const char* lat ="19.029435";
const char* lon ="-98.241717";



void setup() {

  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() !=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());

  String url = String("http://api.openweathermap.org/data/2.5/weather?lat=") + lat + "&lon=" + lon + "&appid=" + apiKey;
  // Hacer la solicitud HTTP
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      // Verificar que la solicitud fue exitosa
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("Error en la solicitud HTTP: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Error al conectar al Wi-Fi");
  }

}

void loop() {



}
