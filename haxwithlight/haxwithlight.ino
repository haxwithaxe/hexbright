/*
Copyright (c) 2012, "David Hilton" <dhiltonp@gmail.com>
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

Copyright (C) 2013 haxwithaxe
- modified for less complexity
- added lower bound on button press time to help prevent accidentally turning on the light.
*/

#include <hexbright.h>

#include <Wire.h>

#define OFF_MODE 0
#define BLINKY_MODE 1
#define CYCLE_MODE 2

#define LEVEL_MAX 1000
#define LEVEL_MIN 1
#define DRV_MIN 150
#define DRV_MAX 255
#define LEVELS_LEN 5
#define MIN_PRESSED_MS 200
#define PRESSED_MS 600
#define HELD_MS 700

int cycle_level_index = 0;

hexbright hb;

struct LightMode_t {
	int light;
	int driver;
	LightMode_t(): light(0), driver(0) {};
	LightMode_t(int l, int d): light(l), driver(d){};
};

int mid_drv = ((DRV_MAX-DRV_MIN)/(LEVELS_LEN-1))+DRV_MIN;
int upper_mid_drv = ((DRV_MAX-DRV_MIN)/(LEVELS_LEN-1))*2+DRV_MIN;

LightMode_t cycle_levels[LEVELS_LEN] = {
	LightMode_t(LEVEL_MIN, DRV_MIN),
	LightMode_t(LEVEL_MAX/(LEVELS_LEN-1), mid_drv),
	LightMode_t((LEVEL_MAX/(LEVELS_LEN-1))*2, upper_mid_drv),
	LightMode_t(LEVEL_MAX, DRV_MAX),
    LightMode_t(OFF_LEVEL, 0)
};

struct State_t {
	int btn_mode;
	int mode;
	LightMode_t level;
	State_t():btn_mode(0), mode(0) {};
	State_t(int b, int m):btn_mode(b), mode(m) {};
};

State_t state(0, 0);

void setup() {
  hb.init_hardware();
} 

void loop() {
	hb.update();

	// Button actions to recognize, one-time actions to take as a result
	if(hb.button_just_released() && hb.button_pressed_time()>MIN_PRESSED_MS && hb.button_pressed_time()<PRESSED_MS) {
		state.btn_mode = PRESSED;
	}
	if(hb.button_pressed_time()>HELD_MS) { // if held for over HELD_MS milliseconds (whether or not it's been released)
		state.btn_mode = HELD;
	}

	// Actions for each btn_mode
	switch (state.btn_mode) {
		case PRESSED:
			break;
		case HELD:
			break;
	}
	

	// Actions over time for a given mode
	switch (mode) {
		case CYCLE_MODE:
			if(!hb.printing_number()) hb.print_number(hb.get_fahrenheit());
			break;
		case OFF_MODE: // charging, or turning off
			if(!hb.printing_number()) hb.print_charge(GLED);
			break;
	}
}

int cycle_brightness(int index) {
	if (index >= LEVELS_LEN-1) {
		set_off();
		index = 0;
	} else {
		index++;
		set_light(cycle_levels[index].light, cycle_levels[index].driver);
	}
	return index;
}

void blink(unsigned long *last_blink_on, unsigned long on_delta, unsigned long off_delta, int rate) {
	if (millis() - *last_blink_on > delta) {
		state.levels.light = CURRENT_LEVEL;

		*last_blink_on = millis();
	}
}

void set_light(int light, int driver) {
	hb.set_light(CURRENT_LEVEL, light, driver);
}

void set_off() {
	state.mode = OFF_MODE;
	hb.set_light(CURRENT_LEVEL, OFF_LEVEL, NOW);
}
