#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define LED_PIN  1
#define POT_PIN  2
#define POT_ANALOG_PIN  1
#define N_LEDS   24
#define BRIGHTNESS 255
#define RAINBOW_STOP 100
#define WAIT 25
#define READ_BUFFER_SIZE 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  pinMode(POT_PIN, OUTPUT);
}

void loop() {
  uint8_t mode = readMode();
  uint8_t currentMode;

  uint16_t i, j;

  if (mode >= RAINBOW_STOP) {
    uint32_t color = Adafruit_NeoPixel::Color(50, 50, 50, mode);
    oneColor(color);
  } else {
    for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
      for (i = 0; i< N_LEDS; i++) {
        strip.setPixelColor(i, wheel(((i * 256 / N_LEDS) + j) & 255, mode));
      }
      strip.show();
      delay(WAIT);

      // Early abort if color changed in the loop
      currentMode = readMode();
      if (abs(mode - currentMode) > 5) {
        break;
      }
    }
  }
}

uint8_t readMode() {
  // Take average of a few readings to account for a noisy potentiometer
  uint16_t sum = 0;
  for (int i = 0; i < READ_BUFFER_SIZE; i++) {
    sum += analogRead(POT_ANALOG_PIN);
  }
  // uint8_t raw = analogRead(POT_ANALOG_PIN);
  // return map(raw, 0, 200, 0, 255);
  return sum / READ_BUFFER_SIZE;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// white is added to the color
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

void oneColor(uint32_t color) {
  for (int i = 0; i < N_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
