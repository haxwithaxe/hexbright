#ifndef HEXBRIGHT_Strobe_h
#define HEXBRIGHT_Strobe_h

class Strobe {
	hexbright& hbo;
	Debug& debug;
	private:
		void _pulse(unsigned long lvl, unsigned long interval);
		void _set_strobe(unsigned int interval);					
	public:
		Strobe();
		Strobe(hexbright& hbor);
		Strobe(hexbright& hbor, Debug& debugo);
		void pulse(unsigned long lvl, unsigned long interval);
		void set_strobe(unsigned int interval);
};
#endif
