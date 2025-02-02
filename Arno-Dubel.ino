// Board Arduino Pro Micro
// 16.000 MHz
// Board: Arduino Micro, hatte auch mit Arduino Leonardo funktioniert.
// Das Programm bewegt die Maus von Pixel 10 bis Pixel 900 mit einer Random-Zahl.
// Beim Programmieren verändert sich regelmäßig der Com-Port.
// //
// Auf Pin 2 (D2) ist ein Eingang mit eingeschaltetem internem Pull-UP Widerstand.
// Wird Pin 2 auf GND gelegt wechselt der Zustand AnAus und die Maus wird bewegt oder nicht bewegt.
// //
// Einzustellen ist als Board
// Board: Arduino Micro
// Port: Com xx (Arduino Micro)
// Programmer: AVR ISP 



#include <Mouse.h>
#define BUTTON_PIN 2

const int xAxis = A1;         //analog sensor for X axis
const int yAxis = A2;         // analog sensor for Y axis

int range = 12;               // output range of X or Y movement
int responseDelay = 2;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value
int minima[] = {1023, 1023};  // actual analogRead minima for {x, y}
int maxima[] = {0, 0};        // actual analogRead maxima for {x, y}
int axis[] = {xAxis, yAxis};  // pin numbers for {x, y}
int mouseReading[2];          // final mouse readings for {x, y}
bool AnAus = false;
bool LetzterStatusTaster = false;


long randNumberx;
long randNumbery;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Mouse.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {

bool  StatusTaster = digitalRead(BUTTON_PIN);


    // Prüfen, ob eine fallende Flanke erkannt wird
    if (LetzterStatusTaster == HIGH && StatusTaster == LOW) {
        delay(300);  // Entprellzeit
        //StatusTaster = digitalRead(buttonPin); // Erneutes Lesen zum Absichern
            AnAus = !AnAus;  // LED-Zustand umschalten
        //    digitalWrite(ledPin, ledState);
        Serial.print("Status ");
        Serial.println(AnAus);
        
    }
    
    LetzterStatusTaster = StatusTaster;  // Speichern des aktuellen Zustands

  if(AnAus)                       {   
  // make random numbers
  randNumberx = random(10, 900);
  randNumbery = random(10, 900);
  // read and scale the two axes:
  int xReading = readAxis(0);
  int yReading = readAxis(1);

  // move the mouse:
  Mouse.move(randNumberx, randNumbery, 0);
  delay(responseDelay);
  Serial.println(randNumberx);
  }
}

/*
  reads an axis (0 or 1 for x or y) and scales the
  analog input range to a range from 0 to <range>
*/

int readAxis(int axisNumber) {
  int distance = 0; // distance from center of the output range

  // read the analog input:
  int reading = analogRead(axis[axisNumber]);

  // of the current reading exceeds the max or min for this axis,
  // reset the max or min:
  if (reading < minima[axisNumber]) {
    minima[axisNumber] = reading;
  }
  if (reading > maxima[axisNumber]) {
    maxima[axisNumber] = reading;
  }

  // map the reading from the analog input range to the output range:
  reading = map(reading, minima[axisNumber], maxima[axisNumber], 0, range);

  // if the output reading is outside from the
  // rest position threshold,  use it:
  if (abs(reading - center) > threshold) {
    distance = (reading - center);
  }

  // the Y axis needs to be inverted in order to
  // map the movemment correctly:
  if (axisNumber == 1) {
    distance = -distance;
  }

  // return the distance for this axis:
  return distance;
}
