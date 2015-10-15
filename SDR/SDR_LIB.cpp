#include "SDR_LIB.h"

void SDR_LIB::Init(){
    InitGSM();
    Senal(5,150);
    InitMEM();
    Senal(5,350);

    #ifdef FORMAT
        FormatMem(MEM);
    #endif // FORMATB

    //SerialUSB.println("Cargando Registros.");
    REGS0.SetDir(REGISTRO0_DIR);
    REGS1.SetDir(REGISTRO1_DIR);
    REGS2.SetDir(REGISTRO2_DIR);
    REGS3.SetDir(REGISTRO3_DIR);
    REGS4.SetDir(REGISTRO4_DIR);
    REGS5.SetDir(REGISTRO5_DIR);
    REGS6.SetDir(REGISTRO6_DIR);
    REGS7.SetDir(REGISTRO7_DIR);

    this->GetPassword();
    this->CargarTelefonos();
    this->CargarRegistros();
    this->CargarEntradas();

    pinMode(REGS0.GetPort(),OUTPUT);
    pinMode(REGS1.GetPort(),OUTPUT);
    pinMode(REGS2.GetPort(),OUTPUT);
    pinMode(REGS3.GetPort(),OUTPUT);
    pinMode(REGS4.GetPort(),OUTPUT);
    pinMode(REGS5.GetPort(),OUTPUT);
    pinMode(REGS6.GetPort(),OUTPUT);
    pinMode(REGS7.GetPort(),OUTPUT);

    pinMode(ENT0.Port,INPUT);
    pinMode(ENT1.Port,INPUT);
    pinMode(ENT2.Port,INPUT);
    pinMode(ENT3.Port,INPUT);
    pinMode(ENT4.Port,INPUT);
    pinMode(ENT5.Port,INPUT);
    pinMode(ENT6.Port,INPUT);

    if(!gsm_local.IsInitialized()){
    	SerialUSB.println("Inicializando GSM");
    	gsm_local.begin(9600);
    }
    SerialUSB.println(gsm_local.IsRegistered());

    SerialUSB.print("Entrada 0: ");
    SerialUSB.print(ENT0.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT0.Port, DEC);

    SerialUSB.print("Entrada 1: ");
    SerialUSB.print(ENT1.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT1.Port, DEC);

    SerialUSB.print("Entrada 2: ");
    SerialUSB.print(ENT2.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT2.Port, DEC);

    SerialUSB.print("Entrada 3: ");
    SerialUSB.print(ENT3.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT3.Port, DEC);

    SerialUSB.print("Entrada 4: ");
    SerialUSB.print(ENT4.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT4.Port, DEC);

    SerialUSB.print("Entrada 5: ");
    SerialUSB.print(ENT5.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT5.Port, DEC);

    SerialUSB.print("Entrada 6: ");
    SerialUSB.print(ENT6.Nombre);
    SerialUSB.print(" en ");
    SerialUSB.println(ENT6.Port, DEC);

    SMS.ConfigHora();

    EnviarATodos("Sistema Iniciado.");
    //REGS1.Start();

}

void SDR_LIB::Main(){
	CheckEntradas();
	CheckSalidas();

	uint32 bufint = SMS.GetHora();
	if(bufint > 0) arrayHora = bufint;
	bufint = SMS.GetFecha();
	if(bufint > 0) arrayFecha = bufint;
	SerialUSB.println(arrayHora);

    if(Check_SMS(telefono,mensaje)){
    	SerialUSB.print("Mensaje de: ");
    	SerialUSB.print(telefono);
    	SerialUSB.print(", ");
    	SerialUSB.println(mensaje);
        GetCmd(mensaje,CMD,ARG1,ARG2,ARG3);

        if(MEMO->CompararCadena("FORMAT",CMD)){
        	if(this->CheckPassword(ARG1)){
        		this->EnviarSMS(telefono,"Formateando Memoria.");
        		FormatMem(MEM);
        		this->EnviarSMS(telefono,"Formato Completo.");
        	}else{
        		this->EnviarSMS(telefono,"Password Incorrecto");
        	}
        }

        else if(MEMO->CompararCadena("REGISTROS",CMD)) VerRegistros();

        else if(MEMO->CompararCadena("ADD",CMD)){
        	if(this->CheckPassword(ARG1)){
        		if(!this->BuscarTelefono(telefono)){
        			this->MEMO->AddCadenaEPROMM(telefono,TELEFONOS_DIR);
        			this->CargarTelefonos();
        			this->EnviarSMS(telefono,"Telefono Habilitado.");
        		}else{
        			SerialUSB.println("El telefono ya se encuentra habilitado.");
        			this->EnviarSMS(telefono,"El Telefono ya existe.");
        		}
        	}
        }
        else if(this->BuscarTelefono(telefono)){
        	if(MEMO->CompararCadena("TEST",CMD)){
        		togglePin(REGS0.GetPort());
        	    togglePin(REGS1.GetPort());
        	    togglePin(REGS2.GetPort());
        	    togglePin(REGS3.GetPort());
        	    togglePin(REGS4.GetPort());
        	    togglePin(REGS5.GetPort());
        	    togglePin(REGS6.GetPort());
        	    togglePin(REGS7.GetPort());

        	    delay(1500);

        	    togglePin(REGS0.GetPort());
        	    togglePin(REGS1.GetPort());
        	    togglePin(REGS2.GetPort());
        	    togglePin(REGS3.GetPort());
        	    togglePin(REGS4.GetPort());
        	    togglePin(REGS5.GetPort());
        		togglePin(REGS6.GetPort());
        		togglePin(REGS7.GetPort());

        	    delay(500);
        	}else if(MEMO->CompararCadena("ACTIVAR",CMD)){
    			//if(REGS0.CheckNombre(ARG1)) REGS0.Start();
    			if(REGS1.CheckNombre(ARG1)){EnviarSMS(telefono,"Activada"); REGS1.Start();}
    			if(REGS2.CheckNombre(ARG1)) REGS2.Start();
    			if(REGS3.CheckNombre(ARG1)) REGS3.Start();
    			if(REGS4.CheckNombre(ARG1)) REGS4.Start();
    			if(REGS5.CheckNombre(ARG1)) REGS5.Start();
    			if(REGS6.CheckNombre(ARG1)) REGS6.Start();
    			if(REGS7.CheckNombre(ARG1)) REGS7.Start();
    			if(MEMO->CompararCadena("MANUAL",ARG1)){
        			if(REGS0.CheckNombre(ARG2)) REGS0.SetActivacionManual(true);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetActivacionManual(true);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetActivacionManual(true);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetActivacionManual(true);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetActivacionManual(true);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetActivacionManual(true);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetActivacionManual(true);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetActivacionManual(true);
    			}
        	}else if(MEMO->CompararCadena("DESACTIVAR",CMD)){
    			//if(REGS0.CheckNombre(ARG1)) REGS0.Stop();
    			if(REGS1.CheckNombre(ARG1)) REGS1.Stop();
    			if(REGS2.CheckNombre(ARG1)) REGS2.Stop();
    			if(REGS3.CheckNombre(ARG1)) REGS3.Stop();
    			if(REGS4.CheckNombre(ARG1)) REGS4.Stop();
    			if(REGS5.CheckNombre(ARG1)) REGS5.Stop();
    			if(REGS6.CheckNombre(ARG1)) REGS6.Stop();
    			if(REGS7.CheckNombre(ARG1)) REGS7.Stop();
    			if(MEMO->CompararCadena("MANUAL",ARG1)){
        			if(REGS0.CheckNombre(ARG2)) REGS0.SetActivacionManual(false);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetActivacionManual(false);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetActivacionManual(false);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetActivacionManual(false);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetActivacionManual(false);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetActivacionManual(false);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetActivacionManual(false);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetActivacionManual(false);
    			}
        	}else if(REGS1.CheckNombre(CMD)){
        		char buff[300];
        		REGS1.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(REGS2.CheckNombre(CMD)){
        		char buff[300];
        		REGS2.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(REGS3.CheckNombre(CMD)){
        		char buff[300];
        		REGS3.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(REGS4.CheckNombre(CMD)){
        		char buff[300];
        		REGS4.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(REGS5.CheckNombre(CMD)){
        		char buff[300];
        		REGS5.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(REGS6.CheckNombre(CMD)){
        		char buff[300];
        		REGS6.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(REGS7.CheckNombre(CMD)){
        		char buff[300];
        		REGS7.VerRegistro(buff);
        		EnviarSMS(telefono,buff);
        	}else if(MEMO->CompararCadena("SET",CMD)){     //Inicio comando SET
        		if(MEMO->CompararCadena("HORA",ARG1)){
        			char hora[3];
        			char minutos[3];
        			hora[0] = ARG2[0];
        			hora[1] = ARG2[1];
        			hora[2] = 0;
        			minutos[0] = ARG2[2];
        			minutos[1] = ARG2[3];
        			minutos[2] = 0;
        			SMS.SetHora(hora,minutos);
        		}

        		if(MEMO->CompararCadena("FECHA",ARG1)){
        			char ano[3];
        			char mes[3];
        			char dia[3];
        			dia[0] = ARG2[0];
        			dia[1] = ARG2[1];
        			dia[2] = 0;
        			mes[0] = ARG2[2];
        			mes[1] = ARG2[3];
        			mes[2] = 0;
        			ano[0] = ARG2[4];
        			ano[1] = ARG2[5];
        			ano[2] = 0;
        			SMS.SetFecha(dia,mes,ano);
        		}

        		if(MEMO->CompararCadena("NOMBRE",ARG1)){
        			//if(REGS0.CheckNombre(ARG2)) REGS0.SetNombre(ARG3);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetNombre(ARG3);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetNombre(ARG3);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetNombre(ARG3);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetNombre(ARG3);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetNombre(ARG3);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetNombre(ARG3);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetNombre(ARG3);
        			if(MEMO->CompararCadena(ARG2,ENT0.Nombre)){strcpy(ENT0.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        			if(MEMO->CompararCadena(ARG2,ENT1.Nombre)){strcpy(ENT1.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        			if(MEMO->CompararCadena(ARG2,ENT2.Nombre)){strcpy(ENT2.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        			if(MEMO->CompararCadena(ARG2,ENT3.Nombre)){strcpy(ENT3.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        			if(MEMO->CompararCadena(ARG2,ENT4.Nombre)){strcpy(ENT4.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        			if(MEMO->CompararCadena(ARG2,ENT5.Nombre)){strcpy(ENT5.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        			if(MEMO->CompararCadena(ARG2,ENT6.Nombre)){strcpy(ENT6.Nombre,ARG3); GuardarEntradas(); CrearMensajeEntradas();}
        		}
        		if(MEMO->CompararCadena("INTOFF",ARG1)){
        			if(REGS0.CheckNombre(ARG2)) REGS0.SetIntApagado(ARG3);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetIntApagado(ARG3);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetIntApagado(ARG3);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetIntApagado(ARG3);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetIntApagado(ARG3);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetIntApagado(ARG3);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetIntApagado(ARG3);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetIntApagado(ARG3);
        		}
        		if(MEMO->CompararCadena("INTON",ARG1)){
        			if(REGS0.CheckNombre(ARG2)) REGS0.SetIntEncendido(ARG3);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetIntEncendido(ARG3);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetIntEncendido(ARG3);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetIntEncendido(ARG3);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetIntEncendido(ARG3);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetIntEncendido(ARG3);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetIntEncendido(ARG3);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetIntEncendido(ARG3);
        		}
        		if(MEMO->CompararCadena("RANGOON",ARG1)){
        			if(REGS0.CheckNombre(ARG2)) REGS0.SetRangoOn(ARG3);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetRangoOn(ARG3);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetRangoOn(ARG3);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetRangoOn(ARG3);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetRangoOn(ARG3);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetRangoOn(ARG3);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetRangoOn(ARG3);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetRangoOn(ARG3);
        		}
        		if(MEMO->CompararCadena("RANGOOFF",ARG1)){
        			if(REGS0.CheckNombre(ARG2)) REGS0.SetRangoOff(ARG3);
        			if(REGS1.CheckNombre(ARG2)) REGS1.SetRangoOff(ARG3);
        			if(REGS2.CheckNombre(ARG2)) REGS2.SetRangoOff(ARG3);
        			if(REGS3.CheckNombre(ARG2)) REGS3.SetRangoOff(ARG3);
        			if(REGS4.CheckNombre(ARG2)) REGS4.SetRangoOff(ARG3);
        			if(REGS5.CheckNombre(ARG2)) REGS5.SetRangoOff(ARG3);
        			if(REGS6.CheckNombre(ARG2)) REGS6.SetRangoOff(ARG3);
        			if(REGS7.CheckNombre(ARG2)) REGS7.SetRangoOff(ARG3);
        		}
            	if(MEMO->CompararCadena("PASSWD",ARG1)){
            		SetPassword(ARG2);
            	}

        	} //Fin SET

        	if(MEMO->CompararCadena("TELEFONOS",CMD)){
        		char BUFFER[200];
        		sprintf(BUFFER,"Telefonos Habilitados:\n1: %s\n2: %s\n3: %s\n4: %s\n5: %s\n6: %s\n7: %s\n8: %s\n9: %s\n10: %s",TELEFONOS[0].tel,TELEFONOS[1].tel,TELEFONOS[2].tel,TELEFONOS[3].tel,TELEFONOS[4].tel,TELEFONOS[5].tel,TELEFONOS[6].tel,TELEFONOS[7].tel,TELEFONOS[8].tel,TELEFONOS[9].tel);
        		EnviarSMS(telefono,BUFFER);
        	}
        	if(MEMO->CompararCadena("ESTADO",CMD)){
        		char BUFFER[200];
        		sprintf(BUFFER,"Salidas:\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n", REGS0.Nombre, REGS0.isStarted(), REGS1.Nombre, REGS1.isStarted(), REGS2.Nombre, REGS2.isStarted(), REGS3.Nombre, REGS3.isStarted(), REGS4.Nombre, REGS4.isStarted(), REGS5.Nombre, REGS5.isStarted(), REGS6.Nombre, REGS6.isStarted(), REGS7.Nombre, REGS7.isStarted());
        		EnviarSMS(telefono,BUFFER);
        	}
        	//Fin de Telefonos Habilitados
        }else{
        	SerialUSB.println("El Telefono no se encuantra habilitado para operar con este terminal.");
        	//this->EnviarSMS(telefono,"El Telefono no se encuantra habilitado para operar con este terminal.");
        }
    }
    //delay(1000);
}

void SDR_LIB::InitGSM(){
	Serial2.flush();
	pinMode(GSM_ON,OUTPUT);
	pinMode(R_GSM,OUTPUT);
	digitalWrite(GSM_ON, LOW);
	digitalWrite(R_GSM, LOW);
	while(!gsm_local.begin(9600)){
		digitalWrite(GSM_ON, HIGH);
		delay(1200);
		digitalWrite(GSM_ON, LOW);
		delay(5000);
	}
}

void SDR_LIB::InitMEM(){
  MEMO->Init();
  if(MEMO->CheckFormat(MEM) == 0){
    FormatMem(MEM);
    InitMEM();
  }
}

void SDR_LIB::FormatMem(byte device){
	Senal(3,100);
	delay(500);
	Senal(3,100);
	delay(500);
	Senal(3,100);
	delay(500);

	MEMO->SetByte(0xAF,1);
	MEMO->SetByte(1,PASSWORD_DIR);
	MEMO->SetByte(0,TELEFONOS_DIR);
	MEMO->SetByte(0,REGISTRO0_DIR);
	MEMO->SetByte(0,REGISTRO1_DIR);
	MEMO->SetByte(0,REGISTRO2_DIR);
	MEMO->SetByte(0,REGISTRO3_DIR);
	MEMO->SetByte(0,REGISTRO4_DIR);
	MEMO->SetByte(0,REGISTRO5_DIR);
	MEMO->SetByte(0,REGISTRO6_DIR);
	MEMO->SetByte(0,REGISTRO7_DIR);
	MEMO->SetByte(0,ENTRADAS_DIR);

	REGS0.SetEprom(MEMO);
	REGS1.SetEprom(MEMO);
	REGS2.SetEprom(MEMO);
	REGS3.SetEprom(MEMO);
	REGS4.SetEprom(MEMO);
	REGS5.SetEprom(MEMO);
	REGS6.SetEprom(MEMO);
	REGS7.SetEprom(MEMO);

	this->REGS0.Init("SALIDA0",SALIDA_0,REGISTRO0_DIR);
	this->REGS0.GuardarRegistro();

	this->REGS1.Init("SALIDA1",SALIDA_1,REGISTRO1_DIR);
	this->REGS1.GuardarRegistro();

	this->REGS2.Init("SALIDA2",SALIDA_2,REGISTRO2_DIR);
	this->REGS2.GuardarRegistro();

	this->REGS3.Init("SALIDA3",SALIDA_3,REGISTRO3_DIR);
	this->REGS3.GuardarRegistro();

	this->REGS4.Init("SALIDA4",SALIDA_4,REGISTRO4_DIR);
	this->REGS4.GuardarRegistro();

	this->REGS5.Init("SALIDA5",SALIDA_5,REGISTRO5_DIR);
  	this->REGS5.GuardarRegistro();

  	this->REGS6.Init("SALIDA6",SALIDA_6,REGISTRO6_DIR);
  	this->REGS6.GuardarRegistro();

  	this->REGS7.Init("SALIDA7",SALIDA_7,REGISTRO7_DIR);
  	this->REGS7.GuardarRegistro();

  	strcpy(ENT0.Nombre,"ALERTA0");
  	strcpy(ENT1.Nombre,"ALERTA1");
  	strcpy(ENT2.Nombre,"ALERTA2");
  	strcpy(ENT3.Nombre,"ALERTA3");
  	strcpy(ENT4.Nombre,"ALERTA4");
  	strcpy(ENT5.Nombre,"ALERTA5");
  	strcpy(ENT6.Nombre,"ALERTA6");

  	ENT0.Port = ENTRADA_0;
  	ENT1.Port = ENTRADA_1;
  	ENT2.Port = ENTRADA_2;
  	ENT3.Port = ENTRADA_3;
  	ENT4.Port = ENTRADA_4;
  	ENT5.Port = ENTRADA_5;
  	ENT6.Port = ENTRADA_6;

  	GuardarEntradas();

  	const char *Password = "88888888";
  	SetPassword(Password);

  	CargarEntradas();
  	CargarTelefonos();
  	GetPassword();

  	Senal(3,100);
  	delay(250);
  	Senal(3,100);
  	delay(250);
  	Senal(3,100);
  	delay(250);
}

boolean SDR_LIB::BuscarTelefono(char *tel){
    //return MEMO->CompararCadenaEPROMM(tel,TELEFONOS_DIR);
	byte i = 0;
	boolean timeout = false;
	SerialUSB.print("Telefono a buscar: ");
	SerialUSB.println(tel);
	while(!MEMO->CompararCadena(tel,TELEFONOS[i].tel) && !timeout){
		SerialUSB.print("{");
		SerialUSB.print(i);
		SerialUSB.print("}");
		SerialUSB.print(" Con: ");
		SerialUSB.println(TELEFONOS[i].tel);
		if(i == 10) timeout = true; else i++;
	}
	SerialUSB.println("Fin de Busqueda.");
	if(timeout) return false; else return true;
}

boolean SDR_LIB::CheckPassword(char* pass){
    if(pass[8] == '\0') return MEMO->CompararCadena(pass,PASSWORD); else return false;
}

void SDR_LIB::SetPassword(const char *pass){
  MEMO->UpdateCadenaEPROMM(pass,0,PASSWORD_DIR);
  GetPassword();
}

void SDR_LIB::SetPassword(char *pass){
  MEMO->UpdateCadenaEPROMM(pass,0,PASSWORD_DIR);
  GetPassword();
}

void SDR_LIB::GetPassword(){
	MEMO->GetCadena(PASSWORD,1,PASSWORD_DIR);
}

boolean SDR_LIB::Check_SMS(char *tel, char *msj){
    tel[0] = '\0';
    msj[0] = '\0';
	Serial2.flush();
    char nmes = SMS.IsSMSPresent(SMS_ALL);
    if(nmes != 0)
    {
    	SerialUSB.println("Mensaje...");
    	SMS.GetSMS(nmes, tel, msj, 100);
        Senal(5,200);
        SMS.DeleteSMS(nmes);
        //#ifdef DEBUG
        	SerialUSB.print("Telefono: ");
            SerialUSB.println(tel);
            SerialUSB.print("Mensaje: ");
            SerialUSB.println(msj);
        //#endif // DEBUG
        Upper(msj,100);
        if(tel[0] == 0){
        	Senal(10,100);
        	ReiniciarGSM();
        	delay(5000);
        	Senal(10,150);
        }else{
        	Senal(2,150);
        }
        return true;
    }else return false;
}

void SDR_LIB::Upper(char *texto, uint8 len){
    byte i = 0;
    while(texto[i] != 0 && i != len){
        if(texto[i] >= 97) texto[i] = texto[i] - 32;
        i++;
    }
}

void SDR_LIB::GetCmd(char *sms, char *cmd, char *arg1, char *arg2, char *arg3){

	toggleLED();
	byte i ,f;

    cmd[0] = '\0';
    arg1[0] = '\0';
    arg2[0] = '\0';
    arg3[0] = '\0';

    i = 0;
    f = 0;

    while(sms[i] != 0 && sms[i] != 32){
        cmd[f] = sms[i];
        #ifdef DEBUG
            ////SerialUSB.print("CMD: ");
            ////SerialUSB.print(cmd[f]);
            ////SerialUSB.print(" | ");
            ////SerialUSB.print(cmd[f],DEC);
            ////SerialUSB.print(" SMS: ");
            ////SerialUSB.print(sms[i]);
            ////SerialUSB.print(" | ");
            ////SerialUSB.println(sms[i],DEC);
        #endif
        f++;
        i++;
    }
    cmd[f] = 0;
    if(sms[i] == 32){
        f=0;
        i++;
        while(sms[i] != 0 && sms[i] != 32){
            arg1[f] = sms[i];
            #ifdef DEBUG
                ////SerialUSB.print("arg1: ");
                ////SerialUSB.print(arg1[f]);
                ////SerialUSB.print(" | ");
                ////SerialUSB.print(arg1[f],DEC);
                ////SerialUSB.print(" SMS: ");
                ////SerialUSB.print(sms[i]);
                ////SerialUSB.print(" | ");
                ////SerialUSB.println(sms[i],DEC);
            #endif
            f++;
            i++;
        }
        arg1[f] = 0;
        if(sms[i] == 32){
            f=0;
            i++;
            while(sms[i] != 0 && sms[i] != 32){
                arg2[f] = sms[i];
                #ifdef DEBUG
                    ////SerialUSB.print("arg2: ");
                    ////SerialUSB.print(arg2[f]);
                    ////SerialUSB.print(" | ");
                    ////SerialUSB.print(arg2[f],DEC);
                    ////SerialUSB.print(" SMS: ");
                    ////SerialUSB.print(sms[i]);
                    ////SerialUSB.print(" | ");
                    ////SerialUSB.println(sms[i],DEC);
                #endif
                f++;
                i++;
            }
            arg2[f] = 0;
            if(sms[i] == 32){
                f=0;
                i++;
                while(sms[i] != 0 && sms[i] != 32){
                    arg3[f] = sms[i];
                    #ifdef DEBUG
                        ////SerialUSB.print("arg3: ");
                        ////SerialUSB.print(arg3[f]);
                        ////SerialUSB.print(" | ");
                        ////SerialUSB.print(arg3[f],DEC);
                        ////SerialUSB.print(" SMS: ");
                        ////SerialUSB.print(sms[i]);
                        ////SerialUSB.print(" | ");
                        ////SerialUSB.println(sms[i],DEC);
                    #endif
                    f++;
                    i++;
                }
                arg3[f] = 0;
            }
        }
    }
    #ifdef DEBUG

        ////SerialUSB.print("Comando: ");
        ////SerialUSB.print(cmd);
        ////SerialUSB.println(".");

        ////SerialUSB.print("Argumento 1: ");
        ////SerialUSB.print(arg1);
        ////SerialUSB.println(".");

        ////SerialUSB.print("Argumento 2: ");
        ////SerialUSB.print(arg2);
        ////SerialUSB.println(".");

        ////SerialUSB.print("Argumento 3: ");
        ////SerialUSB.print(arg3);
        ////SerialUSB.println(".");

    #endif // DEBUG
    toggleLED();
}

void SDR_LIB::EnviarSMS(char *num,char *txt){
	SerialUSB.print("Mensaje Enviado: ");
	SerialUSB.print(txt);
	SerialUSB.print(" a ");
	SerialUSB.println(num);
    SMS.SendSMS(num,txt);
}

void SDR_LIB::EnviarSMS(char *num,const char *txt){
	SerialUSB.print("Mensaje Enviado: ");
	SerialUSB.print(txt);
	SerialUSB.print(" a ");
	SerialUSB.println(num);
	SMS.SendSMS(num,(char *)txt);
}

void SDR_LIB::VerPos(char *var){
    char buf[30];
    sprintf(buf,"VAR: %p",&var[0]);
    ////SerialUSB.println(buf);
}

void SDR_LIB::CargarTelefonos(){
	byte i = 0;
	for(i = 0; i<10; i++){
		MEMO->GetCadena(TELEFONOS[i].tel,i+1,TELEFONOS_DIR);
		SerialUSB.print("Telefono ");
		SerialUSB.print(i);
		SerialUSB.print(": ");
		SerialUSB.println(TELEFONOS[i].tel);
	}
}

void SDR_LIB::MostrarRegistro(REGISTRO reg){

}

void SDR_LIB::VerRegistros(){
	REGS0.VerRegistro();
	REGS1.VerRegistro();
	REGS2.VerRegistro();
	REGS3.VerRegistro();
	REGS4.VerRegistro();
	REGS5.VerRegistro();
	REGS6.VerRegistro();
	REGS7.VerRegistro();

}


void SDR_LIB::Senal(uint8 bips, uint16 tiempo){
	uint8 i = 0;
	for(i = 0; i<bips; i++){
		toggleLED();
		delay(tiempo);
		toggleLED();
		delay(tiempo);
	}
}

void SDR_LIB::CheckEntradas(void){
	if(digitalRead(ENT0.Port) && !E0){E0 = true; EnviarATodos(ENT0.Mensaje);}
	else if(!digitalRead(ENT0.Port) && E0) E0 = false;

	if(digitalRead(ENT1.Port) && !E1){E1 = true; EnviarATodos(ENT1.Mensaje);}
	else if(!digitalRead(ENT1.Port) && E1) E1 = false;

	if(digitalRead(ENT2.Port) && !E2){E2 = true; EnviarATodos(ENT2.Mensaje);}
	else if(!digitalRead(ENT1.Port) && E2) E2 = false;

	if(digitalRead(ENT3.Port) && !E3){E3 = true; EnviarATodos(ENT3.Mensaje);}
	else if(!digitalRead(ENT3.Port) && E3) E3 = false;

	if(digitalRead(ENT4.Port) && !E4){E4 = true; EnviarATodos(ENT4.Mensaje);}
	else if(!digitalRead(ENT4.Port) && E4) E4 = false;

	if(digitalRead(ENT5.Port) && !E5){E5 = true; EnviarATodos(ENT5.Mensaje);}
	else if(!digitalRead(ENT5.Port) && E5) E5 = false;

	if(digitalRead(ENT6.Port) && !E6){E6 = true; EnviarATodos(ENT6.Mensaje);}
	else if(!digitalRead(ENT6.Port) && E6) E6 = false;
}

void SDR_LIB::CheckSalidas(){

	if(REGS1.GetActivacionManual()){
		digitalWrite(REGS1.GetPort(), HIGH);
	}else{
		if(REGS1.isStarted()){
			REGS1.CheckVentana(arrayHora, arrayFecha);
			REGS1.Checktiempo(arrayHora, arrayFecha);
			if(REGS1.isActive()) digitalWrite(REGS1.GetPort(), HIGH); else digitalWrite(REGS1.GetPort(), LOW);
		}else digitalWrite(REGS1.GetPort(), LOW);
	}

	if(REGS2.GetActivacionManual()){
		digitalWrite(REGS2.GetPort(), HIGH);
	}else{
		if(REGS2.isStarted()){
			REGS2.CheckVentana(arrayHora, arrayFecha);
			REGS2.Checktiempo(arrayHora, arrayFecha);
			if(REGS2.isActive()) digitalWrite(REGS2.GetPort(), HIGH); else digitalWrite(REGS2.GetPort(), LOW);
		}else digitalWrite(REGS2.GetPort(), LOW);
	}

	if(REGS3.GetActivacionManual()){
		digitalWrite(REGS3.GetPort(), HIGH);
	}else{
		if(REGS3.isStarted()){
			REGS3.CheckVentana(arrayHora, arrayFecha);
			REGS3.Checktiempo(arrayHora, arrayFecha);
			if(REGS3.isActive()) digitalWrite(REGS3.GetPort(), HIGH); else digitalWrite(REGS3.GetPort(), LOW);
		}else digitalWrite(REGS3.GetPort(), LOW);
	}

	if(REGS4.GetActivacionManual()){
		digitalWrite(REGS4.GetPort(), HIGH);
	}else{
		if(REGS4.isStarted()){
			REGS4.CheckVentana(arrayHora, arrayFecha);
			REGS4.Checktiempo(arrayHora, arrayFecha);
			if(REGS4.isActive()) digitalWrite(REGS4.GetPort(), HIGH); else digitalWrite(REGS4.GetPort(), LOW);
		}else digitalWrite(REGS4.GetPort(), LOW);
	}

	if(REGS5.GetActivacionManual()){
		digitalWrite(REGS5.GetPort(), HIGH);
	}else{
		if(REGS5.isStarted()){
			REGS5.CheckVentana(arrayHora, arrayFecha);
			REGS5.Checktiempo(arrayHora, arrayFecha);
			if(REGS5.isActive()) digitalWrite(REGS5.GetPort(), HIGH); else digitalWrite(REGS5.GetPort(), LOW);
		}else digitalWrite(REGS5.GetPort(), LOW);
	}

	if(REGS6.GetActivacionManual()){
		digitalWrite(REGS6.GetPort(), HIGH);
	}else{
		if(REGS6.isStarted()){
			REGS6.CheckVentana(arrayHora, arrayFecha);
			REGS6.Checktiempo(arrayHora, arrayFecha);
			if(REGS6.isActive()) digitalWrite(REGS6.GetPort(), HIGH); else digitalWrite(REGS6.GetPort(), LOW);
		}else digitalWrite(REGS6.GetPort(), LOW);
	}

	if(REGS7.GetActivacionManual()){
		digitalWrite(REGS7.GetPort(), HIGH);
	}else{
		if(REGS7.isStarted()){
			REGS7.CheckVentana(arrayHora, arrayFecha);
			REGS7.Checktiempo(arrayHora, arrayFecha);
			if(REGS7.isActive()) digitalWrite(REGS7.GetPort(), HIGH); else digitalWrite(REGS7.GetPort(), LOW);
		}else digitalWrite(REGS7.GetPort(), LOW);
	}

	if(digitalRead(REGS1.GetPort()) || digitalRead(REGS2.GetPort())|| digitalRead(REGS3.GetPort())|| digitalRead(REGS4.GetPort())|| digitalRead(REGS5.GetPort())|| digitalRead(REGS6.GetPort())|| digitalRead(REGS7.GetPort())) digitalWrite(REGS0.GetPort(),HIGH); else digitalWrite(REGS0.GetPort(), LOW);
}

void SDR_LIB::VerEstado(){
	SerialUSB.print("Reg0: ");SerialUSB.println(REGS0.isStarted());
	SerialUSB.print("Reg1: ");SerialUSB.println(REGS1.isStarted());
	SerialUSB.print("Reg2: ");SerialUSB.println(REGS2.isStarted());
	SerialUSB.print("Reg3: ");SerialUSB.println(REGS3.isStarted());
	SerialUSB.print("Reg4: ");SerialUSB.println(REGS4.isStarted());
	SerialUSB.print("Reg5: ");SerialUSB.println(REGS5.isStarted());
	SerialUSB.print("Reg6: ");SerialUSB.println(REGS6.isStarted());
	SerialUSB.print("Reg7: ");SerialUSB.println(REGS7.isStarted());
}

void SDR_LIB::EnviarATodos(char *txt){
	byte i = 0;
	while(TELEFONOS[i].tel[0] != 0){
		EnviarSMS(TELEFONOS[i].tel,txt);
		i++;
	}
}

void SDR_LIB::EnviarATodos(const char *txt){
	byte i = 0;
	while(TELEFONOS[i].tel[0] != 0){
		EnviarSMS(TELEFONOS[i].tel,txt);
		i++;
	}
}

void SDR_LIB::CargarRegistros(){
	REGS0.CargarRegistro();
	REGS1.CargarRegistro();
	REGS2.CargarRegistro();
	REGS3.CargarRegistro();
	REGS4.CargarRegistro();
	REGS5.CargarRegistro();
	REGS6.CargarRegistro();
	REGS7.CargarRegistro();
}

void SDR_LIB::CargarEntradas(){
	char buff[10];
	MEMO->GetCadena(ENT0.Nombre,1,ENTRADAS_DIR);
	MEMO->GetCadena(buff,2,ENTRADAS_DIR);
	ENT0.Port = REGS0.StoI(buff);

	MEMO->GetCadena(ENT1.Nombre,3,ENTRADAS_DIR);
	MEMO->GetCadena(buff,4,ENTRADAS_DIR);
	ENT1.Port = REGS0.StoI(buff);

	MEMO->GetCadena(ENT2.Nombre,5,ENTRADAS_DIR);
	MEMO->GetCadena(buff,6,ENTRADAS_DIR);
	ENT2.Port = REGS0.StoI(buff);

	MEMO->GetCadena(ENT3.Nombre,7,ENTRADAS_DIR);
	MEMO->GetCadena(buff,8,ENTRADAS_DIR);
	ENT3.Port = REGS0.StoI(buff);

	MEMO->GetCadena(ENT4.Nombre,9,ENTRADAS_DIR);
	MEMO->GetCadena(buff,10,ENTRADAS_DIR);
	ENT4.Port = REGS0.StoI(buff);

	MEMO->GetCadena(ENT5.Nombre,11,ENTRADAS_DIR);
	MEMO->GetCadena(buff,12,ENTRADAS_DIR);
	ENT5.Port = REGS0.StoI(buff);

	MEMO->GetCadena(ENT6.Nombre,13,ENTRADAS_DIR);
	MEMO->GetCadena(buff,14,ENTRADAS_DIR);
	ENT6.Port = REGS0.StoI(buff);

	this->CrearMensajeEntradas();
}

void SDR_LIB::GuardarEntradas(){
	char buff[10];
	MEMO->SetByte(0x00,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT0.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT0.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT1.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT1.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT2.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT2.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT3.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT3.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT4.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT4.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT5.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT5.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);

	MEMO->AddCadenaEPROMM(ENT6.Nombre,ENTRADAS_DIR);
	REGS0.ItoS(buff,ENT6.Port);
	MEMO->AddCadenaEPROMM(buff,ENTRADAS_DIR);
}

void SDR_LIB::CrearMensajeEntradas(){
	sprintf(ENT0.Mensaje,"%s Activada.",ENT0.Nombre);
	sprintf(ENT1.Mensaje,"%s Activada.",ENT1.Nombre);
	sprintf(ENT2.Mensaje,"%s Activada.",ENT2.Nombre);
	sprintf(ENT3.Mensaje,"%s Activada.",ENT3.Nombre);
	sprintf(ENT4.Mensaje,"%s Activada.",ENT4.Nombre);
	sprintf(ENT5.Mensaje,"%s Activada.",ENT5.Nombre);
	sprintf(ENT6.Mensaje,"%s Activada.",ENT6.Nombre);
}

void SDR_LIB::ReiniciarGSM(){
	digitalWrite(R_GSM, HIGH);
	delay(2000);
	digitalWrite(R_GSM, LOW);
	delay(2000);
}

void SDR_LIB::CheckMaster(){

}
