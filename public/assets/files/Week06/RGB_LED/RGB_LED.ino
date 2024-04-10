#include <Adafruit_NeoPixel.h>

int Power = 11;  // Pin necessary to turn on the RGB
int PIN = 12;    // Pin necessary to program RGB

int currentColorIndex = 0;  // Index of the current color
int frequency = 1000;      // Initial PWM frequency

#define NUMPIXELS 1  // Define the number of rows of the Neopixel

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // Necessary to program the neopixel

void setup() {
  pixels.begin();
  pinMode(Power, OUTPUT);  // Pin to turn on the Neopixel
  digitalWrite(Power, HIGH);

  Serial.begin();
  Serial.setTimeout(10);
}

void loop() {
  // Increment the index of the color
  currentColorIndex++;

  // Reset the index when it reaches the total number of colors
  if (currentColorIndex >= 5) {
    currentColorIndex = 0;
  }

  // Change the color of the NeoPixel according to the index
  pixels.clear();  // Function to program to change the colors of the RGB
  switch (currentColorIndex) {
    case 0:
      pixels.setPixelColor(0, pixels.Color(15, 25, 205));  // Blue
      break;
    case 1:
      pixels.setPixelColor(0, pixels.Color(103, 25, 205));  // Purple
      break;
    case 2:
      pixels.setPixelColor(0, pixels.Color(233, 242, 205));  // Yellow
      break;
    case 3:
      pixels.setPixelColor(0, pixels.Color(233, 23, 23));    // Red
      break;
    case 4:
      pixels.setPixelColor(0, pixels.Color(12, 66, 101));    // DarkBlue
      break;
  }

  pixels.show();

  // Increment the frequency for the next color change
  frequency += 100;
  if (frequency > 600) {
    frequency = 200;  // Reset the frequency when it reaches 2000 Hz
  }
 
  delay(200);  // Wait for a brief time to avoid rapid color changes
}


