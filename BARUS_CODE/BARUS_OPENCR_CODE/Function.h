#ifndef Function_h
#define Function_h

#include "Arduino.h"
#include <DynamixelWorkbench.h>
#include <Servo.h>

// Function for reading strings from Raspberry Pi
int read_Int() 
{
  int Byte1 = 0;
  int Byte2 = 0;
  if(Serial.available() > 0)
  {
    Byte1 = Serial.read();
    Byte2 = Serial.read(); 
    return ((Byte1<<8) + Byte2);  
  }
  return -1; 
}

// Function for sending a string to RaspberryPi

void writeIntToRpi(int msg) 
{ 
  Serial.write(lowByte(msg));
  Serial.write(highByte(msg));
}

void checkBegin(bool* ptrIsBegin)
{
  int beginSignal = 420;
  while(!(*ptrIsBegin))
  {
    if(read_Int() == beginSignal)  
    {
      *ptrIsBegin = true;
      writeIntToRpi(beginSignal);
    }
  }
}

bool signalIsValid(int inputSignal)
{
  bool isOk = false;

  if(inputSignal >= 0 && inputSignal <=80000)
  {
    isOk = true;
  }

  return isOk;  
 }

#endif
