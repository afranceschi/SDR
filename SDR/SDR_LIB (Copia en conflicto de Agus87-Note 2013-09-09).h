#ifndef _SDR_LIB_H
#define _SDR_LIB_H

#include "Eprom.h"
#include "GSMSHIELD/sim900.h"
#include "GSMSHIELD/sms.h"
#include "GSMSHIELD/GSM.h"
#include "Registro.h"
#include <stdio.h>
#include <string.h>

#define SALIDA_0 23
#define SALIDA_1 25
#define SALIDA_2 27
#define SALIDA_3 29
#define SALIDA_4 31
#define SALIDA_5 33
#define SALIDA_6 35
#define SALIDA_7 37

#define ENTRADA_0 24
#define ENTRADA_1 26
#define ENTRADA_2 28
#define ENTRADA_3 30
#define ENTRADA_4 32
#define ENTRADA_5 34
#define ENTRADA_6 36

#define PASSWORD_DIR 0x10
#define TELEFONOS_DIR 0x30

#define REGISTRO0_DIR 0x100
#define REGISTRO1_DIR 0x200
#define REGISTRO2_DIR 0x300
#define REGISTRO3_DIR 0x400
#define REGISTRO4_DIR 0x500
#define REGISTRO5_DIR 0x600
#define REGISTRO6_DIR 0x700
#define REGISTRO7_DIR 0x800

#define ENTRADAS_DIR 0xA00

#define RESET_GSM_PIN 9


//#define DEBUG
//#define FORMAT

struct TELEFONO{
	char tel[20];
};

struct ENTRADA{
	char Nombre[20];
	uint8 Port;
	char Mensaje[30];
};

class SDR_LIB{

    public:
        void Init();
        void Main();
        void InitGSM();
        void InitMEM();
        void CheckEntradas();

    private:
        GSM gsm_local;
        SMSGSM SMS;
        REGISTRO REGS0;
        REGISTRO REGS1;
        REGISTRO REGS2;
        REGISTRO REGS3;
        REGISTRO REGS4;
        REGISTRO REGS5;
        REGISTRO REGS6;
        REGISTRO REGS7;

        ENTRADA ENT0;
        ENTRADA ENT1;
        ENTRADA ENT2;
        ENTRADA ENT3;
        ENTRADA ENT4;
        ENTRADA ENT5;
        ENTRADA ENT6;

        boolean E0;
        boolean E1;
        boolean E2;
        boolean E3;
        boolean E4;
        boolean E5;
        boolean E6;
        TELEFONO TELEFONOS[10];
        uint32 arrayHora;
        uint32 arrayFecha;
        char mensaje[100];
        char telefono[20];
        char CMD[10];
        char ARG1[10];
        char ARG2[10];
        char ARG3[10];
        char HORA[3];
        char MINUTOS[3];
        char SEGUNDOS[3];
        char DIA[3];
        char MES[3];
        char ANO[3];
        char PASSWORD[10];
        Eprom_Lib *MEMO;

        uint16 diasumar;

        void FormatMem(byte device);
        void SetPassword(const char *pass);
        void SetPassword(char *pass);
        boolean Check_SMS(char *tel, char *msj);
        void EnviarSMS(char *num,char *txt);
        void EnviarSMS(char *num,const char *txt);
        void Upper(char *texto, uint8 len);
        boolean BuscarTelefono(char *tel);
        void CargarTelefonos();
        void GetPassword();
        void GuardarRegistro(REGISTRO *reg, unsigned int dir);
        void GuardarRegistroNuevo(REGISTRO reg, unsigned int dir);
        void CargarRegistro(REGISTRO reg, unsigned int dir);
        void MostrarRegistro(REGISTRO reg);
        void VerRegistros();
        void CheckSalidas();
        void VerEstado();

        boolean CheckPassword(char* pass);


        void GetCmd(char *sms, char *cmd, char *arg1, char *arg2, char *arg3);
        void VerPos(char *var);
        void Senal(uint8 bips, uint16 tiempo);

        void EnviarATodos(char *txt);
        void EnviarATodos(const char *txt);

        void CargarRegistros();
        void CargarEntradas();

        void GuardarEntradas();

        void CrearMensajeEntradas();

        void ReiniciarGSM();

        void CheckMaster();
};

#endif
