#include <hexbright.h>
hexbright hb;

static void strobe::_pulse(unsigned long lvl, unsigned long interval) {
    hb.set_light_level(lvl);
	delay(interval);
	hb.set_light_level(0);
}
static void strobe::_set_strobe(unsigned int interval) {
  static unsigned long flash_time = millis();
  if(flash_time+interval<millis()) { // flash every <interval> milliseconds
    flash_time = millis(); // reset flash_time
    hb.set_light(MAX_LEVEL, 0, 20); // and pulse (going from max to min over 20 milliseconds)
    /* note: because of the refresh rate, it's more like 'go from max brightness on high
     * to max brightness on low to off.
     */
  }
}
