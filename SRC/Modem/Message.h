#ifndef MESSAGE
#define MESSAGE

#define STRLEN 50

typedef struct 
{
  int message_id;
  int port;
  char message[STRLEN];
} Message;

boolean getMessage(String string, Message * message);

#endif

