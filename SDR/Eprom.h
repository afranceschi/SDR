#ifndef _EEP_H
#define _EEP_H

#include <Wire/Wire.h>
#include <wirish/wirish.h>
//#include <libmaple/i2c.h>

#define MEM 0x50

class Eprom_Lib{

	private:
        void writeEEPROM(byte deviceaddress, unsigned int eeaddress, byte data);
        void writeEEPROM_32(byte deviceaddress, unsigned int eeaddress, uint32 data);
        byte readEEPROM(int deviceaddress, unsigned int eeaddress );


    public:
        Eprom_Lib(void);
        void w(){
        	writeEEPROM_32(MEM, 0, 0x123456);
        }
        void Init();
        void StartWrite(byte deviceaddress, unsigned int eeaddress);
        void Write(byte data);
        void StopWrite();

        void Clock();
        void SendBit(uint8 bit);
        void Format();
        byte GetByte(unsigned int dir);
        void SetByte(byte data, unsigned int dir);
        //virtual void Format_ACT();
        byte CheckFormat(byte device);
        boolean CompararCadena(const char* cad1, char* cad2);
        boolean CompararCadena(char* cad1, char* cad2);
        boolean CompararCadenaEPROMM(char *cad, unsigned int dir);
        void AddCadenaEPROMM(char *cad, unsigned int dir);
        void AddCadenaEPROMM(char car, unsigned int dir);
        void UpdateCadenaEPROMM(char *cad, byte t, unsigned int dir);
        void UpdateCadenaEPROMM(const char *cad, byte t, unsigned int dir);
        char GetCadena(char* buff, byte t, unsigned int dir);
        unsigned int GetPosicion(byte n, unsigned int dir);
};

#endif

