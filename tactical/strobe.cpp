#include <hexbright.h>
#include "strobe.h"

Strobe::Strobe(){};

Strobe::Strobe(hexbright& hbor) {
	hbo = hbor;
}
Strobe::Strobe(hexbright& hbor, Debug& debugo){
	debug = debugo;
	hbo = hbor;
}

void Strobe::_pulse(unsigned long lvl, unsigned long interval) {
	debug.print("strobe pulse");
	hbo.set_light(CURRENT_LEVEL, lvl, 0);
	delay(interval);
	hbo.set_light(CURRENT_LEVEL, 0, 20);
}

void Strobe::_set_strobe(unsigned int interval) {
	debug.print("strobe strobe");
	unsigned long flash_time = millis();
	if(flash_time+interval<millis()) { // flash every <interval> milliseconds
		flash_time = millis(); // reset flash_time
		hbo.set_light(MAX_LEVEL, 0, 20); // and pulse (going from max to min over 20 milliseconds)
		/* note: because of the refresh rate, it's more like 'go from max brightness on high
		 * to max brightness on low to off.
		 */
	}
}

void Strobe::set_strobe(unsigned int interval) { _set_strobe(interval); }
void Strobe::pulse(unsigned long lvl, unsigned long interval) { _pulse(lvl, interval); }
