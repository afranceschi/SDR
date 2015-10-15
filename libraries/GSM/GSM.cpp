#include "GSM.h"

void GSM::begin(){

	byte i = 0;
	boolean Iniciado = false;

	Serial2.begin(9600);

	while(!Iniciado){
		for(i = 0; i<10; i++){
			SendCMD("AT");
			if(esperarOK()) Iniciado = true;
		}
		if(!Iniciado) PowerSignal();
	}
}

boolean GSM::CheckSMS(){
	SendCMD("AT+CMGL=\"ALL\"");
	while(!Read()) SerialUSB.println("Esperando Respuesta.");
	SerialUSB.println((char *) comm_buff);
	return true;
}

void GSM::SendCMD(const char *cmd){
	Serial2.println(cmd);
	delay(500);
}

void GSM::SendCMD(char *cmd){
	Serial2.println(cmd);
	delay(500);
}

boolean GSM::esperarOK(){
	uint16 i = 0;;
	while(!this->Read() && i != 255) i++;

	if(i == 255) return false;

	i = 0;
	while(comm_buff[i] != 'O' && comm_buff[i] != 0x00) i++;
	if(comm_buff[i] == 0x00) return false;

	while(comm_buff[i] != 'K' && comm_buff[i] != 0x00) i++;
	if(comm_buff[i] == 0x00) return false;

	return true;
}

 boolean GSM::Read(){
	uint8 i = 0;
	comm_buff[0] = 0x00;
	if(Serial2.available()){
		while(Serial2.available()){
			SerialUSB.print("Buffer: ");
			SerialUSB.println(Serial2.available());
			i = 0;
			do{
				comm_buff[i] = Serial2.read();
				SerialUSB.print((char)comm_buff[i]);
				i++;
			}while(i < 512 && Serial2.available());
			comm_buff[i] = 0x00;
		}
		return true;
	}else{
		return false;
	}
}

void GSM::PowerSignal(){
	pinMode(PWR_GSM_PIN,OUTPUT);
	digitalWrite(PWR_GSM_PIN,HIGH);
	delay(1200);
	digitalWrite(PWR_GSM_PIN,LOW);
	delay(3000);
}


