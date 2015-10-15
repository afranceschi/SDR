/*
  LOG.cpp - Library for standard logging convention.
  Created by Meir Michanie, June 9, 2010.
  Released into the public domain.
  Version 0.1
*/

#include "LOG.h"

LOG::LOG(int level)
{
  setLevel(level);
}

void LOG::DATA(const char* string)
{
 if (_level > 4){
  SerialUSB.print(string);
 }
}

void LOG::DATA(int number)
{
 if (_level > 4){
  SerialUSB.print(number);
 }
}

void LOG::DEBUG(const char* string)
{
 if (_level > 3){
  SerialUSB.print("\n[DEBUG]: ");
  SerialUSB.println(string);
 }
}

void LOG::INFO(const char* string)
{
 if (_level > 2){
   SerialUSB.print("\n[INFO]: ");
   SerialUSB.println(string);
 }
}

void LOG::WARNING(const char* string)
{
 if (_level > 1){
  SerialUSB.print("\n[WARNING]: ");
  SerialUSB.println(string);
 }
}

void LOG::CRITICAL(const char* string)
{
 if (_level > 0){
  SerialUSB.print("\n[CRITICAL]: ");
  SerialUSB.println(string);
 }
}
