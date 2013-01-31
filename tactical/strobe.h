#ifndef HEXBRIGHT_Strobe_h
#define HEXBRIGHT_Strobe_h
#include "debug.h"

class Strobe {
	hexbright& hbo;
	Debug& debug;

	private:
		void _flash(unsigned long lvl);
		void _set_on(unsigned long start_lvl, unsigned long end_lvl, unsigned long period);
		void _set_off(unsigned long lvl, unsigned long period);
		void on(unsigned long lvl);
		void off();

	public:
		Strobe(hexbright& hbor, Debug& debugo);
		void pulse(unsigned long lvl, unsigned long interval);
		void strobe(unsigned int interval, unsigned long* flash_time);

};
#endif
