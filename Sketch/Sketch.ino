#include "Arduino.h"
#include "TM1637Display.h"
#include "cactus_io_AM2302.h"

// Temperature Sensor
#define AM2302_PIN 2
// Display
#define CLK 9
#define DIO 8

// SEGMENT TEXT
const uint8_t SEG_HELO[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,           // H
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_D | SEG_E | SEG_F ,                          // L
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F    // O
  };
const uint8_t SEG_EROR[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_E | SEG_G,                                   // r 
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_E | SEG_G,                                   // r
  };

// Initialize
AM2302 dht(AM2302_PIN);
TM1637Display display(CLK, DIO);

uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

void setup() {
  Serial.begin(9600); 
  dht.begin();
  display.setBrightness(0x0f);
  display.setSegments(SEG_HELO);
  delay(3000);
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(dht.temperature_C)) {
    Serial.println("DHT sensor read failure!");
    display.setSegments(SEG_EROR);
    return;
  }
 
  Serial.print(" Temp: "); Serial.print(dht.temperature_C); Serial.println(" *C\t");

  int temp_c = round(dht.temperature_C);
  if(temp_c<0) {
    temp_c*=-1;
    data[0] = 0x40; // -
    data[1] = (temp_c/10)?display.encodeDigit(temp_c/10):0x00; // IF 0 DON'T SHOW
    data[2] = display.encodeDigit(temp_c%10);
    data[3] = 0x00; // BLANK
    display.setSegments(data);
    delay(2000);
    //
    data[0] = 0x00;
    data[1] = 0x63; // °
    data[2] = 0x39; // C
    data[3] = 0x00;
    display.setSegments(data);
    delay(1000);    
  } else {
    data[0] = (temp_c/10)?display.encodeDigit(temp_c/10):0x00;
    data[1] = display.encodeDigit(temp_c%10);
    data[2] = 0x63; // °
    //data[2] = 0x00; // BLANK
    data[3] = 0x39; // C
    display.setSegments(data);
    delay(3000);
  } 
  
}
