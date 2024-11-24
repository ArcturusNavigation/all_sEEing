#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS 144

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte code;
byte en;

byte px;
byte r;
byte g;
byte b;

void setup() {
  Wire.begin(0x11);
  Wire.onReceive(receiveEvent);

  pinMode(3, OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);

  pixels.begin();
}

void loop() {
  //update readings here
}

void receiveEvent() {
	code = Wire.read();
  switch(code) {
    case 0x01:
      en = Wire.read();
      if(en != 0x00) {
        digitalWrite(3,1);
      }
      else {
        digitalWrite(3,0);
      }
      break;
    case 0x02:
      en = Wire.read();
      if(en != 0x00) {
        digitalWrite(4,1);
      }
      else {
        digitalWrite(4,0);
      }
      break;
    case 0x03:
      en = Wire.read();
      if(en != 0x00) {
        digitalWrite(5,1);
      }
      else {
        digitalWrite(5,0);
      }
      break;
    case 0x04:
      r = Wire.read();
      g = Wire.read();
      b = Wire.read();
      pixels.fill(pixels.Color(r,g,b),0,144);
      pixels.show();
      break;
    case 0x05:
      px = Wire.read();
      r = Wire.read();
      g = Wire.read();
      b = Wire.read();
      pixels.setPixelColor(px, pixels.Color(r,g,b));
      pixels.show();
      break;
    case 0x06:
      pixels.clear();
      pixels.show();
      break;
  }
}
