#ifndef _REGISTROS_H_
#define _REGISTROS_H_

#include <wirish/wirish.h>
#include <string.h>
#include <stdlib.h>
#include "Eprom.h"

#define POS_CONFIGURACION 1
#define POS_PUERTO 2
#define POS_NOMBRE 3
#define POS_ACT_SALIDA_HIGH 4
#define POS_ACT_SALIDA_LOW 5
#define POS_DEC_SALIDA_HIGH 6
#define POS_DEC_SALIDA_LOW 7
#define POS_ACT_VENTANA_HIGH 8
#define POS_ACT_VENTANA_LOW 9
#define POS_DEC_VENTANA_HIGH 10
#define POS_DEC_VENTANA_LOW 11
#define POS_INTERVALO_APAGADO 12
#define POS_INTERVALO_PRENDIDO 13
#define POS_HORA_IN_VENTANA 14
#define POS_HORA_AP_VENTANA 15


class REGISTRO{

	private:
		uint8 Configuracion;
		uint8 Puerto;
		uint32 Act_Ventana_Low;
		uint32 Act_Ventana_High;
		uint32 Dec_Ventana_Low;
		uint32 Dec_Ventana_High;
		uint32 Act_Salida_Low;
		uint32 Act_Salida_High;
		uint32 Dec_Salida_Low;
		uint32 Dec_Salida_High;
		uint32 Intervalo_Apagado;
		uint32 Intervalo_Prendido;
		uint32 Hora_IN_Ventana;
		uint32 Hora_AP_Ventana;
		Eprom_Lib *EP;
		boolean Activo_flag;
		boolean Started;
		boolean ACT_MAN;
		unsigned int DIR;
		boolean esBisiesto(uint32 ano);
		boolean SalidaOn;

	public:
		char Nombre[10];

		uint32 SumarUnDia(uint32 fecha);
		uint32 SumarDias(uint8 dias, uint32 fecha);

		void Init(char *nom, uint8 port, unsigned int dir);
		void Init(const char *nom, uint8 port, unsigned int dir);

		void Activar();
		void Desactivar();
		void SetNombre(char* nom);
		void SetNombre(const char* nom);
		void SetIntApagado(char* intervalo);
		void SetIntEncendido(char* intervalo);
		void SetRangoOn(char* hora);
		void SetRangoOff(char* hora);
		void SetPort(uint8 port);
		void SetDir(unsigned int dir);
		void SetEprom(Eprom_Lib *ep);

		void Start();
		void Stop();
		boolean isStarted();
		boolean isActive();

		uint8 GetPort();

		bool CheckNombre(char* nom);
		bool CheckNombre(const char* nom);
		void CheckVentana(uint32 hora, uint32 fecha);

		uint32 SumarHora(uint32 hora, uint32 cantidad);

		void GuardarRegistro();
		void CargarRegistro();

		void VerRegistro();
		void VerRegistro(char *buff);

		void Checktiempo(uint32 hora, uint32 fecha);

		void ItoS(char *str, uint32 num);
		uint32 StoI(char *txt);
		uint32 StoI(const char *txt);

		void SetActivacionManual(boolean act);
		boolean GetActivacionManual();

		void Concatenar(char *a, char *b);
		void Concatenar(char *a, const char *b);
};
#endif
