#include <hexbright.h>
#include "strobe.h"

Strobe::Strobe(hexbright& hbor, Debug& debugo):
	debug(debugo),
	hbo(hbor)
{
	((void)0); //noop
};

void Strobe::_set_on(unsigned long start_lvl, unsigned long end_lvl, unsigned long period) {
	hbo.set_light(start_lvl, end_lvl, period);
}
void Strobe::on(unsigned long lvl) { _set_on(CURRENT_LEVEL, lvl, 20); };

void Strobe::_set_off(unsigned long lvl, unsigned long period) {
	hbo.set_light(lvl, 0, period);
}
void Strobe::off() { _set_off(CURRENT_LEVEL, 20); };

void Strobe::pulse(unsigned long lvl, unsigned long interval) {
	debug.print("strobe pulse");
	on(lvl);
	delay(interval);
	off();
};

void Strobe::_flash(unsigned long lvl) {
	hbo.set_light(lvl, 0, 20); // and pulse (going from max to min over 20 milliseconds)
	/* note: because of the refresh rate, it's more like 'go from max brightness on high
	 * to max brightness on low to off.
	 */
}

void Strobe::strobe(unsigned int interval, unsigned long* flash_time) {
	debug.print("strobe strobe");
	if(*flash_time+interval<millis()) { // flash every <interval> milliseconds
		debug.print("flash");
		*flash_time = millis(); // reset flash_time
		_flash(MAX_LEVEL);
	};
};
