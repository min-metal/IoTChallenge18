#include "State.h"
#include "Message.h"


/***************************************************************
 * Normal Arduino stuff starts here.
 */

LoRaModem modem;

unsigned long timeStart;

void setup()
{
  #ifdef DEBUG
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    // wait a moment for the serial port to finish starting up
    delay(1000);
  #endif

  timeStart = millis();
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

//    modem.Msg("IN:12,OUT:15,AREA:1");
    
//    delay(1000);
//
//  modem.cMsg("111,12,15,1");
//  modem.cMsg("100,-123.45");
//  delay(1000);
//  modem.checkAT();
}

/* Loop ****************************/

double thresholdPoints[6] = {-0.5, -0.25, -0.1, 0.1, 0.25, 0.5};
unsigned short msgFreq[5] = {1, 1, 1, 1, 1}; /* {1, 30, 60, 30, 1} */
State state(thresholdPoints, msgFreq, 1, &modem);

void loop() // run over and over
{  
  char prompt[20];
//  unsigned long upTime = millis() - timeStart;
//  sprintf(prompt, "Uptime: %dh:%dm:%ds", 
//    upTime/3600000, upTime/60000, upTime/1000);
//  sprintf(prompt, "Uptime: %dms", upTime);
//  delay(1000);
//  modem.cMsg(prompt);
//  delay(1000);
//  
//  modem.cMsg("100,123.45");
//  delay(2000);
//
//  String test = modem.getAscii();
//  //char * test = modem.getAsciiAsP();
//  delay(1000);
  
//  DEBUG_PRINT(test);

//  Message message;
//  boolean operationResult = getMessage(test, &message);
//
//  if(operationResult)
//  {
//    Serial.print("ID: ");
//    Serial.println(message.message_id);
//    Serial.print("Port: ");
//    Serial.println(message.port);
//    Serial.print("Message: ");
//    Serial.println(message.message);
//    
//    String returnMessage = "300," + String(message.port);
//    modem.cMsg(returnMessage);
//    delay(1000);
//  }

  Message message;
  boolean mResult;
  byte count = 0, MAX_TRIES = 5;
  do {
    modem.Msg("AT");
    delay(1000);
    String downLink = modem.getAscii();
    if(mResult = getMessage(downLink, &message))
    {
      execMessage(&message, &state);
    }
    ++count;

    if(count == MAX_TRIES)
      break;
  }
  while(mResult); //If a message is recieved, keep reading for more messages

  
  //State_Result result = state.run();

}

