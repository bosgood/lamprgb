#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define LED_PIN  1
#define POT_PIN  2
#define POT_ANALOG_PIN  1
#define N_LEDS   24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

uint8_t brightness = 255;
// uint32_t naturalWhite = Adafruit_NeoPixel::Color(50, 50, 50, 255);

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  strip.begin();
  strip.setBrightness(brightness);
  strip.show();

  pinMode(POT_PIN, OUTPUT);
}

void loop() {
  int mode = analogRead(POT_ANALOG_PIN);

  if (mode >= 100) {
    uint32_t color = Adafruit_NeoPixel::Color(50, 50, 50, mode);
    oneColor(color);
  } else {
    rainbowCycle(25, mode);
  }
}

void oneColor(uint32_t color) {
  for (int i = 0; i < N_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheel(byte wheelPos, uint8_t white) {
  if (wheelPos < 85) {
   return Adafruit_NeoPixel::Color(wheelPos * 3, 255 - wheelPos * 3, 0, white);
  } else if (wheelPos < 170) {
   wheelPos -= 85;
   return Adafruit_NeoPixel::Color(255 - wheelPos * 3, 0, wheelPos * 3, white);
  } else {
   wheelPos -= 170;
   return Adafruit_NeoPixel::Color(0, wheelPos * 3, 255 - wheelPos * 3, white);
  }
}

void rainbowCycle(uint8_t wait, uint8_t w) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i< N_LEDS; i++) {
      strip.setPixelColor(i, wheel(((i * 256 / N_LEDS) + j) & 255, w));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  rainbowCycle(wait, 0);
}
