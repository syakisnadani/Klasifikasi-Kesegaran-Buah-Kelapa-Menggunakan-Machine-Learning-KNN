#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;


HX711 scale;

void setup() {
  Serial.begin(38400);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
  
  Serial.println("Readings:");
}

void loop() {
  Serial.print("reading:\t");
  Serial.print(scale.get_units(23)/100, 2);
  Serial.println();
  scale.power_down();             // put the ADC in sleep mode
  delay(20);
  scale.power_up();
}
