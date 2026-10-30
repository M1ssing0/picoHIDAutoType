#include "LittleFS.h"
#include "Keyboard.h"

// mutable file path
const char* filepath = "/test.txt";

void setup() {
  // define essentials
  pinMode(LED_BUILTIN, OUTPUT);

  //clock essentials
  unsigned long lastBlink = 0;
  unsigned long lastCharTime = 0;
  bool ledState = false;
  
  // initialize fs
  if (!LittleFS.begin()) {

    //if fs mount fails, lock up
    while (true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    return;
  }

  Keyboard.begin(); //initialize keyboard BUT do not start typing yet

  // indicate safety buffer
  //flash every second for 10 seconds
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10);
    digitalWrite(LED_BUILTIN, LOW);
    delay(30);
    delay(970);
  }

// after safety buffer, begin printing with a delay to prevent dropped characters

  File readFile = LittleFS.open(filepath, "r");
    if (readFile) {
    
      //initalize char timer
      lastCharTime = millis();

      while (readFile.available()) {

       //every 15ms, type character
        while (millis() - lastCharTime < 15) {
          if (millis() - lastBlink >= 500) { 
            lastBlink = millis();
            ledState = !ledState;
            digitalWrite(LED_BUILTIN, ledState);
          }
        }
      
        // update clock and print
       lastCharTime = millis();
       char c = readFile.read();
       Keyboard.print(c);

        // led updates
       if (millis() - lastBlink >= 500) { 
          lastBlink = millis();
          ledState = !ledState;
         digitalWrite(LED_BUILTIN, ledState);
       }
     }
     readFile.close();

    Keyboard.write(KEY_RETURN);
    Keyboard.print("D O N E");
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {

}
