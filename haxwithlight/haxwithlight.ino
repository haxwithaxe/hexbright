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

#include <stdtype.h>

#define OFF_MODE 0
#define BLINKY_MODE 1
#define CYCLE_MODE 2
#define LOCK_MODE 8
#define DAZZLE_MODE 9

// Light Level Bounds
#define LEVEL_MAX 1000
#define LEVEL_MIN 1
#define DRV_MIN 150
#define DRV_MAX 255

// Number of light levels
#define LEVELS_LEN 5

// Button Press Thresholds
#define MIN_PRESSED_MS 200
#define PRESSED_MS 600
#define HELD_MS 700

// Button State
#define PRESSED 10
#define HELD 11

int cycle_level_index = 0;

hexbright hb;

int mid_drv = ((DRV_MAX-DRV_MIN)/(LEVELS_LEN-1))+DRV_MIN;
int upper_mid_drv = ((DRV_MAX-DRV_MIN)/(LEVELS_LEN-1))*2+DRV_MIN;

uint8_t cycle_levels[LEVELS_LEN] = {
	LEVEL_MIN,
	LEVEL_MAX/(LEVELS_LEN-1),
	(LEVEL_MAX/(LEVELS_LEN-1))*2,
	LEVEL_MAX,
    OFF_LEVEL
};

struct State_t {
	uint8_t btn_mode;
	uint8_t act_mode;
	int _level;
	int last_level;
	unsigned long transition_rate;

	State_t(): btn_mode(0), act_mode(OFF_MODE), level(OFF_LEVEL), last_level(OFF_LEVEL), transition_rate(NOW) {};

	State_t(uint8_t b, uint8_t m, int l):btn_mode(b), act_mode(m), level(l), last_level(OFF_LEVEL), transition_rate(NOW) {};

	void level(int new_level) {
		last_level = _level;
		_level = new_level;
	}
	int level() { return _level; }
};

State_t state();

void setup() {
  hb.init_hardware();
}

void loop() {
	hb.update();
	collect_state();
	reconsile_state();
	upkeep();
}

void collect_state() {
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
	
	switch (state.act_mode) {
		case OFF_MODE:
			break;
		case BLINKY_MODE:
			break;
		case CYCLE_MODE:
			break;
		case LOCK_MODE:
			break;
		case DAZZLE_MODE:
			break;
	}
}

int cycle_brightness(int index) {
	state.level(cycle_levels[index]);
    index++;
	if (index > LEVELS_LEN-1) index = 0;
	return index;
}

void set_blink(unsigned long *last_blink_on, unsigned long on_delta, unsigned long off_delta, unsigned long transition_rate) {
	// time until the light should be set back to `on` sum(deltas)+2*rate of transition
	unsigned long total_off_delta = on_delta + off_delta + transition_rate*2
	state.act_mode = BLINK_MODE
	if (millis() - *last_blink_on > on_delta && CURRENT_LEVEL != OFF_LEVEL) {
		state.level(OFF_LEVEL);
		state.last_blink = millis();
		state.transition_rate = transition_rate;

	} else if (millis() - state.last_blink > total_off_delta && CURRENT_LEVEL == OFF_LEVEL) {
		state.level(state.last_level);
		state.transition_rate = transition_rate;
	}
}

void upkeep() {
    // Actions over time for a given mode
    switch (state.act_mode) {
        case CYCLE_MODE:
            if(!hb.printing_number()) hb.print_number(hb.get_fahrenheit());
            break;
        case OFF_MODE: // charging, or turning off
            if(!hb.printing_number()) hb.print_charge(GLED);
            break;
    }
}

void reconsile_state() {
	if (state.level != CURRENT_LEVEL) {
		hb.set_light(CURRENT_LEVEL, state.level, state.transition_rate);
	}
}

void set_light(int light) {
	hb.set_light(CURRENT_LEVEL, light, NOW);
}

void set_off() {
	state.mode = OFF_MODE;
	hb.set_light(CURRENT_LEVEL, OFF_LEVEL, NOW);
}
