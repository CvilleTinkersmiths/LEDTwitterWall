#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information   

// We limit this so you won't use all of your Temboo calls while testing
int maxCalls = 3;

// The number of times this Choreo has been run so far in this sketch
int calls = 0;

int outputPin = 13;


void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected(USB ONLY!)
  delay(4000);
  while(!Serial);
  Bridge.begin();
  
  // Initialize pins
  pinMode(outputPin, OUTPUT);

  Serial.println("Setup complete.\n");
}

void loop() {
  if (calls < maxCalls) {
    Serial.println("Calling LatestMention Choreo...");
    runLatestMention();
    calls++;
  } else {
    Serial.println("Skipping to save Temboo calls. Adjust maxCalls as required.");
  }
  delay(30000);
}

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
          
      Serial.println("field: "+ name);

      if (name == "Text") {
        
           ///////// Testing array copying stuff ///////////////
          char msg[21]          = "initializing...";
          data.toCharArray(msg, sizeof(data)*8);
          msg[sizeof(data)*8] = 0;
          Serial.println("msg:");
          Serial.println(msg);
          ///////////////
             
          digitalWrite(outputPin, HIGH);
          Serial.println("data: " + data);
        
      }
    }
  }

  LatestMentionChoreo.close();
}
