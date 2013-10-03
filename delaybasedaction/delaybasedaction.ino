/*
Copyright (C) 2013 haxwithaxe
Rewrite of functional.ino by "David Hilton" <dhiltonp@gmail.com>
License: FreeBSD (pending research on compatibility with MIT)
<insert license text here>
*/

#include <hexbright.h>

#include <Wire.h>

// Light is off
#define OFF_MODE 0

// Light blinks at a regular interval
// NOT IMPLEMENTED
#define BLINKY_MODE 20

// Light cycles between brightnesses per button click
#define CYCLE_MODE 30

// Light "strobes" at random intervals and random intensities within a certain range
// NOT IMPLEMENTED
#define DAZZLE_MODE 90

//Maximum light level value (probably should use the one in hb)
#define LEVEL_MAX 1000

// Minimum light level value for CYCLE_MODE
#define LEVEL_MIN 150

#define LEVELS_LEN 4
#define MIN_PRESSED_MS 100
#define PRESSED_MS 600
#define HELD_MS 700
#define TRANSITION_MS 0
#define MODE_TIMEOUT 2000

// Light levels for CYCLE_MODE
#define LEVELS_LEN 4

int levels[LEVELS_LEN] = {
	LEVEL_MIN,
	(LEVEL_MAX-LEVEL_MIN)/LEVELS_LEN, /* 1/LEVELS_LEN power */
	(LEVEL_MAX-LEVEL_MIN)/(LEVELS_LEN/2), /* 2*previous power */
	LEVEL_MAX /* full power */
};

int mode = 0;
int brightness_index = 0;
int pressed = 0;

hexbright hb;

void setup() {
  hb.init_hardware();
} 

void loop() {
	// update hexbright lib state awareness
	hb.update();

	/* State based actions */
	// Button State
	if(hb.button_just_released()) { //If button pressed and released
		/* CYCLE_MODE
		 * If button held for less than PRESSED_MS but more than MIN_PRESSED_MS enter cycle mode and cycle through them
		 *	the bottom cut off *helps* prevent accidentally turning on the light in a pocket, etc.
		 *	the MIN_PRESSED_MS and PRESSED_MS likely need tuning per individual.
		 */
		if(hb.button_pressed_time()>MIN_PRESSED_MS && hb.button_pressed_time()<PRESSED_MS) {
			if (brightness_index >= LEVELS_LEN-1) { // hit max index, reset to `off` state
				set_off();
			} else if (mode == CYCLE_MODE && millis() - pressed > MODE_TIMEOUT) {
				set_off();
			} else {
				pressed = millis();
				mode = CYCLE_MODE;
				set_light_level(levels[brightness_index]);
				brightness_index++;
			}
		}
	}

	/* OFF_MODE
	 * If held for over HELD_MS milliseconds (whether or not it's been released), then turn off the light.
	 */
	if(hb.button_pressed_time()>HELD_MS) {
		set_off();
	}

	// Do stuff that happens every cycle.
	upkeep();
}

void upkeep() {
  switch (mode){
	case CYCLE_MODE:
		break;
	case OFF_MODE:
		// Indicate charging with the tail light if battery is charging or charged.
	    if(!hb.printing_number()) hb.print_charge(GLED);
		break;
  }
}

void set_light_level(int level) {
	hb.set_light(CURRENT_LEVEL, level, TRANSITION_MS);
}

void reset_brightness() {
	/* Reset brightness_index even if mode != CYCLE_MODE.
	 * If not then CYCLE_MODE would possibly begin at a different level than user expects.
	 */
	brightness_index = 0;
}

void set_off() {
	pressed = 0;
	mode = OFF_MODE;
	set_light_level(OFF_LEVEL);
	reset_brightness();
}

