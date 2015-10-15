#include "Registro.h"

void REGISTRO::Init(char *nom, uint8 port, unsigned int dir){
	this->DIR = dir;
	strcpy(this->Nombre,nom);
	this->Configuracion = 0;
	this->Puerto = port;
	this->Act_Salida_Low = 0;
	this->Act_Salida_High = 0;
	this->Dec_Salida_Low = 0;
	this->Dec_Salida_High = 0;
	this->Act_Ventana_High = 0;
	this->Act_Ventana_Low = 0;
	this->Dec_Ventana_High = 0;
	this->Dec_Ventana_Low = 0;
	this->Intervalo_Apagado = 0;
	this->Intervalo_Prendido = 0;
	this->Hora_AP_Ventana = 0;
	this->Hora_IN_Ventana = 0;
	this->Activo_flag = false;
	this->Started = false;
	this->ACT_MAN = false;
	this->SalidaOn = false;
}

void REGISTRO::Init(const char *nom, uint8 port, unsigned int dir){
	this->DIR = dir;
	strcpy(this->Nombre,nom);
	this->Configuracion = 0;
	this->Puerto = port;
	this->Act_Salida_Low = 0;
	this->Act_Salida_High = 0;
	this->Dec_Salida_Low = 0;
	this->Dec_Salida_High = 0;
	this->Act_Ventana_High = 0;
	this->Act_Ventana_Low = 0;
	this->Dec_Ventana_High = 0;
	this->Dec_Ventana_Low = 0;
	this->Intervalo_Apagado = 0;
	this->Intervalo_Prendido = 0;
	this->Hora_AP_Ventana = 0;
	this->Hora_IN_Ventana = 0;
	this->Activo_flag = false;
	this->Started = false;
	this->ACT_MAN = false;
	this->SalidaOn = false;
}

void REGISTRO::SetDir(unsigned int dir){
	this->DIR = dir;
}

void REGISTRO::SetEprom(Eprom_Lib *ep){
	this->EP = ep;
}

void REGISTRO::SetNombre(char* nom){
	strcpy(this->Nombre,nom);
	this->GuardarRegistro();
}

void REGISTRO::SetNombre(const char* nom){
	strcpy(this->Nombre,nom);
	this->GuardarRegistro();
}

void REGISTRO::SetIntApagado(char* intervalo){
	this->Intervalo_Apagado = this->StoI(intervalo);
	this->GuardarRegistro();
}

void REGISTRO::SetIntEncendido(char* intervalo){
	this->Intervalo_Prendido = this->StoI(intervalo);
	this->GuardarRegistro();
}

void REGISTRO::SetRangoOn(char* hora){
	this->Hora_IN_Ventana = this->StoI(hora);
	this->GuardarRegistro();
}

void REGISTRO::SetRangoOff(char* hora){
	this->Hora_AP_Ventana = this->StoI(hora);
	this->GuardarRegistro();
}

void REGISTRO::Start(){
	if(Hora_AP_Ventana > 0 && Hora_IN_Ventana > 0 && Intervalo_Apagado > 0 && Intervalo_Prendido > 0){
		this->Started = true;
	}else{
		this->Started = false;
	}
}

void REGISTRO::Stop(){
	this->Started = false;
}

boolean REGISTRO::isStarted(){
	return this->Started;
}

bool REGISTRO::CheckNombre(const char* nom){
	return this->EP->CompararCadena(nom,this->Nombre);
}

bool REGISTRO::CheckNombre(char* nom){
	return this->EP->CompararCadena(nom,this->Nombre);
}

void REGISTRO::SetPort(uint8 port){
	this->Puerto = port;
}

uint8 REGISTRO::GetPort(){
	return this->Puerto;
}

void REGISTRO::GuardarRegistro(){
	char buff[20];
	buff[0] = 0;
	this->EP->SetByte(0,this->DIR);

	buff[0] = 0;
	if(this->isStarted()) this->Configuracion = 1; else this->Configuracion = 0;
	this->ItoS(buff,this->Configuracion);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Puerto);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	this->EP->AddCadenaEPROMM(this->Nombre, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Act_Salida_High);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Act_Salida_Low);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Dec_Salida_High);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Dec_Salida_Low);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Act_Ventana_High);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Act_Ventana_Low);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Dec_Ventana_High);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Dec_Ventana_Low);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Intervalo_Apagado);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Intervalo_Prendido);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Hora_IN_Ventana);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	buff[0] = 0;
	this->ItoS(buff,this->Hora_AP_Ventana);
	this->EP->AddCadenaEPROMM(buff, this->DIR);

	this->VerRegistro();
}

void REGISTRO::CargarRegistro(){
	char buff[20];
	this->EP->GetCadena(buff,POS_CONFIGURACION,this->DIR);
	this->Configuracion = this->StoI(buff);
	if(this->Configuracion == 1) this->Start(); else this->Stop();

	this->EP->GetCadena(buff,POS_PUERTO,this->DIR);
	this->Puerto = this->StoI(buff);

	this->EP->GetCadena(buff,POS_NOMBRE,this->DIR);
	strcpy(this->Nombre,buff);

	this->EP->GetCadena(buff,POS_ACT_SALIDA_HIGH,this->DIR);
	this->Act_Salida_High = this->StoI(buff);

	this->EP->GetCadena(buff,POS_ACT_SALIDA_LOW,this->DIR);
	this->Act_Salida_Low = this->StoI(buff);

	this->EP->GetCadena(buff,POS_DEC_SALIDA_HIGH,this->DIR);
	this->Dec_Salida_High = this->StoI(buff);

	this->EP->GetCadena(buff,POS_DEC_SALIDA_LOW,this->DIR);
	this->Dec_Salida_Low = this->StoI(buff);

	this->EP->GetCadena(buff,POS_ACT_VENTANA_HIGH,this->DIR);
	this->Act_Ventana_High = this->StoI(buff);

	this->EP->GetCadena(buff,POS_ACT_VENTANA_LOW,this->DIR);
	this->Act_Ventana_Low = this->StoI(buff);

	this->EP->GetCadena(buff,POS_DEC_VENTANA_HIGH,this->DIR);
	this->Dec_Ventana_High = this->StoI(buff);

	this->EP->GetCadena(buff,POS_DEC_VENTANA_LOW,this->DIR);
	this->Dec_Ventana_Low = this->StoI(buff);

	this->EP->GetCadena(buff,POS_INTERVALO_APAGADO,this->DIR);
	this->Intervalo_Apagado = this->StoI(buff);

	this->EP->GetCadena(buff,POS_INTERVALO_PRENDIDO,this->DIR);
	this->Intervalo_Prendido = this->StoI(buff);

	this->EP->GetCadena(buff,POS_HORA_IN_VENTANA,this->DIR);
	this->Hora_IN_Ventana = this->StoI(buff);

	this->EP->GetCadena(buff,POS_HORA_AP_VENTANA,this->DIR);
	this->Hora_AP_Ventana = this->StoI(buff);

	this->VerRegistro();
}

void REGISTRO::VerRegistro(){
	SerialUSB.print("Configuracion: ");
	SerialUSB.println(this->Configuracion);

	SerialUSB.print("Puerto: ");
	SerialUSB.println(this->Puerto);

	SerialUSB.print("Nombre: ");
	SerialUSB.println(this->Nombre);

	SerialUSB.print("Activacion de Salida: H: ");
	SerialUSB.print(this->Act_Salida_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Act_Salida_Low);

	SerialUSB.print("Desactivacion de Salida: H: ");
	SerialUSB.print(this->Dec_Salida_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Dec_Salida_Low);

	SerialUSB.print("Activacion de Ventana: H: ");
	SerialUSB.print(this->Act_Ventana_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Act_Ventana_Low);

	SerialUSB.print("Desactivacion de Ventana: H: ");
	SerialUSB.print(this->Dec_Ventana_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Dec_Ventana_Low);

	SerialUSB.print("Intervalo Apagado: ");
	SerialUSB.println(this->Intervalo_Apagado);

	SerialUSB.print("Itervalo Prendido: ");
	SerialUSB.println(this->Intervalo_Prendido);

	SerialUSB.print("Hora Apagado: ");
	SerialUSB.println(this->Hora_AP_Ventana);

	SerialUSB.print("Hora Prendido: ");
	SerialUSB.println(this->Hora_IN_Ventana);
}

void REGISTRO::VerRegistro(char *buff){
	buff[0] = 0;
	char aux[20];

	Concatenar(buff,"Activacion: ");
	ItoS(aux,this->Configuracion);
	Concatenar(buff,aux);
	Concatenar(buff,"\n");

	Concatenar(buff,"Puerto: ");
	ItoS(aux,this->Puerto);
	Concatenar(buff,aux);
	Concatenar(buff,"\n");

	Concatenar(buff,"Nombre: ");
	Concatenar(buff,this->Nombre);
	Concatenar(buff,"\n");
/*
	SerialUSB.print("Activacion de Salida: H: ");
	SerialUSB.print(this->Act_Salida_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Act_Salida_Low);

	SerialUSB.print("Desactivacion de Salida: H: ");
	SerialUSB.print(this->Dec_Salida_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Dec_Salida_Low);

	SerialUSB.print("Activacion de Ventana: H: ");
	SerialUSB.print(this->Act_Ventana_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Act_Ventana_Low);

	SerialUSB.print("Desactivacion de Ventana: H: ");
	SerialUSB.print(this->Dec_Ventana_High);
	SerialUSB.print(" L: ");
	SerialUSB.println(this->Dec_Ventana_Low);
*/
	Concatenar(buff,"Intervalo Apagado: ");
	ItoS(aux,this->Intervalo_Apagado);
	Concatenar(buff,aux);
	Concatenar(buff,"\n");

	Concatenar(buff,"Intervalo Prendido: ");
	ItoS(aux,this->Intervalo_Prendido);
	Concatenar(buff,aux);
	Concatenar(buff,"\n");

	Concatenar(buff,"Hora Apagado: ");
	ItoS(aux,this->Hora_AP_Ventana);
	Concatenar(buff,aux);
	Concatenar(buff,"\n");

	Concatenar(buff,"Hora Prendido: ");
	ItoS(aux,this->Hora_IN_Ventana);
	Concatenar(buff,aux);
}

void REGISTRO::ItoS(char *str, uint32 num){
	if(num == 0){
		strcpy(str,"0");
	}else{
		uint32 aux = num;
		int n;
		int i = 0;
		char straux[20];
		while(num != 0){
			aux = num;
			aux = aux / 10;
			aux = aux * 10;
			n = num - aux;
			straux[i] = (0x30 + n);
			i++;
			num = num / 10;
		}
		i--;
		int f = 0;
		while(i!=-1){
			str[f] = straux[i];
			i--;
			f++;
		}
		str[f] = '\0';
	}
}

uint32 REGISTRO::StoI(char *txt){
	uint32 num;
	num = atoi(txt);
	return num;
	//int i = 0;
	//while(txt[i]!=0){

	//}
}

uint32 REGISTRO::StoI(const char *txt){
	uint32 num;
	num = atoi(txt);
	return num;
}

void REGISTRO::Activar(){
	this->Activo_flag = true;
}

void REGISTRO::Desactivar(){
	digitalWrite(this->Puerto,LOW);
	this->Activo_flag = false;
}

void REGISTRO::Checktiempo(uint32 hora, uint32 fecha){
	if(Activo_flag){
		if(!digitalRead(Puerto)){
			if(hora >= Act_Salida_Low && fecha == Act_Salida_High){
				//digitalWrite(23,HIGH);
				Dec_Salida_Low = SumarHora(hora,Intervalo_Prendido);
				if(Dec_Salida_Low < Act_Salida_Low){
					Dec_Salida_High = SumarUnDia(fecha);
				}else{
					Dec_Salida_High = fecha;
				}
				GuardarRegistro();
				this->SalidaOn = true;
			}
		}else{
			if(hora >= Dec_Salida_Low && fecha == Dec_Salida_High){
				//digitalWrite(23,LOW);
				Act_Salida_Low = SumarHora(hora,Intervalo_Apagado);
				if(Act_Salida_Low < Dec_Salida_Low){
					Dec_Salida_High = SumarUnDia(fecha);
				}else{
					Dec_Salida_High = fecha;
				}
				GuardarRegistro();
				this->SalidaOn = false;
			}
		}
	}
}

uint32 REGISTRO::SumarHora(uint32 hora, uint32 cantidad){
	uint32 buff;
	uint8 h = 0,m = 0,s = 0;
	uint8 h1 = 0,m1 = 0,s1 = 0;

	SerialUSB.print("Hora: ");
	SerialUSB.println(hora);
	SerialUSB.print("Hora a sumar: ");
	SerialUSB.println(cantidad);
	SerialUSB.println("");
	h = hora / 10000;
	m = (hora / 100) - (h * 100);
	s = hora - ((m + (h * 100))) * 100;

	h1 = cantidad / 10000;
	m1 = (cantidad / 100) - (h1 * 100);
	s1 = cantidad - ((m1 + (h1 * 100))) * 100;

	h += h1;
	m += m1;
	s += s1;

	while(s > 60){m++; s -= 60;}
	while(m > 60){h++; m -= 60;}

	buff = 0;
	buff += (h * 10000);
	buff += (m * 100);
	buff += s;

	SerialUSB.print("Hora sumada: ");
	SerialUSB.println(buff);

	return buff;
}

void REGISTRO::CheckVentana(uint32 hora, uint32 fecha){
	if((Hora_AP_Ventana > Hora_IN_Ventana)){
		if(hora > (Hora_IN_Ventana*100) && hora < (Hora_AP_Ventana*100)){
			if(!Activo_flag) Act_Salida_Low = hora;
			if(!Activo_flag) Act_Salida_High = fecha;
			SerialUSB.println("Rango en ejecucion.");
			Activo_flag = true;
		}else{
			Activo_flag = false;
			digitalWrite(Puerto,LOW);
		}
	}else{
		if(hora > (Hora_IN_Ventana*100) || hora < (Hora_AP_Ventana*100)){
			if(!Activo_flag) Act_Salida_Low = hora;
			if(!Activo_flag) Act_Salida_High = fecha;
			Activo_flag = true;
		}else{
			Activo_flag = false;
			digitalWrite(Puerto,LOW);
		}
	}
}

uint32 REGISTRO::SumarUnDia(uint32 fecha){
	uint32 buff = 0;
	uint8 ano = 0;
	uint8 mes = 0;
	uint8 dia = 0;

	ano = fecha / 10000;
	mes = (fecha / 100) - (ano * 100);
	dia = fecha - ((mes + (ano * 100))) * 100;

	dia++;

	if(mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10 || mes==12){
		if(dia > 31){
			dia -= 31;
			mes++;
			if(mes > 12){
				mes -= 12;
				ano++;
			}
		}
	}else if(mes==4 || mes==6 || mes==9 || mes==11){
		if(dia > 30){
			dia -=30;
			mes++;
			if(mes > 12){
				mes -= 12;
				ano++;
			}
		}
	}else if(mes==2){
		if(!esBisiesto(ano)){
			if(dia > 28){
				dia -= 28;
				mes++;
				if(mes>12){
					mes -= 12;
					ano++;
				}
			}
		}else{
			if(dia > 29){
				dia -= 29;
				mes++;
				if(mes>12){
					mes -= 12;
					ano++;
				}
			}
		}
	}

	buff = 0;
	buff += (ano * 10000);
	buff += (mes * 100);
	buff += dia;
	return buff;
}

uint32 REGISTRO::SumarDias(uint8 dias, uint32 fecha){
	uint8 i = 0;
	for(i = 0; i < dias; i++) fecha = this->SumarUnDia(fecha);
	return fecha;
}

boolean REGISTRO::esBisiesto(uint32 ano){
	if(ano%4==0 && (ano%100!=0 || ano%400==0)) return true; else return false;
}

void REGISTRO::SetActivacionManual(boolean act){
	//if(act) digitalWrite(this->Puerto, HIGH); else digitalWrite(this->Puerto, LOW);
	this->ACT_MAN = act;
}

boolean REGISTRO::GetActivacionManual(){
	return this->ACT_MAN;
}

boolean REGISTRO::isActive(){
	return this->SalidaOn;
}

void REGISTRO::Concatenar(char *a, char *b){
	uint8 i = 0;
	uint8 f = 0;

	while(a[i] != 0){
		i++;
	}

	while(b[f] != 0){
		a[i] = b[f];
		i++;
		f++;
	}

	a[i] = 0;
}

void REGISTRO::Concatenar(char *a, const char *b){
	uint8 i = 0;
	uint8 f = 0;

	while(a[i] != 0){
		i++;
	}

	while(b[f] != 0){
		a[i] = b[f];
		i++;
		f++;
	}

	a[i] = 0;
}
