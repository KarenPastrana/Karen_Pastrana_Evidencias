#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Reemplaza estos valores con los tuyos
const char* ssid = "iPhone";
const char* password = "nomelase";
String api_key = "20a679978be8158d6c3d4ee004dd4c61";
String city = "Puebla";
String country = "MX";

void setup() {
  Serial.begin(115200);
  
  // Conexión a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");

  // Construir la URL de la API
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + country + "&appid=" + api_key;

  // Hacer la solicitud HTTP GET
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println("Respuesta recibida:");
    Serial.println(payload);

    // Parsear la respuesta JSON
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 260;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, payload);

    float temperature_kelvin = doc["main"]["temp"];
    float temperature_celsius = temperature_kelvin - 273.15;
    String weather_description = doc["weather"][0]["description"].as<String>();

    // Mostrar la información
    Serial.printf("Temperatura actual en %s: %.2f°C\n", city.c_str(), temperature_celsius);
    Serial.printf("Descripción del clima: %s\n", weather_description.c_str());

  } else {
    Serial.printf("Error en la solicitud HTTP: %d\n", httpResponseCode);
  }

  http.end();
}

void loop() {
  // No se necesita hacer nada en loop
}