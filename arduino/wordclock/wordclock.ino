#include <Wire.h>
#include "RTClib.h"
// #include <IRremote.h>
// Update time with command : date +%Y%m%d%H%M%S > /dev/ttyACM3

RTC_DS3231 rtc;
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int incomingByte = 0;
//char readString[15];
String readString;

//int sensorPin = A0; // select the input pin for ldr
//int sensorValue = 0; // variable to store the value coming from the sensor
#define RECV_PIN  11
#define DISPLAY_PIN 6
#define NUM_LEDS 113
#define BRIGHTNESS 90
#define HOLD_CODE  0xFFFFFFFF
#define POWER_BUTTON 0xFFA25D
#define MINUS_BUTTON 0xFFA857
#define PLUS_BUTTON 0xFF906F
#define MODE_BUTTON 0xFF629D
#define ONE_BUTTON 0xFF30CF
#define TWO_BUTTON 0xFF18E7
#define THREE_BUTTON 0xFF7A85
#define ZERO_BUTTON 0xFF6897
#define CRAZY_ARROWS 0xFF9867


int last_button_pressed = 0;

//int ledIL[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,8,7,88,89,90,91,92,93,94,95,96,97,98,99,-1};

int ledIL[] = {98,99,-1};
int ledEST[] = {101,102,103,-1};
int ledUN[] = {-1};
int ledUNE[] = {80,81,82,-1};
int ledDEUX[] = {105,106,107,108,-1};
int ledTROIS[] = {91,90,89,88,87,-1};
int ledQUATRE[] = {97,96,95,94,93,92,-1};
int ledCINQ[] = {68,67,66,65,-1};
int ledSIX[] = {71,70,69,-1};
int ledSEPT[] = {83,84,85,86,-1};
int ledHUIT[] = {75,74,73,72,-1};
int ledNEUF[] = {76,77,78,79,-1};
int ledDIX[] = {56,57,58,-1};
int ledONZE[] = {53,52,51,50,-1};
int ledMINUIT[] = {59,60,61,62,63,64,-1};
int ledMIDI[] = {54,55,56,57,-1};
int led5CINQ[] = {16,17,18,19,-1};
int led5DIX[] = {40,41,42,-1};
int ledVINGT[] = {10,11,12,13,14,-1};
int ledHEURE[] = {48,47,46,45,44,-1};
int ledHEURES[] = {48,47,46,45,44,43,-1};

int ledNone[] = {-1};
int ledM1[] = {109,-1};
int ledM2[] = {112,-1};
int ledM3[] = {111,-1};
int ledM4[] = {110,-1};

int ledETQUART[] = {31,30,28,27,26,25,24,-1};
int ledVINGTCINQ[] = {10,11,12,13,14,15,16,17,18,19,-1};
int ledETDEMI[] = {9,8,6,5,4,3,2,-1};
int ledMOINSVINGTCINQ[] = {32,33,34,35,36,10,11,12,13,14,15,16,17,18,19,-1};
int ledMOINSVINGT[] = {32,33,34,35,36,10,11,12,13,14,-1};
int ledMOINSLEQUART[] = {32,33,34,35,36,38,39,28,27,26,25,24,-1};
int ledMOINSDIX[] = {32,33,34,35,36,40,41,42,-1};
int ledMOINSCINQ[] = {32,33,34,35,36,16,17,18,19,-1};

int ledBONNEANNEE[] = {-1};

int lastHour=-1;
int last5Minutes=-1;
int lastMinuteRest=-1;
int settedBrightness = BRIGHTNESS;
int lastBrightness = BRIGHTNESS;

int forceDisplay=0;

void* hoursLED[]={&ledMINUIT, &ledUNE, &ledDEUX, &ledTROIS, &ledQUATRE, &ledCINQ, &ledSIX, &ledSEPT, &ledHUIT, &ledNEUF, &ledDIX, &ledONZE, &ledMIDI, NULL};
void* hourSuffixLED[]={&ledNone, &ledHEURE, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES, &ledHEURES,   &ledNone, NULL};
void* step5minutesLED[]={&ledNone, &led5CINQ, &led5DIX, &ledETQUART, &ledVINGT, &ledVINGTCINQ, &ledETDEMI, &ledMOINSVINGTCINQ, &ledMOINSVINGT, &ledMOINSLEQUART, &ledMOINSDIX, &ledMOINSCINQ, NULL};
void* minuteRestLED[]={&ledNone, &ledM1, &ledM2, &ledM3, &ledM4, NULL};

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

int on=true;

void setup() {
  readString[0]='\0';
  Serial.begin(115200);
  randomSeed(analogRead(0));
    // End of trinket special code
  grid.setBrightness(BRIGHTNESS);
  //irrecv.enableIRIn(); // Start the receiver
  grid.begin();
  rtc.begin();
  grid.show(); // Initialize all pixels to 'off'
  grid.clear();
  forceDisplay=1;

//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 
}


void loop() {

  
  while (Serial.available()) {
    //delay(3);  //delay to allow buffer to fill
    if (Serial.available()>0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }
  //Serial.print(F("read from serial port "));
  //Serial.println(readString.length());
  if (readString.length()>=14 && readString.toInt()!=0) {
    Serial.print(F("Update date : "));
    Serial.println(readString);
    int year=readString.substring(0,4).toInt();
    int month=readString.substring(4,6).toInt();
    int day=readString.substring(6,8).toInt();
    int hour=readString.substring(8,10).toInt();
    int minute=readString.substring(10,12).toInt();
    int second=readString.substring(12,14).toInt();
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    forceDisplay=1;
  }
  readString="";
  DateTime currentTime=rtc.now();

  int currentHour=currentTime.hour();
  int current5Minutes=currentTime.minute() / 5;
  int currentMinuteRest=currentTime.minute() % 5;
  if (last5Minutes!=current5Minutes) {
     Serial.println(currentTime.minute(),DEC);
     change5Minutes(current5Minutes);
     last5Minutes=current5Minutes;
  }
  if (last5Minutes>6) {
    Serial.println("dans les 5 dernieres minutes");
    Serial.println(currentTime.hour(),DEC);
    currentHour++;
  }
  if (currentHour>12) {
    Serial.println(">12");
    Serial.println(currentTime.hour(),DEC);
    currentHour=currentHour % 12;
  }
  if (lastHour!=currentHour) {
    Serial.println("change d'heure");
    Serial.println(currentTime.hour(),DEC);
    changeHour(currentHour);
    lastHour=currentHour;
  }

  if (lastMinuteRest!=currentMinuteRest) {
     displayTime(currentHour, current5Minutes, currentMinuteRest);
     //paintWord(ledM4, colorBlack);
     //paintWord((int *)minuteRestLED[currentMinuteRest], currentColor);
     lastMinuteRest=currentMinuteRest;
  }


  if (lastBrightness != settedBrightness) {
    lastBrightness=settedBrightness;
    Serial.print(F("change brightness : "));
    Serial.println(lastBrightness);
    grid.setBrightness(lastBrightness);
    forceDisplay=1;
  }

  if (forceDisplay) {
    
    displayTime(currentHour, current5Minutes, currentMinuteRest);
    forceDisplay=0;
  }

  if (currentTime.day()==31 && currentTime.month()==12 && currentTime.hour()==23 && currentTime.minute()==59 && currentTime.second()==50) {
    displayNewYearDay();
    forceDisplay=1;
  }
  delay(100);

}

void displayNewYearDay() {
  Serial.print(F("10s avant la nouvelle annee"));
  grid.clear();
  paintWord(ledDIX ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledNEUF ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledHUIT ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledSEPT ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledSIX ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledCINQ ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledQUATRE ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledTROIS ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledDEUX ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledUN ,colorRed);
  delay(1000);
  grid.clear();
  paintWord(ledBONNEANNEE ,colorWhite);
  delay(30000);
  grid.clear();

}

void colorWipe(uint32_t c, uint8_t wait) {
  Serial.println("colorWipe");
  for(uint16_t i=0; i<grid.numPixels(); i++) {
    grid.setPixelColor(i, c);
    grid.show();
    delay(wait);
  }
}

void displayTime(int currentHour, int current5Minutes, int currentMinuteRest) {
  Serial.println("displayTime");
  grid.setBrightness(lastBrightness);
  grid.clear();
  paintWord(ledIL ,currentColor);
  paintWord(ledEST ,currentColor);
  changeHour(currentHour);
  change5Minutes(current5Minutes);
  paintWord(ledM4, colorBlack);
  if (on) {
     for(uint16_t i=0; i<=currentMinuteRest; i++) { 
       paintWord((int *)minuteRestLED[i], currentColor);
     }
  }
  Serial.println("displayTime end");
}


void change5Minutes(int minutesIndex) {
 for (int i = 0; step5minutesLED[i]!=NULL; ++i) {
   int* tmp = ((int *)(step5minutesLED[i]));
   for (int j=0; tmp[j] != -1; ++j) {
       grid.setPixelColor(tmp[j],colorBlack);
   }
 }
 int* tmp = ((int *)(step5minutesLED[minutesIndex]));
 for (int j=0; tmp[j] != -1; ++j) {
   if (on) {
     grid.setPixelColor(tmp[j],currentColor);
   }
 }
}

void changeHour(int hourIndex) {
 for (int i = 0; hoursLED[i]!=NULL; ++i) {
   int* tmp = ((int *)(hoursLED[i]));
   for (int j=0; tmp[j] != -1; ++j) {
     grid.setPixelColor(tmp[j],colorBlack);
   }
 }
 int* tmp = ((int *)(hoursLED[hourIndex]));
 for (int j=0; tmp[j] != -1; ++j) {
   if (on) {
     grid.setPixelColor(tmp[j],currentColor);
   }
 }
 for (int i = 0; hourSuffixLED[i]!=NULL; ++i) {
   int* tmp = ((int *)(hourSuffixLED[i]));
   for (int j=0; tmp[j] != -1; ++j) {
       grid.setPixelColor(tmp[j],colorBlack);
   }
 }
 int* hourSuffix = ((int *)(hourSuffixLED[hourIndex]));
 for (int j=0; hourSuffix[j] != -1; ++j) {
   if (on) {
     grid.setPixelColor(hourSuffix[j],currentColor);
   }
 }
}

void paintWord(int ledWord[], uint32_t intColor){
  Serial.println("paintWord begin");
  for(int i = 0; ledWord[i] != -1; ++i){
      if (on) {
        grid.setPixelColor(ledWord[i],intColor);
      } else {
        grid.setPixelColor(ledWord[i],colorBlack);
      }
  }
  grid.show();
    Serial.println("paintWord end");
}


