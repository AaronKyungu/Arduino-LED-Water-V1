#include "FastLED.h"
#include "IRremote.h"
#define NUM_LEDS 300
CRGB leds[NUM_LEDS];
#define LEDPIN1 13
#define LEDPIN2 10

#define POWER_KEY 0xFFA25D
//#define UP_KEY 0xFF906F
//#define DOWN_KEY 0xFFE01F //setbrightness -5

int receiver = 7; // Signal Pin of IR receiver to Arduino Digital Pin 7
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
bool ledState; //led state on/off

// ------ setup --------

void setup()
{
  Serial.begin(9600); 
  irrecv.enableIRIn(); // Start the receiver
  ledState = false; //starts off
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  
  FastLED.addLeds<WS2812, LEDPIN1, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, LEDPIN2, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}
// ------- water stream --------

void waterStream (byte red, byte green, byte blue, byte streamSize, byte streamTrailDecay, boolean streamRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = NUM_LEDS + streamSize; i > 0; i--) {
   
    // fade brightness all LEDs one step
    for(int j=NUM_LEDS; j > 0; j--) {
      if( (!streamRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, streamTrailDecay );        
      }
    }
   
    // draw water stream
       for(int j = streamSize; j > 0; j--) {
       if( ( i+j <NUM_LEDS) && (i+j>=0) ) {
        setPixel(i+j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

// -------- the functions for waterStream function ---------------

void fadeToBlack(int ledNo, byte fadeValue) {
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
}
void showStrip() {
   // FastLED
   FastLED.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   // FastLED
   leds[Pixel] = CRGB(red, green, blue);
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
// ------------------------------
void loop() {
  if (irrecv.decode(&results)) // have we received an IR signal?
  {

    //power
    if (POWER_KEY == results.value && ledState == false)
    {
      for(int i = 0; i < 4500; i++){
      int firstRand = random(30);
           int randNum = random(32, 90);
           waterStream(247,250,255,10, randNum, true, firstRand);
           ledState = true;
      }// for loop
    }
    else if(POWER_KEY == results.value && ledState == true)
    {
      FastLED.clear();
      setAll(0,0,0);
      ledState = false;
    }
    irrecv.resume(); // receive the next value  
  }  
} //loop
