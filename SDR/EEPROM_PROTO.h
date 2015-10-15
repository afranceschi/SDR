#ifndef _EEPROM_PROTO_H_
#define _EEPROM_PROTO_H_

#include "Arduino.h"
#include <Wire.h>

#define MEM 0x50

class EEPROM_PROTO{

    private:
        void writeEEPROM(byte deviceaddress, unsigned int eeaddress, byte data );
        byte readEEPROM(int deviceaddress, unsigned int eeaddress );

    public:
        void Format();
        byte GetByte(unsigned int dir);
        void SetByte(byte data, unsigned int dir);
        virtual void Format_ACT();
        byte CheckFormat(byte device);
        boolean CompararCadena(const char* cad1, char* cad2);
        boolean CompararCadenaEPROMM(char *cad, unsigned int dir);
        void AddCadenaEPROMM(char *cad, unsigned int dir);
        void UpdateCadenaEPROMM(char *cad, byte t, unsigned int dir);
        void UpdateCadenaEPROMM(const char *cad, byte t, unsigned int dir);
        int GetPosicion(byte n, unsigned int dir);

};

#endif
