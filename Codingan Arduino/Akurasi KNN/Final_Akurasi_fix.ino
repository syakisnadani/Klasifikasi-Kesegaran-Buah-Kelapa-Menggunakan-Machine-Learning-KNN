#include <LiquidCrystal.h>
#include <HX711_ADC.h>
#include <Arduino_KNN.h>

// Create a new KNNClassifier, input values are array of 2 (floats),
// change if you need a bigger input size
KNNClassifier myKNN(2);


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

int data = 0; 

void setup() 
{
  lcd.begin(16, 2);


  pinMode(s0, OUTPUT);   //pin modes
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  
  Serial.begin(57600);
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
  while (!Serial);
  Serial.println("Simple KNN");
  Serial.println();

  Serial.print("Adding examples to myKNN ... ");
  Serial.println();
  
  // add examples to KNN {Berat (gr),G}
float kelapa1[] = { 4.64, 23 };
  float kelapa2[] = { 4.55, 24 };
  float kelapa3[] = { 4.52, 29 };
  float kelapa4[] = { 4.56, 20 };
  float kelapa5[] = { 4.41, 24 };
  float kelapa6[] = { 4.59, 24 };
  float kelapa7[] = { 4.62, 20 };
  float kelapa8[] = { 4.78, 27 };
  float kelapa9[] = { 4.79, 20 };
  float kelapa10[] = { 4.60, 23 };
  float kelapa11[] = { 0.51, 45 };
  float kelapa12[] = { 0.77, 59 };
  float kelapa13[] = { 0.73, 84 };
  float kelapa14[] = { 0.95, 79 };
  float kelapa15[] = { 0.90, 50 };
  float kelapa16[] = { 0.82, 52 };
  float kelapa17[] = { 0.47, 53 };
  float kelapa18[] = { 0.75, 70 };
  float kelapa19[] = { 0.52, 61 };
  float kelapa20[] = { 0.82, 48 };
  /*float kelapa21[] = { 28.0, 20.0 };
  float kelapa22[] = { 32.0, 18.0 };
  float kelapa23[] = { 31.0, 21.0 };
  float kelapa24[] = { 27.0, 20.0 };
  float kelapa25[] = { 30.0, 19.0 };
  float kelapa26[] = { 30.0, 22.0 };
  float kelapa27[] = { 35.0, 20.0 };
  float kelapa28[] = { 30.0, 19.0 };
  float kelapa29[] = { 30.0, 20.0 };
  float kelapa30[] = { 28.0, 19.0 };
  /*float kelapa31[] = { 31.0, 18.0 };
  float kelapa32[] = { 31.0, 18.0 };
  float kelapa33[] = { 27.0, 18.0 };
  float kelapa34[] = { 30.0, 20.0 };
  float kelapa35[] = { 28.0, 21.0 };
  float kelapa36[] = { 38.0, 19.0 };
  float kelapa37[] = { 33.0, 19.0 };
  float kelapa38[] = { 36.0, 22.0 };
  float kelapa39[] = { 39.0, 20.0 };
  float kelapa40[] = { 32.0, 18.0 };*/
  /*float kelapa41[] = { 38.0, 15.0 };
  float kelapa42[] = { 40.0, 15.0 };
  float kelapa43[] = { 47.0, 13.0 };
  float kelapa44[] = { 33.0, 14.0 };
  float kelapa45[] = { 34.0, 12.0 };
  float kelapa46[] = { 29.0, 20.0 };
  float kelapa47[] = { 30.0, 22.0 };
  float kelapa48[] = { 28.0, 22.0 };
  float kelapa49[] = { 27.0, 19.0 };
  float kelapa50[] = { 24.0, 19.0 };*/

  //1 = kelapa muda, 2 = kelapa tua
  myKNN.addExample(kelapa1, 1); // add example for class 7
  myKNN.addExample(kelapa2, 1); // add example for class 5
  myKNN.addExample(kelapa3, 1); // add example for class 9
  myKNN.addExample(kelapa4, 1); // add example for class 5
  myKNN.addExample(kelapa5, 1); // add example for class 5
  myKNN.addExample(kelapa6, 1); // add example for class 7
  myKNN.addExample(kelapa7, 1); // add example for class 5
  myKNN.addExample(kelapa8, 1); // add example for class 9
  myKNN.addExample(kelapa9, 1); // add example for class 5
  myKNN.addExample(kelapa10, 1); // add example for class 5
  myKNN.addExample(kelapa11, 2); // add example for class 7
  myKNN.addExample(kelapa12, 2); // add example for class 5
  myKNN.addExample(kelapa13, 2); // add example for class 9
  myKNN.addExample(kelapa14, 2); // add example for class 5
  myKNN.addExample(kelapa15, 2); // add example for class 5
  myKNN.addExample(kelapa16, 2); // add example for class 7
  myKNN.addExample(kelapa17, 2); // add example for class 5
  myKNN.addExample(kelapa18, 2); // add example for class 9
  myKNN.addExample(kelapa19, 2); // add example for class 5
  myKNN.addExample(kelapa20, 2); // add example for class 5 
  /*myKNN.addExample(kelapa21, 1); // add example for class 5
  myKNN.addExample(kelapa22, 1); // add example for class 9
  myKNN.addExample(kelapa23, 1); // add example for class 5
  myKNN.addExample(kelapa24, 1); // add example for class 5
  myKNN.addExample(kelapa25, 1); // add example for class 7
  myKNN.addExample(kelapa26, 1); // add example for class 7
  myKNN.addExample(kelapa27, 1); // add example for class 5
  myKNN.addExample(kelapa28, 1); // add example for class 9
  myKNN.addExample(kelapa29, 1); // add example for class 5
  myKNN.addExample(kelapa30, 1); // add example for class 5
  /*myKNN.addExample(kelapa31, 1); // add example for class 7
  myKNN.addExample(kelapa32, 1); // add example for class 5
  myKNN.addExample(kelapa33, 1); // add example for class 9
  myKNN.addExample(kelapa34, 1); // add example for class 5
  myKNN.addExample(kelapa35, 1); // add example for class 5
  myKNN.addExample(kelapa36, 1); // add example for class 7
  myKNN.addExample(kelapa37, 1); // add example for class 5
  myKNN.addExample(kelapa38, 1); // add example for class 9
  myKNN.addExample(kelapa39, 1); // add example for class 5
  myKNN.addExample(kelapa40, 1); // add example for class 5 
  /*myKNN.addExample(kelapa41, 2); // add example for class 7
  myKNN.addExample(kelapa42, 2); // add example for class 5
  myKNN.addExample(kelapa43, 2); // add example for class 9
  myKNN.addExample(kelapa44, 2); // add example for class 5
  myKNN.addExample(kelapa45, 2); // add example for class 5
  myKNN.addExample(kelapa46, 1); // add example for class 7
  myKNN.addExample(kelapa47, 1); // add example for class 5
  myKNN.addExample(kelapa48, 1); // add example for class 9
  myKNN.addExample(kelapa49, 1); // add example for class 5
  myKNN.addExample(kelapa50, 1); // add example for class 5*/

  // get and print out the KNN count
  Serial.print("\tmyKNN.getCount() = ");
  Serial.println(myKNN.getCount());
  Serial.println();

  // you can also print the counts by class
  //  Serial.print("\tmyKNN.getCountByClass(5) = ");
  //  Serial.println(myKNN.getCountByClass(5)); // expect 2

  // classify the input
}

void loop()                  //Every 2s we select a photodiodes set and read its data
{
  float i;
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      i = LoadCell.getData();
      Serial.print("Berat: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  Serial.print("G= ");
  GetData();
  data = pulseIn(out, LOW);

  float input[] = { i, data}; //input sensor

  int classification = myKNN.classify(input, 5); // classify input with K=3
  float confidence = myKNN.confidence();

  // print the classification and confidence
  lcd.setCursor(0, 0);
  lcd.print("Kelas = ");
  lcd.print(classification);

  // since there are 2 examples close to the input and K = 3,
  // expect the confidence to be: 2/3 = ~0.67
  lcd.setCursor(0, 1);
  lcd.print("ACC= ");
  lcd.println(confidence);

  delay(500);
}

void GetData() {
  data = pulseIn(out, LOW);    //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again
  Serial.println(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
  Serial.print("\t");          //The higher the frequency the lower the duration
  delay(20);
}
