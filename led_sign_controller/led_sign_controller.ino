#include <FastLED.h>

#define NUM_LEDS    10
#define LED_PIN     6
#define BUTTON_PIN  8
#define POTENTIOMETER_PIN A3

#define BRIGHTNESS  255  // Max brightness
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define DEBOUNCE_TIME 150 // milliseconds for button debounce

// Define an array of color palettes
CRGBPalette16 colorPalettes[] = {RainbowStripeColors_p, CloudColors_p, PartyColors_p, ForestColors_p};
int currentPaletteIndex = 0; // Index to track the current color palette

unsigned long lastButtonPress = 0; // To track the time since the last button press for debouncing


void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  FastLED.setBrightness(BRIGHTNESS);
  //Serial.begin(9600);
}

void loop() {
  // Read the potentiometer, scale the value to use it for brightness (0 - 255)
  int potValue = analogRead(POTENTIOMETER_PIN);
  int brightness = map(potValue, 0, 710, 0, 255);
  if (potValue <= 100)
    brightness = 0;
  if (potValue >=700)
    brightness = 255;
  FastLED.setBrightness(brightness);



  // Check for button press and ensure debounce
  if (digitalRead(BUTTON_PIN) == LOW && millis() - lastButtonPress > DEBOUNCE_TIME) {
    // Update the last button press time
    lastButtonPress = millis();
    
    // Move to the next palette
    currentPaletteIndex = (currentPaletteIndex + 1) % (sizeof(colorPalettes) / sizeof(colorPalettes[0]));
  }

  // Fill LEDs with the current color palette
  fill_palette(leds, NUM_LEDS, 0, 255 / NUM_LEDS, colorPalettes[currentPaletteIndex], 255, LINEARBLEND);
  FastLED.show();


  // Small delay to limit the loop speed and reduce flicker
  delay(10);



  //Serial.println(potValue) ;
  //Serial.println(digitalRead(BUTTON_PIN));
   //delay(50);
}



