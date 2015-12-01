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


