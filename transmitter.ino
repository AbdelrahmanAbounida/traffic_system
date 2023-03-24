#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); // CE, CSN

// used address , the address, through which the two modules will communicate.),
//  We can change the value of this address to any 5 letter string,and this enables to choose to which receiver we will talk
const byte address[6] = "00001"; 

void setup(){
  radio.begin();
  radio.openWritingPipe(address); //  initialize the radio objec, initialize the address of the reciever to which we will send data
  radio.setPALevel(RF24_PA_MIN); // set the Power Amplifier level, in our case I will set it to minimum as my modules are very close to each other
  radio.stopListening(); // sets module as transmitter
}

void loop() {
  const char text[] = "Hello NRF24L01";
  radio.write(&text, sizeof(text));
  delay(1000);

