#include <Arduino.h>
#include <FastLED.h>
#include <Servo.h>

#define debug_print true  // Set to true or false to enable or disable debugging over serial

#if debug_print == true
   #define debug_begin(x)        Serial.begin(x)
   #define debug(x)              Serial.print(x)
   #define debugln(x)            Serial.println(x)
#else
   #define debug_begin(x)
   #define debug(x)
   #define debugln(x)
#endif

#define PIN_LED             D3
#define PIN_SERVO           D2
#define LED_COUNT           8
#define LED_TYPE            WS2811
#define COLOR_ORDER         GRB
#define INIT_BRIGHTNESS     127
#define BRIDGE_OPEN         1500
#define BRIDGE_CLOSED       2400

CRGB leds[LED_COUNT];
int lights[LED_COUNT];

Servo bridge;

// put function declarations here:
int myFunction(int, int);
void rainbow(int, int);
void bridge_tilt(int, int);

void setup() {
  // put your setup code here, to run once:
  debug_begin(115200);

  FastLED.addLeds<LED_TYPE, PIN_LED, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);

  bridge.attach(PIN_SERVO, 500, 2400);
  bridge.writeMicroseconds(BRIDGE_CLOSED);

  fill_rainbow( leds, LED_COUNT, 0 /*starting hue*/, 32);
  for(int i=0; i<=INIT_BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    delay(50);
    FastLED.show();
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  //leds[0] = CRGB::White; FastLED.show(); delay(1000);
	//leds[0] = CRGB::Black; FastLED.show(); delay(1000);
  //leds[0] = CRGB::Red; FastLED.show(); delay(1000);
	//leds[0] = CRGB::Green; FastLED.show(); delay(1000);
  //leds[0] = CRGB::Blue; FastLED.show(); delay(1000);
  
  bridge_tilt(20000,10000);

  for(int i=0; i<10; i++) {
    rainbow(8, 10000);
  }
  
}

// put function definitions here:
void setPositions(){
  for(int i=0; i<LED_COUNT; i++)
  {
    lights[i] = i;
  }
}

void rainbow(int width, int duration){
  unsigned long startTime;
  int hue;
  int delta;

  startTime = millis();
  while(millis() < startTime + duration) {
    hue = ((millis()-startTime)*255)/duration;
    //debugln(hue);
    delay(1);
    delta = (255 / width);
    fill_rainbow( leds, LED_COUNT, hue, delta);
    FastLED.show();
  }

}

void bridge_tilt(int move, int hold) {
  unsigned long startTime;
  int pos = BRIDGE_CLOSED;

  startTime = millis();
  while(millis() < startTime + move) {
    pos = BRIDGE_CLOSED - (((millis()-startTime)*abs(BRIDGE_OPEN-BRIDGE_CLOSED))/move);
    debug("Bridge: ");
    debugln(pos);
    bridge.writeMicroseconds(pos);
  }

  delay(hold);

  startTime = millis();
  while(millis() < startTime + move) {
    pos = BRIDGE_OPEN + (((millis()-startTime)*abs(BRIDGE_OPEN-BRIDGE_CLOSED))/move);
    debug("Bridge: ");
    debugln(pos);
    bridge.writeMicroseconds(pos);
  }

}
