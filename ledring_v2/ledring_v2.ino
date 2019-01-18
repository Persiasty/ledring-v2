#include <Adafruit_NeoPixel.h>

#define N_PIXELS 24
#define LED_PIN 6

#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 3
#define PIN_ENCODER_SWITCH 4
 
unsigned long time = 0; 
volatile long count = 0;
volatile bool wasPressed = false;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

byte state = 0;
byte colorIndex = 0;
byte brightness = 10;
byte colors[][3] = {
    {255, 255, 255},
    {255, 0, 0},
    {255, 63, 0},
    {255, 127, 0},
    {255, 191, 0},
    {255, 255, 0},
    {191, 255, 0},
    {127, 255, 0},
    {63, 255, 0},
    {0, 255, 0},
    {0, 255, 63},
    {0, 255, 127},
    {0, 255, 191},
    {0, 255, 255},
    {0, 191, 255},
    {0, 127, 255},
    {0, 63, 255},
    {0, 0, 255},
    {63, 0, 255},
    {127, 0, 255},
    {191, 0, 255},
    {255, 0, 255},
    {255, 0, 191},
    {255, 0, 127},
    {255, 0, 63}
};

void setValue(bool);
void setColor();
void click();
void setup()
{
    ring.begin();
    setColor();

    Serial.begin(9600);

    pinMode(PIN_ENCODER_A, INPUT_PULLUP);
    pinMode(PIN_ENCODER_B, INPUT_PULLUP);
    pinMode(PIN_ENCODER_SWITCH, INPUT_PULLUP);

    attachInterrupt(0, encA, LOW);
    attachInterrupt(1, encB, LOW);

    time = millis();
}

void encA()
{
  if ((millis() - time) > 3)
        count++; 
  time = millis();
}

void encB()
{
  if ((millis() - time) > 3)  
        count--;
  time = millis();
}
 
void loop()
{
  if(count != 0)
    Serial.println(count);
  if (count > 0) {
    setValue(true);
    count--;
  }
  else if (count < 0) {
    setValue(false);
    count++;
  }
 
  if (digitalRead(PIN_ENCODER_SWITCH) == LOW)
  {
    if (!wasPressed) {
      click();
      delay(5);
    }
    wasPressed = true;
  }
  else
  {
    if (wasPressed) {
      delay(5);
    }
    wasPressed = false;
  }
}

void setColor() {
    for(uint16_t i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]));
    }
    ring.setBrightness(brightness);
    ring.show();
}

void setValue(bool inc) {
    if (state == 0) {
        Serial.print("color");
        colorIndex = inc ? min(24, colorIndex + 1) : max(0, colorIndex - 1);
        Serial.println(colorIndex);
        setColor();
    } else if (state == 1) {
        Serial.print("bright");
        brightness = inc ? min(200, brightness + 10) : max(0, brightness - 10);
        Serial.println(brightness);
        setColor();
    }
}

void click() {
    Serial.print("click");
    state = (state + 1) % 3;
    Serial.println(state);
}
