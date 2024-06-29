#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Reemplaza estos valores con los tuyos
const char* ssid = "iPhone de Rafael";
const char* password = "nomelase";
String api_key = "20a679978be8158d6c3d4ee004dd4c61";
String city = "Puebla";
String country = "MX";

// Declaración de la pantalla OLED
#define SCREEN_WIDTH 128 // Ancho de la pantalla OLED
#define SCREEN_HEIGHT 64 // Alto de la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long previousMillis = 0; // Almacenar el último tiempo de actualización
const long interval = 1800000;    // Intervalo de actualización (30 minutos en milisegundos)

void setup() {
  Serial.begin(115200);
  
  // Inicializar la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Dirección I2C de la pantalla OLED
    Serial.println(F("Fallo al inicializar la pantalla OLED"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // Conexión a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");

  // Llamar a la función de actualización inmediatamente
  updateWeatherData();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Verificar si ha pasado el intervalo de tiempo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Llamar a la función de actualización
    updateWeatherData();
  }
}

void updateWeatherData() {
  // Construir la URL de la API
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + country + "&appid=" + api_key;
  Serial.println("URL construida: " + url);

  // Hacer la solicitud HTTP GET
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  Serial.printf("Código de respuesta HTTP: %d\n", httpResponseCode);

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

    // Mostrar la información en el Serial Monitor
    Serial.printf("Temperatura actual en %s: %.2f°C\n", city.c_str(), temperature_celsius);
    Serial.printf("Descripción del clima: %s\n", weather_description.c_str());

    // Mostrar la información en la pantalla OLED
    display.clearDisplay();
    display.setTextSize(1);      // Tamaño del texto
    display.setTextColor(SSD1306_WHITE); // Color del texto
    display.setCursor(0, 0);     // Posición del texto
    display.printf("Ciudad: %s\n", city.c_str());
    display.printf("Temp: %.2f°C\n", temperature_celsius);
    display.printf("Clima: %s\n", weather_description.c_str());
    display.display();

  } else {
    Serial.printf("Error en la solicitud HTTP: %d\n", httpResponseCode);
    display.clearDisplay();
    display.setTextSize(1);      
    display.setTextColor(SSD1306_WHITE); 
    display.setCursor(0, 0);     
    display.println("Error en la solicitud HTTP");
    display.display();
  }

  http.end();
}
