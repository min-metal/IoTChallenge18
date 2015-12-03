boolean getMessage(String string, Message * message)
{
  String buf = "";
  buf.reserve(STRLEN);
  int messageID = -1, port = 0;
  int i = 0;

  if(string.length() == 0 || string.length() >= STRLEN)
  {
    return false;
  }

  // Message must be in right format
  int count = 0;
  for(int j = 0; j < string.length(); ++i)
  {
    if(string.charAt(i) == ',')
    {
      ++count;
    }
  }
  if(count != 2)
    return false;

  // first interation to find ','
  for(i = 0; i < string.length(); ++i)
  {
    if(string.charAt(i) == ',')
    {
      break;
    }
    buf += string.charAt(i);   
  }

  messageID = buf.toInt();
  buf = "";

  // second iteration
  for(i = i+1; i < string.length(); ++i)
  {
    if(string.charAt(i) == ',')
    {
      break;
    }
    buf += string.charAt(i);
  }

  port = buf.toInt();
  buf = "";

  for(i = i+1; i < string.length(); ++i)
  {
    if(string.charAt(i) == ',')
    {
      break;
    }
    buf += string.charAt(i);
  }
  
  message->message_id = messageID;
  message->port = port;
  buf.toCharArray(message->message, STRLEN);

  return true;
  
}

boolean execMessage(Message * message, State * state)
{
  char MSG_FORM[3][5] = {"STTP", "STFR", "STAR"};
  
  if(message->message_id != 201)
    return false;

  if(strncmp(message->message, MSG_FORM[0], 4) == 0)
  {
    return setThreshold(message->message, state);
  }
  else if(strncmp(message->message, MSG_FORM[1], 4) == 0)
  {
    
  }
  else if(strncmp(message->message, MSG_FORM[2], 4) == 0)
  {
    
  }

  return false;
}

boolean setThreshold(char * message, State * state)
{
  char * start, * buf, * pEnd = NULL;
  char * commaArray[5];
  double doubleArray[6], dubBuf;
  unsigned short count = 0;

//  buf = strchr(str,',');
//  while (buf!=NULL)
//  {
//    commaArray[count++] = buf;
//    buf=strchr(buf+1,',');
//  }

//  if(count != 5)
//    return false;

  start = strchr(message, '\\');

  count = 0;
  dubBuf = strtod(start + 1, &pEnd);
  while(*pEnd)
  {
    doubleArray[count++] = dubBuf;
    dubBuf = strtod(pEnd + 1, &pEnd);
  }

  if(count!=6)
    return false;

  Serial.println("read Threshold success");
  
  return true;  

  
}


