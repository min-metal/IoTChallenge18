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
  delay(1000);
  modem.setPort("1");
  delay(1000);
  
  modem.setID(idDevAddr, idDevEui);
  delay(1000);
  modem.setKeys(idNwSKey, idAppSKey);
  delay(1000);

//  modem.Msg("9999");

//    modem.Msg("IN:12,OUT:15,AREA:1");
    
//    delay(1000);
//
//  modem.cMsg("111,12,15,1");
  modem.cMsg("100,-123.45");
  delay(1000);
//  modem.checkAT();

//    modem.Msg("AT");
}

/* Loop ****************************/

double thresholdPoints[6] = {-0.5, -0.25, -0.1, 0.1, 0.25, 0.5};
unsigned short msgFreq[5] = {1, 1, 1, 1, 1}; /* {1, 30, 60, 30, 1} */
State state(thresholdPoints, msgFreq, 1, &modem);

void loop() // run over and over
{  
  char prompt[40];
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
//  boolean mResult;
//  byte count = 0, MAX_TRIES = 5;
//  do {
//    modem.Msg(" ");
//    delay(1000);
//    String downLink = modem.getAscii();
//    Serial.println(downLink);
//    if(getMessage(downLink, &message))
//    {
//      String returnMessage = "300," + String(message.port);
//      modem.Msg(returnMessage);
//      //execMessage(&message, &state);
//    }
//    ++count;
//
//    if(count == MAX_TRIES)
//      break;
//  }
//  while(mResult); //If a message is recieved, keep reading for more messages

//    Message message;
//    modem.Msg(" ");
//    delay(1000);
//    String downLink = modem.getAscii();
//    Serial.println(downLink);
//    getMessage(downLink, &message);


//  Serial.println("hello");
//  delay(1000);
    
//    if(getMessage(downLink, &message))
//    {
//      char returnMessage[40];
//      sprintf(returnMessage, "300, %d", message.port);
//      //String returnMessage = "300," + String(message.port);
//      modem.Msg(returnMessage);
//      //execMessage(&message, &state);
//    }

  
  
  State_Result result = state.run();


  Serial.print("allIn: ");
  Serial.println(result.in);
  Serial.print("allOut: ");
  Serial.println(result.out);
  Serial.print("timeForPeriod: ");
  Serial.println(result.Time);
  Serial.print("Area: ");
  Serial.println(result.area);

  sprintf(prompt, "111,%d,%d,%lu,%d", 
  result.in, result.out, result.Time / 1000, result.area);

  
  Serial.println(prompt);
  modem.Msg(prompt);
  
  delay(1000);


//    digitalWrite(enPin,HIGH);
////    digitalWrite(enPin2,HIGH);
//    Serial.println(irRead(irSensorPin, irLedPin));
////    Serial.println(irRead(irSensorPin2, irLedPin2)); 
//    digitalWrite(enPin,LOW);
//    delay(100);
}

