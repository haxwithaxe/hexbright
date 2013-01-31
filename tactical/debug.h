#ifndef Debug_h
#define Debug_h

#include <HardwareSerial.h>
class Debug {
	HardwareSerial& out;
	public:
		Debug(HardwareSerial& serial_out): out(serial_out)
		{
			out.println("debug enabled");
		};
		void print(const char* msg) {
#ifdef DEBUG
			out.println(msg);
#else
			((void)0); //noop
#endif
		};
};

#endif
