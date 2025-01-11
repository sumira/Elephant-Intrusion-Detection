#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 12

RF24 radio(9, 10);  // CE, CSN
const byte address[6] = "00001";

int receivedNumber = 0;  

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nReceiver Circuit");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  
}

void loop() {
  
  if (radio.available()) {
    
    radio.read(&receivedNumber, sizeof(receivedNumber));

    Serial.print("Number received: ");
    Serial.println(receivedNumber);

    
    if (receivedNumber % 2 == 0) {
      digitalWrite(led, HIGH);  
    } else {
      digitalWrite(led, LOW);
    }
  }
}
