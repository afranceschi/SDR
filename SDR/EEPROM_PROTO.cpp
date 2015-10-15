#include "EEPROM_PROTO.h"

byte EEPROM_PROTO::GetByte(unsigned int dir){
    return readEEPROM(MEM,dir);
}

void EEPROM_PROTO::SetByte(byte data, unsigned int dir){
    writeEEPROM(MEM,dir,data);
}

byte EEPROM_PROTO::CheckFormat(byte device){
  if(readEEPROM(device,1) == 0xAF){
    return 1;
  }else{
    return 0;
  }
}

void EEPROM_PROTO::writeEEPROM(byte deviceaddress, unsigned int eeaddress, byte data ){
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(5);
}

byte EEPROM_PROTO::readEEPROM(int deviceaddress, unsigned int eeaddress ){
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(deviceaddress,1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;
}

boolean EEPROM_PROTO::CompararCadena(const char* cad1,char* cad2){
    byte i = 0;
    boolean error = false;
    while(cad1[i] != '\0' && cad2[i] != '\0'){
        if(cad1[i] != cad2[i]){
            error=true;
            break;
        }
        i++;
    }
    if(error) return false; else return true;
}

boolean EEPROM_PROTO::CompararCadenaEPROMM(char *cad, unsigned int dir){
    byte c = (char) readEEPROM(MEM,dir);
    byte i = dir + 1;
    byte f = 0;
    boolean error = false;
    boolean coincide = false;
    while(c!=0 && !coincide){
        f=0;
        error = false;
        while(readEEPROM(MEM,i)!= '\0'){
            #ifdef DEBUG
            Serial.print("Comparando: ");
            Serial.print(cad[f]);
            Serial.print(" con ");
            Serial.print((char) readEEPROM(MEM,i));
            Serial.print(" ... ");
            #endif
            if(cad[f] != (char) readEEPROM(MEM,i)){
                error=true;
                #ifdef DEBUG
                Serial.println("FAIL");
                #endif
            }
            #ifdef DEBUG
            else{
                Serial.println("OK");
            }
            #endif
            i++;
            f++;
        }
        i++;
        if(!error) coincide = true; else c--;
    }

    if(coincide == true) return true; else return false;

}

void EEPROM_PROTO::AddCadenaEPROMM(char *cad, unsigned int dir){
    byte c = (char) readEEPROM(MEM, dir);
    byte i = 0;
    byte f = 0;

    if(i < c){
        while(i < c){
            while((char) readEEPROM(MEM,f+dir) != '\0'){
                f++;
            }
            f++;
            i++;
        }
    }else{
        f++;
    }

    i = 0;
    while(cad[i] != '\0'){
        writeEEPROM(MEM,f+dir,cad[i]);
        i++;
        f++;
    }
    writeEEPROM(MEM,f+dir,'\0');
    writeEEPROM(MEM,dir,c+1);
}

void EEPROM_PROTO::UpdateCadenaEPROMM(char *cad, byte t, unsigned int dir){
    byte c = (char) readEEPROM(MEM, dir);
    byte i = 0;
    byte f = 0;

    dir = GetPosicion(t,dir);

    while(cad[i] != '\0'){
        writeEEPROM(MEM,f+dir,cad[i]);
        i++;
        f++;
    }
    writeEEPROM(MEM,f+dir,'\0');
}

void EEPROM_PROTO::UpdateCadenaEPROMM(const char *cad, byte t, unsigned int dir){
    byte c = (char) readEEPROM(MEM, dir);
    byte i = 0;
    byte f = 0;

    dir = GetPosicion(t,dir);

    while(cad[i] != '\0'){
        writeEEPROM(MEM,f+dir,cad[i]);
        i++;
        f++;
    }
    writeEEPROM(MEM,f+dir,'\0');
}

int EEPROM_PROTO::GetPosicion(byte n, unsigned int dir){
    byte c = (char) readEEPROM(MEM, dir);
    byte i = 0;
    byte f = 0;

    if(i < n && i < c){
        while(i < n && i < c){
            while((char) readEEPROM(MEM,f+dir) != '\0'){
                f++;
            }
            f++;
            i++;
        }
    }else{
        f++;
    }
    Serial.print("Pos:");
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(n);
    if(i == n){
        return f+dir;
    }else{
        return -1;
    }
}
