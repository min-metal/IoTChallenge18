#ifndef MESSAGE
#define MESSAGE

#include "State.h"

#define STRLEN 80

typedef struct 
{
  int message_id;
  int port;
  char message[STRLEN];
} Message;

boolean getMessage(String string, Message * message);
boolean execMessage(Message * message, State * state);
boolean setThreshold(char * message, State * state);

#endif

