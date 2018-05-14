#include "Arduino.h"
#include "TM1637Display.h"
#include "cactus_io_AM2302.h"

// Temperature Sensor
#define AM2302_PIN 2
// Display
#define CLK 9
#define DIO 8

// Initialize
AM2302 dht(AM2302_PIN);
TM1637Display display(CLK, DIO);
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

void setup() {
  Serial.begin(9600); 
  dht.begin();
  display.setBrightness(0x0f);
  display.setSegments(data);
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(dht.temperature_C)) {
    Serial.println("DHT sensor read failure!");
    return;
  }
 
  Serial.print(" Temp: "); Serial.print(dht.temperature_C); Serial.println(" *C\t");

  int temp_c = round(dht.temperature_C);
  data[0] = display.encodeDigit(temp_c/10);
  data[1] = display.encodeDigit(temp_c%10);
  data[2] = 0x00; // BLANK
  data[3] = 0x39; // C
  display.setSegments(data);
  
  delay(5000);
}
