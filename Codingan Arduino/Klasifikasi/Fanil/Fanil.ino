#include <LiquidCrystal.h>
#include <HX711_ADC.h>


#define RS 7
#define E  6
#define D4 5
#define D5 4
#define D6 3
#define D7 2
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#define s0 8       //Module pins wiring
#define s1 9
#define s2 10
#define s3 11
#define out 12

const int HX711_dout = A1; //mcu > HX711 dout pin
const int HX711_sck = A0; //mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_calVal_eepromAdress = 0;
long t;

int data = 0;      //This is where we're going to stock our values

void setup()
{
  lcd.begin(16,2);
  pinMode(s0, OUTPUT);   //pin modes
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  Serial.begin(57600);   //intialize the serial monitor baud rate
  Serial.println("Starting...");

  float calibrationValue; // calibration value
  calibrationValue = 216.0;

  LoadCell.begin();
  long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration factor (float)
    Serial.println("Startup is complete");
  }

  while (!LoadCell.update());
  Serial.print("Calibration value: ");
  Serial.println(LoadCell.getCalFactor());
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell.getSPS());
  Serial.print("HX711 measured settlingtime ms: ");
  Serial.println(LoadCell.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell.getSPS() < 7) {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  }
  else if (LoadCell.getSPS() > 100) {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }

  digitalWrite(s0, HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
  digitalWrite(s1, HIGH); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
//  lcd.init(); // initialize the lcd
//  lcd.backlight();

}

void loop()                  //Every 2s we select a photodiodes set and read its data
{
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; //increase value to slow down serial print activity

//   check for new data/start next conversion:
 // if (LoadCell.update()) newDataReady = true;

 // // get smoothed value from the dataset:
  //if (newDataReady) {
    //if (millis() > t + serialPrintInterval) {
      //float i = LoadCell.getData();
      //lcd.setCursor(0, 0);
      //lcd.print("Berat: ");
      //lcd.print(i);
      //newDataReady = 0;
      //t = millis();
//    }
//  } 
  
 /* digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  lcd.setCursor(0, 0);
  lcd.print("R= ");
  GetData(); */

  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("Green= ");
  GetData();

  delay(1000);
  lcd.clear();
}

void GetData() {
  data = pulseIn(out, LOW);    //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again
  lcd.print(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
  lcd.print("\t");          //The higher the frequency the lower the duration
  delay(20);
}
