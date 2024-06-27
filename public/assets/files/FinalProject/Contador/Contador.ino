#include "RTClib.h" // Librería RTC
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AccelStepper.h" //Para motores

// Parte de motores
// Define I2C address of PCF8574
#define PCF8574_ADDRESS 0x20

// Define the motor control pins connected to PCF8574
#define MOTOR3_DIR_PIN 0
#define MOTOR3_STEP_PIN 1
#define MOTOR4_DIR_PIN 2
#define MOTOR4_STEP_PIN 3

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver
#define dirPin D0
#define stepPin D1
#define dirPin2 D2
#define stepPin2 D3

#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2);

// Variables para el temporizador y los pasos del motor
unsigned long previousMillis = 0;
const long interval = 1000; // Intervalo de 1 segundo
int stepsTaken = 0;

// Function to write to a pin on the PCF8574
void pcf8574Write(int pin, int value) {
  Wire.beginTransmission(PCF8574_ADDRESS);
  static uint8_t pcf8574State = 0xFF;  // All pins HIGH by default
  if (value == LOW) {
    pcf8574State &= ~(1 << pin);
  } else {
    pcf8574State |= (1 << pin);
  }
  Wire.write(pcf8574State);
  Wire.endTransmission();
}

// Function to step the motor
void stepMotor(int dirPin, int stepPin, int steps, int delayTime) {
  // Set direction
  pcf8574Write(dirPin, HIGH);  // Change HIGH/LOW depending on the direction you want
  for (int i = 0; i < steps; i++) {
    pcf8574Write(stepPin, HIGH);
    delayMicroseconds(delayTime);
    pcf8574Write(stepPin, LOW);
    delayMicroseconds(delayTime);
  }
}
// Termina configuración de motores

RTC_DS3231 rtc; // Módulo del reloj

// Definir el ancho y alto de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// Dirección I2C de la pantalla OLED
#define OLED_I2C_ADDRESS 0x3C
// Crear una instancia para la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Botón
const int buttonPin = D9; //D9
const int ledPin = D10; // Pin del LED D10
bool isRunning = false; // Estado del temporizador
bool lastButtonState = HIGH; // Último estado leído del botón
bool currentButtonState; // Estado actual leído del botón

unsigned long startTime; // Tiempo en el que se inició el temporizador
unsigned long currentTime; // Tiempo actual
unsigned long elapsedTime; // Tiempo transcurrido desde que se inició el temporizador

const unsigned long workDuration = 25 * 60 * 1000; // 25 minutos en milisegundos
const unsigned long breakDuration = 5 * 60 * 1000; // 5 minutos en milisegundos

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT); // Configurar el pin del LED como salida
  Serial.begin(9600);

  // Iniciar RTC
  if (!rtc.begin()) {
    Serial.println("No encontrado RTC");
    Serial.flush();
    while (1) delay(10);
  }

  // Iniciar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("No se pudo iniciar la pantalla OLED"));
    for (;;);
  }

  // Parte de motores
  Wire.begin();
  // Set all pins as HIGH initially
  pcf8574Write(MOTOR3_DIR_PIN, HIGH);
  pcf8574Write(MOTOR3_STEP_PIN, HIGH);
  pcf8574Write(MOTOR4_DIR_PIN, HIGH);
  pcf8574Write(MOTOR4_STEP_PIN, HIGH);

   // Set the maximum speed and acceleration for the first motor:
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);  // Set lower acceleration for smoother movement

  // Set the maximum speed and acceleration for the second motor:
  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);  // Set lower acceleration for smoother movement
  // Termina parte de motores
}

void loop() {
  DateTime tiempo = rtc.now();

  char formato1[] = "hh:mm:ss ap";
  Serial.println(tiempo.toString(formato1));

  Serial.println();
  
  // Temperatura
  Serial.print("Temperatura: ");
  Serial.print(rtc.getTemperature());
  Serial.println(" C");
  Serial.println();

  // Limpiar la pantalla buffer
  display.clearDisplay();
  
  // Imprimir "Temp:" en tamaño 2
  display.setTextSize(2); // Tamaño del texto ajustado a 2
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Temp: "));

  // Imprimir la temperatura en tamaño 3
  display.setTextSize(3); // Tamaño del texto ajustado a 3
  display.print((int)rtc.getTemperature()); // Convertir la temperatura a entero

  // Imprimir "*C" en tamaño 2
  display.setTextSize(2);
  display.println(F("C"));

  // Obtener el estado actual del botón
  currentButtonState = digitalRead(buttonPin);

  // Detecta si el botón ha sido presionado (transición de LOW a HIGH)
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    isRunning = !isRunning; // Alterna el estado del temporizador
    if (isRunning) {
      startTime = millis(); // Guarda el tiempo actual como el inicio del temporizador
      previousMillis = startTime; // Resetea el temporizador para el motor
      stepsTaken = 0; // Resetea los pasos del motor
    } else {
      digitalWrite(ledPin, LOW); // Apagar el LED cuando el temporizador se detenga
    }
  }

  // Actualiza el último estado del botón
  lastButtonState = currentButtonState;

  // Si el temporizador está en funcionamiento, mostrar el tiempo restante
  if (isRunning) {
    currentTime = millis();
    elapsedTime = currentTime - startTime;

    // Calcular el tiempo restante
    unsigned long remainingTime;
    if (elapsedTime < workDuration) {
      remainingTime = workDuration - elapsedTime;
      Serial.print("Tiempo de trabajo restante: ");
    } else if (elapsedTime < workDuration + breakDuration) {
      remainingTime = workDuration + breakDuration - elapsedTime;
      Serial.print("Tiempo de descanso restante: ");
    } else {
      // Reiniciar el ciclo después del descanso
      startTime = millis();
      elapsedTime = 0;
      remainingTime = workDuration;
      Serial.print("Tiempo de trabajo restante: ");
    }

    // Encender el LED cuando queden 5 segundos restantes
    if (remainingTime <= 5000) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }

    // Mostrar el tiempo restante en el monitor serial
    unsigned long minutes = (remainingTime / 1000) / 60;
    unsigned long seconds = (remainingTime / 1000) % 60;
    Serial.print(minutes);
    Serial.print(" minutos, ");
    Serial.print(seconds);
    Serial.println(" segundos");

    // Mostrar el tiempo restante en la pantalla OLED
    display.setTextSize(2); // Tamaño del texto ajustado a 2
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.print(minutes);
    display.print("m ");
    display.print(seconds);
    display.println("s");

    // Parte de motores
    // Obtener el tiempo actual
    unsigned long currentMillis = millis();

    // Verificar si es tiempo de mover el motor
    if (currentMillis - previousMillis >= interval) {
      // Guardar el último tiempo que movimos el motor
      previousMillis = currentMillis;

      // Mover el primer motor 20 pasos
      stepper.move(20);
      stepsTaken += 20;

      // Ejecutar el movimiento del motor hasta alcanzar la posición objetivo
      while (stepper.distanceToGo() != 0) {
        stepper.run();
      }

      // Verificar si hemos completado 200 pasos
      if (stepsTaken >= 200) {
        // Resetear el contador de pasos
        stepsTaken = 0;

        // Mover el tercer motor 20 pasos con un retardo de 1000 microsegundos entre pasos
        stepMotor(MOTOR3_DIR_PIN, MOTOR3_STEP_PIN, 20, 1000);
      }
    }
    // Termina parte de motores
  }

  // Imprimir la hora en tamaño 1
  display.setTextSize(1); // Tamaño del texto ajustado a 1
  display.setCursor(0, 50);
  display.println(tiempo.toString(formato1));

  // Enviar el buffer a la pantalla
  display.display();
}
