#define STRLEN 50

#include "Modem.h"
#include "Message.h"
#include "IR_Sensor.h"

/***************************************************************
 * Normal Arduino stuff starts here.
 */

LoRaModem modem;

void setup()
{
  #ifdef DEBUG
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    // wait a moment for the serial port to finish starting up
    delay(1000);
  #endif

  IR_setup();
  
  DEBUG_PRINT("Connected");

  modem.Reset();
//  delay(1000);
//  modem.setPort("1");
//  delay(1000);
//  
//  modem.setID(idDevAddr, idDevEui);
//  delay(1000);
//  modem.setKeys(idNwSKey, idAppSKey);
//
//  modem.Msg("9999");
  
}

/* Loop ****************************/


void loop() // run over and over
{
  String returnMessage;
//  
//  modem.cMsg("100,123.45");
//  delay(2000);

  String test = modem.getAscii();
  //char * test = modem.getAsciiAsP();
  delay(1000);
  
  DEBUG_PRINT(test);

  Message message;
  boolean operationResult = getMessage(test, &message);

  if(operationResult)
  {
    Serial.print("ID: ");
    Serial.println(message.message_id);
    Serial.print("Port: ");
    Serial.println(message.port);
    Serial.print("Message: ");
    Serial.println(message.message);
    
    returnMessage = "300," + String(message.port);
    modem.cMsg(returnMessage);
    delay(1000);
  }

  Serial.println(irRead(irSensorPin, irLedPin)); //display the results
  delay(10); //wait for the string to be sent
  

  //free(test);
}

