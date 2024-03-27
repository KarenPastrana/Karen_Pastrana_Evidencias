// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5, rw=4;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16,2);
  lcd.print("Hello World !!!!");
   lcd.setCursor ( 0, 1 ); 
  lcd.print("Brenda Pastrana");
   lcd.setCursor ( 0, 2 ); 

}

void loop() {
 
}

