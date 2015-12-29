#include <FastLED.h>
#define NUM_LEDS 75*7
#define DATA_PIN 6
CRGB leds[NUM_LEDS];

#ifndef DEBUG
#define DEBUG 1 // If DEBUG is 1, the program will wait in startup until a Console is connected
#endif

#ifndef DEBUG
  #include <Console.h>
#endif

// STATUS LED STUFF --------------------------------------------------------

// The Arduino's onboard LED indicates BTLE status.  Fast flash = waiting
// for connection, slow flash = connected, off = disconnected.
#define LED 13                   // Onboard LED (not NeoPixel) pin
int           LEDperiod   = 0;   // Time (milliseconds) between LED toggles
boolean       LEDstate    = LOW; // LED flashing state HIGH/LOW
unsigned long prevLEDtime = 0L;  // For LED timing


///////  create a file TembooAccount.h that contains:  //////////
//#define TEMBOO_ACCOUNT ""       // Your Temboo account name 
//#define TEMBOO_APP_KEY_NAME ""  // Your Temboo app key name
//#define TEMBOO_APP_KEY ""        // Your Temboo app key
/////////////////////////////////////////////////////////////////

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h"


// We limit this so you won't use all of your Temboo calls while testing
int maxCalls = 2;

// The number of times this Choreo has been run so far in this sketch
int calls = 0;

unsigned long prevCalltime = 0L;  // For Temboo Account Call timing
int           callPeriod   = 10*1000;   // Time (milliseconds) between calls to Temboo to check for Twitter mentions (zero disables)

///////// END Temboo stuff /////////////////





////////////////////////////////////////////////////////
////////////       SETUP      //////////////////////////
////////////////////////////////////////////////////////
void setup() {

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  pinMode(LED, OUTPUT);
  LEDstate    = HIGH;
  digitalWrite(LED, LEDstate);
  
  //set it flashing on 1sec interval
  LEDperiod = 1000;

  // Twitter support
  Bridge.begin();

  #if DEBUG
    Console.begin();
    // Wait for Console port to connect
    while (!Console);
      Console.println("Setup complete.\n");
  #endif
  
}

////////////////////////////////////////////////////////
////////////     MAIN LOOP    //////////////////////////
////////////////////////////////////////////////////////
void loop() {
  unsigned long t = millis(); // Current elapsed time, milliseconds.
  // millis() comparisons are used rather than delay() so that animation
  // speed is consistent regardless of message length & other factors.

  if(LEDperiod && ((t - prevLEDtime) >= LEDperiod)) { // Handle LED flash
    prevLEDtime = t;
    LEDstate    = !LEDstate;
    digitalWrite(LED, LEDstate);
   
  }

  if(callPeriod && ((t - prevCalltime) >= callPeriod) && (calls < maxCalls)) { // Check Twitter periodically until maxCalls is reached
    prevCalltime = t;

    
    #if DEBUG
    Console.println("Calling LatestMention Choreo...\n");
    #endif

    runLatestMention();
    
    calls++;
  }

        for(int dot = 0; dot < NUM_LEDS; dot++) { 
            leds[dot] = CRGB::Blue;
            FastLED.show();
            // clear this led for the next time around the loop
            leds[dot] = CRGB::Black;
            //delay(1);
        }

  
}


////////////////////////////////////////////////////////
////////////   Twitter Stuff  //////////////////////////
////////////////////////////////////////////////////////
void runLatestMention() {
    
  TembooChoreo LatestMentionChoreo;

  // Invoke the Temboo client
  LatestMentionChoreo.begin();

  // Set Temboo account credentials
  LatestMentionChoreo.setAccountName(TEMBOO_ACCOUNT);
  LatestMentionChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  LatestMentionChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set profile to use for execution
  LatestMentionChoreo.setProfile("wallsign");
  // Identify the Choreo to run
  LatestMentionChoreo.setChoreo("/Library/Twitter/Timelines/LatestMention");

  // Run the Choreo
  unsigned int returnCode = LatestMentionChoreo.run();
    
  // A return code of zero means everything worked
  if (returnCode == 0) {
      
    while (LatestMentionChoreo.available()) {
      String name = LatestMentionChoreo.readStringUntil('\x1F');
      name.trim();
      
      String data = LatestMentionChoreo.readStringUntil('\x1E');
      data.trim();

      if (name == "Text") {
        
          LEDperiod = 500; //Speed up flashing when the text is found
         
          #if DEBUG
            Console.println("Got it!");
          #endif
          
          //exit the read loop, we have everything we need
          LatestMentionChoreo.close();
      }
    }
    #if DEBUG
      Console.println("Done with the cherno read loop");
    #endif

  } else {
    #if DEBUG
      Console.println("Choreo run returned non-zero value:");
      Console.println(returnCode);
    #endif
  }

  LatestMentionChoreo.close();
}
