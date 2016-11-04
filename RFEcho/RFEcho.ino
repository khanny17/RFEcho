/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

#define TX_DATA_PIN  10
#define TX_POWER_PIN  9

#define BUFFER_SIZE 32

char inputBuffer[BUFFER_SIZE + 1]; //will hold a 32 bit binary string, plus a null terminator
int bufferIndex = 0; //
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  inputBuffer[BUFFER_SIZE] = 0; //Null terminator for the buffer
  
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(TX_DATA_PIN);  // Using Pin #10

  //Use the pin to supply power because i'm too damn lazy
  pinMode(TX_POWER_PIN, OUTPUT);
  digitalWrite(TX_POWER_PIN, HIGH);
}

void loop() {
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.println("Unknown encoding");
    } else {
      Serial.println( mySwitch.getReceivedValue() );
    }

    mySwitch.resetAvailable();
  }

  if(Serial.available()) {
    inputBuffer[bufferIndex] = Serial.read();
    ++bufferIndex; //We count down because we receive the bytes backwards
    //Check if buffer is full
    if(bufferIndex >= BUFFER_SIZE) {
      //buffer is full, we have a full message to relay
                         
      mySwitch.send(inputBuffer);
      bufferIndex = 0; //reset index to the end of the buffer
      Serial.print("Message Sent: ");
      Serial.println(inputBuffer);
    }
  }
}
