
#include <Console.h>
#include <Mailbox.h>

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  Mailbox.begin();
  digitalWrite(13, HIGH);
  
  Console.begin();
  
  // Wait for Console port to connect
  while (!Console);
  Console.println("Setup complete.\n");
  
  Console.println("Welcome to Mailbox Reader\n");
  Console.println("The Mailbox is checked every 10 seconds. The incoming messages will be shown below.\n");
}

void loop() {
  String message;
  while (Mailbox.messageAvailable()){
    Mailbox.readMessage(message);
    Console.println("New mail! -- "+message);
  }
  Console.println("delaying 10 sec ...");
  delay(10000);
}
