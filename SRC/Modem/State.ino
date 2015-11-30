#include "State.h"
State::State(int lower, int upper, int area, LoRaModem * modem)
{
  this->lowerThreshold = lower;
  this->upperThreshold = upper;
  this->areaID = area;
  this->_modem = modem;
};



