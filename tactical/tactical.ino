/*
Copyright (c) 2013, "David Hilton" <dhiltonp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
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
