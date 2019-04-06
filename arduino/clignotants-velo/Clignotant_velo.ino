// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include <Wire.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DISPLAY_PIN     9
#define BRIGHTNESS      90
#define NUM_LEDS        35

Adafruit_NeoPixel grid = Adafruit_NeoPixel(NUM_LEDS, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);

// a few colors
uint32_t colorWhite = grid.Color(255, 255, 255);
uint32_t colorBlack = grid.Color(0, 0, 0);
uint32_t colorRed = grid.Color(240, 30, 30);
uint32_t colorGreen = grid.Color(60, 255, 0);
uint32_t colorBlue = grid.Color(90, 130, 255);
uint32_t currentColor = colorWhite;
int currentRedColor=255;
int currentGreenColor=255;
int currentBlueColor=255;
int delayval = 25; // delay for half a second
int on=true;


// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value
int lastSwitchState = LOW;        // previous switch state
const int xAxis = 3;         // joystick X axis
const int yAxis = 4;         // joystick Y axis
const int switchPin = 5;
int state = 0;

int led[] = {1,-1};

uint32_t delayMS;


void setup() {
  Serial.begin(9600); 

  pinMode(switchPin, INPUT);       // the switch pin
  digitalWrite(switchPin, HIGH); // turns on pull-up resistors

  grid.setBrightness(BRIGHTNESS);

  grid.begin();
  grid.show(); // Initialize all pixels to 'off'
  grid.clear();

}

void loop() {
  setState();
}

bool getState() {
  int xReading = readAxis(A3);
  int yReading = readAxis(A4);

  if (xReading != 0 and yReading != 0 ) 
    return true;
  else 
    return false;
}

void setState() {
  int xReading = readAxis(A3);
  int yReading = readAxis(A4);

  if (!digitalRead(switchPin))
  {
    Serial.println("  Button pressed !");
    state = 0;
  }

  if(yReading > 0 or state == 1 ) {
    blinkRight();
    state = 1;
  }
  else if (yReading < 0 or state == 2 ) {
    blinkLeft();
    state = 2;
  }
  if (xReading < 0 or state == 3 ) {
    blinkForward();
    state = 3;
  }
  else if (xReading > 0 or state == 4 ) {
    blinkStop();
    state = 4;
  }
}

void show() {

  for(int i=0;i<NUM_LEDS;i++){

    grid.setBrightness(valuePhr/10);
    grid.setPixelColor(i, grid.Color(150,150,150)); // Moderately bright green color.
  }

  grid.show(); // This sends the updated pixel color to the hardware.
}



void unshow() {

  for(int i=0;i<NUM_LEDS;i++){

    grid.setPixelColor(i, grid.Color(0,0,0)); //

  }
    grid.show(); // This sends the updated pixel color to the hardware.
}

void showSlow() {

  for(int i=0;i<NUM_LEDS;i++){

    grid.setPixelColor(i, grid.Color(150,150,150)); // Moderately bright green color.
    grid.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}



void blinkRight() {

  for(int i=0;i<NUM_LEDS;i++){

    if (!digitalRead(switchPin) or getState()) {
      unshow();
      return;
    }
    grid.setPixelColor(i, grid.Color(255, 178, 0)); // Moderately bright green color.
    grid.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }

  unshow();
}

void blinkLeft() {

  for(int i=NUM_LEDS;i>0;i--){
    if (!digitalRead(switchPin) or getState()) {
      unshow();
      return;
    }
    grid.setPixelColor(i, grid.Color(255, 178, 0)); // Moderately bright green color.
    grid.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  
  unshow();
}

void blinkForward() {

  for(int i=NUM_LEDS/2;i>0;i--){
    if (!digitalRead(switchPin) or getState()) {
      unshow();
      return;
    }
    grid.setPixelColor(i, grid.Color(60, 255, 0)); // Moderately bright green color.
    grid.setPixelColor(NUM_LEDS-i, grid.Color(60, 255, 0)); // Moderately bright green color.
    

    grid.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  
  unshow();
}

void blinkStop() {

  for(int i=0;i<NUM_LEDS/2;i++){
    if (!digitalRead(switchPin) or getState()) {
      unshow();
      return;
    }
    grid.setPixelColor(i, grid.Color(240, 30, 30)); // Moderately bright green color.
    grid.setPixelColor(NUM_LEDS-i, grid.Color(240, 30, 30)); // Moderately bright green color.
    

    grid.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  
  unshow();
}
void paint(int ledWord[], uint32_t intColor){
  Serial.println("paintWord begin");
  for(int i = 0; ledWord[i] != -1; ++i){
      if (on) {
        Serial.print(i);
        grid.setPixelColor(ledWord[i],intColor);
      } else {
        grid.setPixelColor(ledWord[i],colorBlack);
      }
  }
  grid.show();
    Serial.println("paintWord end");
}


void readJoystick() { // read the switch:
  int switchState = digitalRead(switchPin);
  // if it's changed and it's high, toggle the mouse state:
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      
      // turn on LED to indicate mouse state:
      Serial.print("------------------------------------ ");Serial.print(switchState);
    }
  }
  // save switch state for next comparison:
  lastSwitchState = switchState;

  // read and scale the two axes:
  int xReading = readAxis(A3);
  int yReading = readAxis(A4);

  Serial.print("------------------------------------ ");Serial.print(xReading);
  
  Serial.print("------------------------------------ ");Serial.print(yReading);
}

/*
  reads an axis (0 or 1 for x or y) and scales the analog input range to a range
  from 0 to <range>
*/

int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);
  Serial.println(reading);
  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}
