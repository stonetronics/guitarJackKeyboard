/*
 * 
 *  ID Guitar plug keyboard with two plugs
 *  
 *  This is the arduino code for a Keyboard that has guitar jacks in them with keychains with guita plugs and resistive keychain identification
 *  
 *  Schematic:
 *  
 *  for each plug (Rx) a resistive voltage divider with 100k on the top resistance is used
 *  On the keychain guitar plug there is a resistance that when plugged in can be determined to identify the keychain.
 *  
 *  the voltage divider is built twice, one for each channel
 *      ______       ______
 * 5V--|_100k_|--o--|__Rx__|--GND
 *               |
 *               A0 (A1)
 * 
 *  Calculations:
 *  
 *  The identifing resistances will be 100k and 47k
 *  5V gives 1023 on ADC 
 *  5V * Rx/(100k + Rx) gives 1023 * Rx / (100k + Rx) on ADC
 *  
 *  so for Rx := 100k
 *  ADC = 1023 * 100k / (100k + 100k) = 511.5
 *  
 *  and Rx: 47k
 *  ADC = 1023 * 47k / (100k + 47k) = 327.081...
 *  
 *  So the borders for the identification will be set to (200, 400) for the first keychain and (401, 600) for the second keychain
 *  
 */

typedef enum {_NONE, _KEY1, _KEY2} _key;

#define KEY1_MIN  200
#define KEY1_MAX  400
#define KEY2_MIN  401
#define KEY2_MAX  600

#define TERMINAL1 A0   //analog input defines
#define TERMINAL2 A1

#define LED1_R  2   //led defines. the leds are in 0?-active konfiguration
#define LED1_G  3
#define LED1_B  4
#define LED2_R  5
#define LED2_G  6
#define LED2_B  7

#define RX 11   //pins for bitbang serial to the bluetooth module 
#define TX 12
#define LED 13  //onboard led
#include <SoftwareSerial.h>

SoftwareSerial HC05(TX, RX);


void setup() {

  pinMode(LED1_R, OUTPUT);
  pinMode(LED1_G, OUTPUT);
  pinMode(LED1_B, OUTPUT);
  pinMode(LED2_R, OUTPUT);
  pinMode(LED2_G, OUTPUT);
  pinMode(LED2_B, OUTPUT);

  Serial.begin(57600); //setup debug serial
  HC05.begin(38400); //setup bt
  while(HC05.available() > 0){  //flush buffer             
      HC05.read();
  }
}

String getString(_key key)
{
  switch (key) {
    case _NONE:
      return "none";
      break;

    case _KEY1:
      return "KEY1";
      break;
      
    case _KEY2:
      return "KEY2";
      break;
      
    default:
      break;
  }
  return "";
}

bool isInRange(int minimum, int maximum, int number)
{
  if ((number >= minimum) && (number <= maximum))
    return true;
  return false;
}

_key getKey(int channel)
{
  int adc = 0;
  adc = analogRead(channel);

  if (isInRange(KEY1_MIN, KEY1_MAX, adc))
    return _KEY1;
  else if (isInRange(KEY2_MIN, KEY2_MAX, adc))
    return _KEY2;
  else
    return _NONE;
  
}

void loop() {
  char inputByte = '0';
  _key terminal1 = _NONE;
  _key terminal1_old = _NONE;
  _key terminal2 = _NONE;
  _key terminal2_old = _NONE;

  //read the connected keys
  terminal1 = getKey(TERMINAL1);
  terminal2 = getKey(TERMINAL2);

  //check if a connection has changed
  if (terminal1 != terminal1_old)
  {
    //notify over bluetooth
    HC05.print("terminal1: ");
    HC05.print(getString(terminal1_old));
    HC05.print(" => ");
    HC05.print(getString(terminal1));

    //change led accordingly
    switch (terminal1) {
      case _KEY1:
          //green led
        break;
      case _KEY2:
          //blue led
        break;
      case _NONE:
      default:
         //red led
        break;

      //save value
      terminal1_old = terminal1;
    }
  }

  if (terminal2 != terminal2_old)
  {
    HC05.print("terminal2: ");
    HC05.print(getString(terminal2_old));
    HC05.print(" => ");
    HC05.print(getString(terminal2));
    terminal2_old = terminal2;
    switch (terminal2) {
      case _KEY1:
          //green led
        break;
      case _KEY2:
          //blue led
        break;
      case _NONE:
      default:
         //red led
        break;
      
    }
  }
  
  while(HC05.available() > 0) {
    inputByte = HC05.read();
    switch (inputByte) {
      case 's':
      case 'S': //status request, print connected keychains

        Serial.println("status request");

        HC05.println("Connected Keychains:");
        HC05.print("Terminal 1: ");
        HC05.println(getString(terminal1));
        HC05.println(analogRead(TERMINAL1));
        HC05.print("Terminal 2: ");
        HC05.println(getString(terminal2));
        HC05.println(analogRead(TERMINAL2));
        break;

      default:
        break;
    }
  }
}
