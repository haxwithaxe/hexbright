/*
Copyright (c) 2013, haxwithaxe <me@haxwithaxe.net>
All rights reserved.
Licensed under GPLv3 (see LICENSE file in repository root)
Modified from an example by "David Hilton" <dhiltonp@gmail.com>
*/

#include <hexbright.h>
#include <Wire.h>
#include "debug.h"

Debug debug(Serial);

hexbright hb;

#include "strobe.h"
Strobe strobe(hb, debug);

#include "morse.h"
Morse morse(strobe);

#include "tap.h"
Tap tap_3(hb, 3, 5, &morse.sos);

#define HOLD_TIME 250 // milliseconds before going to strobe

#define BRIGHTNESS_COUNT 4
int brightness[BRIGHTNESS_COUNT] = {1000, 600, 300, 0};
int current_brightness = BRIGHTNESS_COUNT-1; // start on the last mode (off)

#define RESUME_FROM_STROBE
//15Hz=66.6...ms
#define STROBE_INTERVAL 67

void setup() {
	hb.init_hardware();
#ifdef DEBUG
	Serial.begin(9600);
#endif
}

void loop() {
	hb.update();

	if(hb.button_just_released() && hb.button_pressed_time()<HOLD_TIME) { 
		// if held for less than 200 ms before release, change regular modes
		debug.print("click");
		current_brightness = (current_brightness+1)%BRIGHTNESS_COUNT;
		set_light();
	} else if (hb.button_pressed_time()>HOLD_TIME) {
		if(hb.button_pressed()) {
			debug.print("holding");
			// held for over HOLD_TIME ms, go to strobe
			static unsigned long flash_time = millis();
			strobe.strobe(STROBE_INTERVAL, &flash_time);
		} else { // we have been doing strobe, but the light is now released
#ifdef RESUME_FROM_STROBE
			// after strobe, go to previous light level:
			set_light();
#else
			// after strobe, shutdown
			current_brightness = 0;
			hb.shutdown();
#endif
		}
	}
	tap_3.upkeep();
	hb.print_charge(GLED);
}

void set_light() {
	if(brightness[current_brightness] == 0) {
		hb.shutdown();
	} else {
		hb.set_light(CURRENT_LEVEL, brightness[current_brightness], 50);
	}
}
