#ifndef GSM_H_
#define GSM_H_

#include <wirish.h>

#define comm_buff_len 1024

#define PWR_GSM_PIN 7

class GSM{

	public:
		void begin();
		boolean CheckSMS();

	private:
		uint8 comm_buff[comm_buff_len];
		void SendCMD(const char *cmd);
		void SendCMD(char *cmd);
		boolean esperarOK();
		boolean Read();
		void PowerSignal();
};

#endif
