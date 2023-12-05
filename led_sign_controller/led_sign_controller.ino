
/*
  The circuit:
  - LED attached from pin 5 to ground through 220 ohm resistor
  - Toggle Button attached to 7
  - Potentiometer attached to pin A5

Relevent Examples:
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/


//WS2812 LED Setup
#include "FastLED.h"

#define NUM_LEDS 6
#define LED_PIN 2
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];



// PINOUT
const int power_button = 7;  // Power button PIN
const int mode_button =  12; // Mode Button PIN
const int mode_total = 6;
const int led_r = 3 ;
const int led_g = 5 ;
const int led_b = 6;
//const int ledPin = 5;    // the number of the LED pin
const int potPin = A5;
const float LED_MAX=255;
// const int roygbiv[7][3] = {{255,0,0},{255,127,0},{255,255,0},{0,255,0},{0,0,255},{75,0,130},{148,0,211}};


// CONSTANTS
const int debug=1; //debug toggle
const int debug_loop=0; //loop toggle

// INPUT VARIABLES 
bool buttonState=false;  // variable for reading the pushbutton status
int slider_input=0;
int potVal = 0;

// STATE VARIABLES
int led_pwr=0;
int outputValue=0;
int debug_time=0;
int red = 0;
int green = 0;
int blue = 0;
int r = LOW ;
int g = LOW ;
int b = LOW ;
int color = 1;
int power_state;
float slide_per;
int led_color;
float pwr_perc;
int mode_btn;
bool switch_modes=false;
bool mode_btn_press=false;
int current_mode;
int mode_num;
int mode;
int init_brightness;

//int color_code [1] = {LOW}
//int color_code [2] = {LOW}
//int color_code [3] = {LOW}

void setup() {
  // initialize the power_button as an input:
  pinMode(power_button, INPUT);
  pinMode(mode_button, INPUT);
  //LED array setup
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(init_brightness);

  // initialize the LED pin as an output:
  //pinMode(ledPin, OUTPUT);
  pinMode(led_r, red) ;
  pinMode(led_g, green) ;
  pinMode(led_b, blue) ;
  if (debug == 1 || debug_loop==1)
  {Serial.begin(9600);}


}


void loop() {
  //Read Inputs
  power_state = digitalRead(power_button);
  potVal = analogRead(potPin);
  mode_btn_press = digitalRead(mode_button);
  Serial.println(mode_btn_press);

  //Mode Button
  mode_num = mode_controller(mode_btn_press);

  //DEBUG
  if (debug == 1)
  { debug_output(debug);}

  
  // If on...
  if (power_state == 1)
  {
    //Read Slider Input
    slider_input = map(potVal, 0, 1023, 0, 255); 
    led_pwr = slider_input;
    //Serial.println(LED_MAX);
    pwr_perc = (led_pwr/LED_MAX);
    //Serial.println(pwr_perc);
    
    led_color = 2;
    led_code( led_color , led_pwr);
  }
  //If off
  if (power_state == 0)
  {led_code(0,0);}


}


//FUNCTIONS

int led_code (int color, float pwr)
{
  /*
  COLOR CODES
  0=OFF
  1=RED
  2=GREEN
  3=BLUE
  4=WHITE
  */

  // 1 - RED
  if (color == 1)
  { digitalWrite(led_r, LED_MAX*pwr);
    digitalWrite(led_g, 0);
    digitalWrite(led_b, 0);
  }
  // 2 - GREEN
    if (color == 2)
  { digitalWrite(led_r, 0);
    digitalWrite(led_g, LED_MAX*pwr);
    digitalWrite(led_b, 0);
  }
  // 3 -BLUE
  if (color == 3)
  { digitalWrite(led_r, 0);
    digitalWrite(led_g, 0);
    digitalWrite(led_b, LED_MAX*pwr);
  }

  // 4 - WHITE
    if (color == 4)
  { digitalWrite(led_r, HIGH);
    digitalWrite(led_g, HIGH);
    digitalWrite(led_b, HIGH);
  }

  // 0 - OFF
  if (color == 0)
  { digitalWrite(led_r, LOW);
    digitalWrite(led_g, LOW);
    digitalWrite(led_b, LOW);
  }

}


int debug_output(int debug_state)
  {  //DEBUG ONLY
      if (debug_state == 1)
      {
      if (debug_time==0)
      {Serial.print("\n \n \n --------DEBUG ENABLED--------- \n");}
      
      power_state= digitalRead(power_button);
      slide_per=analogRead(potPin);
      Serial.print("\n   Time = ");
      Serial.print(debug_time);
      Serial.print(" sec ");
      Serial.print("\n \t POWER STATE =");
      Serial.print(power_state);
      Serial.print("\n \t MODE BUTTON PRESS  -  ");
      Serial.print(mode_btn_press);   
      Serial.print("\n \t MODE =");
      Serial.print(mode_num);       
      Serial.print("\n \t LED Color Code =");
      Serial.print(led_color);      
      Serial.print("\n \t LED_pwr % =");
      Serial.print(led_pwr);
      Serial.print("\n");
      
      //Time keeper
      debug_time=debug_time+1;
    
      //Delay <X> micro seconds  
      delay(1000);
    //return;
  }
  }

int mode_controller( int button_press)
{
  Serial.println(button_press);
  if (button_press==1)
  {mode=mode++;}
  if (mode > mode_total)
  {return mode=0;}
  else
  return mode;
}


int LED_ONE_COLOR(int LED_r, int LED_g, int LED_b) 
{
  for (int pinNo = 0; pinNo <= NUM_LEDS-1; pinNo++) {
    leds[pinNo] = CRGB( 0, 255, 0);    // Set all LEDs to green color 
  }
  FastLED.show(); // Update the LEDs
}

void LED_RAINBOW_STATIC() {
// leds[LED#] = CRGB(roygbiv[ROW][0],roygbiv[ROW][1],roygbiv[ROW],2)
  for(int i =0;i<NUM_LEDS;i++)
  {
    leds[i]= ColorFromPalette(RainbowColors_p,3, 255, LINEARBLEND);
  }
  FastLED.show(); // Update the LEDs
}

void rainbow_wave(uint8_t thisSpeed, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
 
// uint8_t thisHue = beatsin8(thisSpeed,0,255);                // A simple rainbow wave.
 uint8_t thisHue = beat8(thisSpeed,255);                     // A simple rainbow march.
  
 fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);            // Use FastLED's fill_rainbow routine.
 
} // rainbow_wave()



