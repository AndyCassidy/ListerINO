/*
  Listerino
 By Andy Cassidy
 
 Counts RPM and temperature of a 1940 Lister D 1.5HP engine.
 Board = Arduino Mega2650 (from Sunfounder)
 Or Arduino UNO with ATMEGA 328p (32kb max memory)
 Or Arduino nano with ATmega 328p (30kb max memory)
 
 -RPM is measured by a 55110 hall effect sensor which sees a magnet on the flywheel,
 once per rotation and sends an interrupt signal to pin 3 (interrupt 1), to increment 
 a counter. Once this counter reaches a sample size (maxSample), the RPM is calculated.
 -Temperature is from a DSB10B20 waterproof sensor dipped into the cooling hopper and
 wired into arduino digital pin 3.
 Output is to a 16x2 LCD screen with a trim pot for contrast adjustment.
 
 */

// include the library code:
#include <LiquidCrystal.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
// Some definitions
#define ONE_WIRE_BUS 2 
#define maxSample 8

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
volatile byte magnet_count;
unsigned long rpm;
unsigned long timeold;
byte customChar[8] = {
  0b11100,
  0b10100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(1,0);
  lcd.print("LISTERINO V1.0");
  delay(1000);
  lcd.setCursor(3,1);
  lcd.print("By Andy C");
  delay(1800);

  sensors.begin(); 
  attachInterrupt(1, magnet_detect, RISING);  //Initialize the intterrupt pin (Arduino digital pin 2)
  magnet_count = 0;
  rpm = 0;
  timeold = 0;
 // Serial.begin(115200);
  lcd.createChar(0, customChar);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RPM:");
  lcd.setCursor(0,1);
  lcd.print("TEMP:");
  lcd.setCursor(6, 0);
  lcd.print(" START ");
  delay(1000);
  lcd.setCursor(6, 0);
  lcd.print(" ENGINE ");
  delay(1000);
    lcd.setCursor(6, 0);
  lcd.print("        ");

}

void loop() {


  lcd.setCursor(7, 1);
  sensors.requestTemperatures();
  lcd.print(sensors.getTempCByIndex(0)); 
  lcd.setCursor(13, 1);
  lcd.write((uint8_t)0);
  lcd.setCursor(14, 1);
  lcd.print("C");


  // rpm now
  int maxSampleNo = maxSample;
 // Serial.println(magnet_count);

  if ((magnet_count <= maxSampleNo) && (millis()-timeold) >= 5000) { 
    lcd.setCursor(6, 0);
    lcd.print(" 0     ");
    delay(1000);
    magnet_count = 0;
    timeold = millis();
  }

  else  if (magnet_count >= maxSampleNo) { 
    rpm = (60000/(millis() - timeold))*magnet_count;
    lcd.setCursor(6, 0);
    lcd.print("          ");
    lcd.setCursor(6, 0);
    lcd.print(rpm,DEC);
    timeold = millis();
    magnet_count = 0;
  }


}

void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
  magnet_count++;

}



