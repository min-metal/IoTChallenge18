#include "Modem.h"
LoRaModem::LoRaModem()
{
  _LoRaSerial.begin(9600);
  _LoRaSerial.setTimeout(100);
};

/******************
 * _sendSerial
 * Generic private method for sending serial data to the modem, used by everything. 
 */

void LoRaModem::_sendSerial(String message)
{
  DEBUG_PRINT("> " + message);
  _LoRaSerial.read();
  _LoRaSerial.println(message);
};

/******************
 * _checkresponse
 * Generic private method for reading back from the modem, and qualifying the response against a Regex 
 */

int LoRaModem::_checkresponse(char* checkVal, int call_timeout = rx_timeout)
{
  modemResp[0] = 0;
  
  unsigned long startTime = millis();

  int i = 0;
  
  while (i < rx_buffer-1)
  {
    int rxcount = _LoRaSerial.readBytes(&modemResp[i], rx_buffer-i);
    i += rxcount;
    modemResp[i] = 0;
    
    if (millis() - startTime > call_timeout)
    {
        _rspMs.Target(modemResp, i);
        char result = _rspMs.Match(checkVal, 0);
        if (result == REGEXP_MATCHED)
        {
          //Terminate the string
          DEBUG_PRINT(modemResp);
          return 0;
        }
        else
        {
          DEBUG_PRINT("Unexpected response / no response");
          DEBUG_PRINT(modemResp);
          return 1;
        }
    }
   
  }
  
};


// Check AT, just check if the modem is alive
int LoRaModem::checkAT(){
  _sendSerial("AT");
  return _checkresponse("%+AT: OK", rx_timeout_fast);  
};

// Find out current modem ID
int LoRaModem::checkID(){
  _sendSerial("AT+ID");
  if (_checkresponse("%+ID: DevAddr (.+)\r\n%+ID: DevEui (.+)\r\n%+ID: AppEui (.+)"))
  {
    return 1;
  }
  _rspMs.GetCapture (DevAddr, 0);
  _rspMs.GetCapture (DevEui, 1);
  _rspMs.GetCapture (AppEui, 2);

   String temp_s;
   temp_s = String(DevAddr);
   temp_s.replace(":", " ");
   temp_s.toCharArray(DevAddr, 12);

   temp_s = String(DevEui);
   temp_s.replace(":", " ");
   temp_s.toCharArray(DevEui, 24);

   temp_s = String(AppEui);
   temp_s.replace(":", " ");
   temp_s.toCharArray(AppEui, 24);

};

// Set the modem ID
int LoRaModem::setID(String addr, String dev){
  _sendSerial("AT+ID=DevAddr,\"" + addr + "\"");
  if (_checkresponse("%+ID: DEVADDR .+", rx_timeout_fast))
  {
    return 1;
  }
  
  _sendSerial("AT+ID=DevEui,\"" + dev + "\"");
  if (_checkresponse("%+ID: DEVEUI .+", rx_timeout_fast))
  {
    return 1;
  }

  return 0;
};

// Set the modem keys
int LoRaModem::setKeys(String NWSKey, String AppSKey){
  _sendSerial("AT+KEY=NWKSKEY,\"" + NWSKey + "\"");
  if (_checkresponse("%+KEY: NWKSKEY .+", rx_timeout_fast))
  {
    return 1;
  }
  
  _sendSerial("AT+KEY=APPSKEY,\"" + AppSKey + "\"");
  if (_checkresponse("%+KEY: APPSKEY .+", rx_timeout_fast))
  {
    return 1;
  }

  return 0;
  
};

// Set the payload port #
int LoRaModem::setPort(String portNum){
  _sendSerial("AT+PORT=" + portNum);
  if (_checkresponse("%+PORT: OK.+", rx_timeout_fast))
  {
    return 1;
  }

  return 0;
  
};

// Send a message, expect an ACK
int LoRaModem::cMsg(String message){
  _sendSerial("AT+CMSG=\"" + message + "\"");

  if (_checkresponse(".+ACK Received.+CMSG: Done"))
  {
    return 1;
  }

  return 0;
}

// Send a message, don't expect an ACK
int LoRaModem::Msg(String message){
  _sendSerial("AT+MSG=\"" + message + "\"");

  if (_checkresponse(".+MSG: Done"))
  {
    return 1;
  }

  return 0;
}

// Reset the modem
int LoRaModem::Reset(){
  _sendSerial("AT+RESET");

  if (_checkresponse(".+RESET: OK.+ 1.7.1"))
  {
    return 1;
  }
  return 0;
}

// Extract the downlink payload (if any) and convert to ASCII
String LoRaModem::getAscii(){

  char buf[100];
  
  char result = _rspMs.Match(".+RX: \"(.+)\".+", 0);
  if (result == REGEXP_MATCHED)
    {
      _rspMs.GetCapture (buf, 0);
    
      String ASCIIValue = "";
      
      for (int i = 0; i < strlen(buf)+1; i += 3) {
          char val = buf[i] > 0x39 ? (buf[i] - 55) * 16 : (buf[i] - '0') * 16;
          val += buf[i+1] > 0x39 ? (buf[i+1] - 55) : (buf[i+1] - '0');
          ASCIIValue += val;
        }
      return ASCIIValue;
    }
    else
    {
      DEBUG_PRINT("No ASCII payload...");
      return "";
    }
}

char * LoRaModem::getAsciiAsP(){

  char buf[100];
  char * bufAscii = (char *) malloc(100);
  
  char result = _rspMs.Match(".+RX: \"(.+)\".+", 0);
  if (result == REGEXP_MATCHED)
  {
    _rspMs.GetCapture (buf, 0);
  
    int j = 0;
    for (int i = 0; i < strlen(buf)+1; i += 3) 
    {
      char val = buf[i] > 0x39 ? (buf[i] - 55) * 16 : (buf[i] - '0') * 16;
      val += buf[i+1] > 0x39 ? (buf[i+1] - 55) : (buf[i+1] - '0');
      bufAscii[j++] = val;
    }
    return bufAscii;
  }
  else
  {
    DEBUG_PRINT("No ASCII payload...");
    return NULL;
  }

    
}


