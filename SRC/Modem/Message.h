typedef struct 
{
  int message_id;
  int port;
  char message[STRLEN];
} Message;

boolean getMessage(String string, Message * message);

