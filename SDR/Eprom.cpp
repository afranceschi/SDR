#include "Eprom.h"

//#define SDA 30
//#define SCL 29

void Eprom_Lib::Init(){
	Wire.begin();
}

void Eprom_Lib::Clock(){
	digitalWrite(SCL,LOW);
	delay(100);
	digitalWrite(SCL,HIGH);
	delay(100);
}

void Eprom_Lib::SendBit(uint8 bit){
	digitalWrite(SDA,bit);
	this->Clock();
}

byte Eprom_Lib::GetByte(unsigned int dir){
    return readEEPROM(MEM,dir);
}

void Eprom_Lib::SetByte(byte data, unsigned int dir){
    writeEEPROM(MEM,dir,data);
}

byte Eprom_Lib::CheckFormat(byte device){

	//////SerialUSB.print("Leido de mem: 0x");
	//////SerialUSB.println(readEEPROM(device,1),HEX);
	byte x;
	uint8 i = 0;
	for(i = 0; i<3; i++){
		x = readEEPROM(device,1);
		Wire.begin();
		//SerialUSB.print("Flag de Format: 0x");
		//SerialUSB.println(x,HEX);
	}
	if(x == 0xAF) return 1; else return 0;
}

void Eprom_Lib::writeEEPROM(byte deviceaddress, unsigned int eeaddress, byte data ){
	Wire.beginTransmission(deviceaddress);
	//delay(5);
	Wire.send((int)(eeaddress >> 8));   // MSB
	//delay(5);
	Wire.send((int)(eeaddress & 0xFF)); // LSB
	//delay(5);
	Wire.send(data);
	//delay(5);
	Wire.endTransmission();

	delay(10);
}

/*void Eprom_Lib::writeEEPROM_32(byte deviceaddress, unsigned int eeaddress, uint32 data){
	//data = 0x12345678;

	uint8 a = (data >> 16) >> 8;
	uint8 b = (data >> 16) & 0xFF;
	uint8 c = (data >> 8) & 0xFF;
	uint8 d = data & 0xFF;
	////SerialUSB.print("DATA: ");
	////SerialUSB.print(a,HEX);
	////SerialUSB.print(" ");
	////SerialUSB.print(b,HEX);
	////SerialUSB.print(" ");
	////SerialUSB.print(c,HEX);
	////SerialUSB.print(" ");
	////SerialUSB.println(d,HEX);

	Wire.beginTransmission(deviceaddress);
	Wire.send((int)(eeaddress >> 8));   // MSB
	Wire.send((int)(eeaddress & 0xFF)); // LSB
	Wire.send(a);
	Wire.send(b);
	Wire.send(c);
	Wire.send(d);
	Wire.endTransmission();

}*/

void Eprom_Lib::StartWrite(byte deviceaddress, unsigned int eeaddress){
	//////SerialUSB.println("Inicio de guardado.");
	Wire.beginTransmission(deviceaddress);
	//////SerialUSB.print("Posicion: H: 0x");
	//////SerialUSB.print((eeaddress >> 8),HEX);
	//////SerialUSB.print(" L: 0x");
	//////SerialUSB.println((eeaddress & 0xFF),HEX);
	Wire.send((int)(eeaddress >> 8));   // MSB
	Wire.send((int)(eeaddress & 0xFF)); // LSB
}

void Eprom_Lib::Write(byte data){
	//////SerialUSB.print("Guardado: ");
	//////SerialUSB.print((char) data);
	//////SerialUSB.print(" | 0x");
	//////SerialUSB.println(data,HEX);
	Wire.send(data);
	//delay(5);
}

void Eprom_Lib::StopWrite(){
	Wire.endTransmission();
	delay(10);
	//////SerialUSB.println("Fin de guardado.");
}

byte Eprom_Lib::readEEPROM(int deviceaddress, unsigned int eeaddress ){
  byte rdata = 0xFF;
  do{
	  //////SerialUSB.print("Leyendo posicion: 0x");
	  //////SerialUSB.println(eeaddress,HEX);
	  Wire.beginTransmission(deviceaddress);
	  Wire.send((int)(eeaddress >> 8));   // MSB
	  Wire.send((int)(eeaddress & 0xFF)); // LSB
	  Wire.endTransmission();
	  //delay(100);
	  //////SerialUSB.println("Esperando Dato de Memoria.");
	  Wire.requestFrom(deviceaddress,1);
	  //////SerialUSB.print("Tiempo de espera: ");
	  //////SerialUSB.println(i+5);
	  if(!Wire.available()){
		  //#ifdef DEBUG
		  	  ////SerialUSB.println("\tReiniciando Memoria.");
		  //#endif
		  Wire.begin();
		  delay(500);
	  }
  }while(!Wire.available());

  rdata = Wire.receive();
  //if (Wire.available()) rdata = Wire.receive();
  //////SerialUSB.print("Leido: 0x");
  //////SerialUSB.println(rdata,HEX);
  //delay(2);
  return rdata;
}

boolean Eprom_Lib::CompararCadena(const char* cad1,char* cad2){
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

boolean Eprom_Lib::CompararCadena(char* cad1,char* cad2){
    byte i = 0;
    boolean error = false;
    if((cad1[0] != 0 && cad2[0] != 0) || (cad1[0] == 0 && cad2[0] == 0)){
    	while(cad1[i] != '\0' && cad2[i] != '\0'){
    		if(cad1[i] != cad2[i]){
    			error=true;
    			break;
    		}
    		i++;
    	}
    }else{
    	error = true;
    }
    if(error) return false; else return true;
}

boolean Eprom_Lib::CompararCadenaEPROMM(char *cad, unsigned int dir){
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

void Eprom_Lib::AddCadenaEPROMM(char *cad, unsigned int dir){
	byte c = readEEPROM(MEM, dir);
    delay(10);
	//#ifdef DEBUG
    	//SerialUSB.print("Guardando: ");
    	//SerialUSB.print(cad);
    	//SerialUSB.print(" en Posicion: ");
    	//SerialUSB.println(c);
	//#endif
    byte i = 0;
    byte f = 0;

    if(i < c){
        while(i < c){
            while(readEEPROM(MEM,f+dir) != 0){
                f++;
            }
            f++;
            i++;
        }
    }else{
        f++;
    }

    //delay(10);

    i = 0;
    //this->StartWrite(MEM,dir+f);
    while(cad[i] != '\0'){
        writeEEPROM(MEM,f+dir,cad[i]);
    	//SerialUSB.print(cad[i]);
    	//SerialUSB.print(" en 0x");
    	//SerialUSB.println(f+dir,HEX);
        //this->Write(cad[i]);
    	i++;
        f++;
    }
    writeEEPROM(MEM,f+dir,'\0');
    //this->Write('\0');
    //this->StopWrite();

    //delay(10);

    c++;
    //this->StartWrite(MEM,dir);
    //this->Write(c);
    //this->StopWrite();
    writeEEPROM(MEM,dir,c);
}

void Eprom_Lib::AddCadenaEPROMM(char car, unsigned int dir){
    byte c = (char) readEEPROM(MEM, dir);
    delay(10);
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
    this->StartWrite(MEM,dir+f);
    this->Write((byte) car);
    //writeEEPROM(MEM,f+dir,'\0');
    this->Write('\0');
    this->StopWrite();

    writeEEPROM(MEM,dir,c+1);
}
void Eprom_Lib::UpdateCadenaEPROMM(const char *cad, byte t, unsigned int dir){
		//byte c = (char) readEEPROM(MEM, dir);
	    byte i = 0;
	    //byte f = 0;

	    dir = GetPosicion(t,dir);

	    this->StartWrite(MEM,dir);

	    while(cad[i] != '\0'){
	        //writeEEPROM(MEM,f+dir,cad[i]);
	        	this->Write(cad[i]);
	    	i++;
	        //f++;
	    		dir++;
	    }
	    //writeEEPROM(MEM,f+dir,'\0');
	    	this->Write('\0');
	    	this->StopWrite();
}

void Eprom_Lib::UpdateCadenaEPROMM(char *cad, byte t, unsigned int dir){
    //byte c = (char) readEEPROM(MEM, dir);
    byte i = 0;
    //byte f = 0;

    dir = GetPosicion(t,dir);

    this->StartWrite(MEM,dir);

    while(cad[i] != '\0'){
        //writeEEPROM(MEM,f+dir,cad[i]);
        	this->Write(cad[i]);
    	i++;
        //f++;
    		dir++;
    }
    //writeEEPROM(MEM,f+dir,'\0');
    	this->Write('\0');
    	this->StopWrite();
}

char Eprom_Lib::GetCadena(char* buff, byte t, unsigned int dir){
    //byte cant = GetByte(dir);
    byte i = 0;
    //byte c = readEEPROM(MEM,dir);

    //SerialUSB.print("Cantidad de registros: 0x");
    //SerialUSB.println(c,HEX);

    //SerialUSB.print("POS Inicial: 0x");
    //SerialUSB.println(dir,HEX);

    dir = GetPosicion(t-1,dir);

    //SerialUSB.print("POS a Buscar: 0x");
    //SerialUSB.println(dir,HEX);

    if(dir != 0){
    	while(readEEPROM(MEM,dir+i) != 0){
    		buff[i] = readEEPROM(MEM,dir+i);
    		//SerialUSB.print("Leido: ");
    		//SerialUSB.print(buff[i]);
    		//SerialUSB.print(" en 0x");
    		//SerialUSB.println(dir+i,HEX);
    		i++;
    	}

    	buff[i] = 0;
    	return  0;
    }else{
    	buff[0] = 0;
    	return -1;
    }

    //Serial.println(buff);
}

unsigned int Eprom_Lib::GetPosicion(byte n, unsigned int dir){
    byte c = readEEPROM(MEM, dir);
    byte i = 0;
    byte f = 0;

    ////SerialUSB.print("\tC: 0x");
    ////SerialUSB.println(c,HEX);

    if(c==0) return 0;
    if(n == c) return 0;

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
    #ifdef DEBUG
        Serial.print("Pos:");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(n);
    #endif
    if(i == n){
        return f+dir;
    }else{
        return 0;
    }
}

