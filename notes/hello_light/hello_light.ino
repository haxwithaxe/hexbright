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
*/

#include <hexbright.h>

#include <Wire.h>

#define DEBUG 2

#define OFF_MODE 0
#define ON_MODE 1
#define BLINK_MODE 2
#define SLEEP_MODE 3
#define ON 1
#define OFF 0

#define BLINK_WAIT 900

int mode = 0;
int timer = 0; 
int blink_mode = ON;
int blink_count = 1;

hexbright hb;

void setup() {
  hb.init_hardware();
} 

void loop() {
  hb.update();

  //// Button actions to recognize, one-time actions to take as a result
  if(hb.button_just_released()) {
    if(hb.button_pressed_time()<300) { //<300 milliseconds
		if (mode == OFF_MODE) {
			mode = ON_MODE;
		} else {
			mode = OFF_MODE;
			hb.set_light(CURRENT_LEVEL, OFF_LEVEL, 150);
		}
    }
  }
  if(hb.button_pressed_time()>700) { // if held for over 700 milliseconds (whether or not it's been released), go to SLEEP mode
    mode = SLEEP_MODE;
    hb.set_light(CURRENT_LEVEL, OFF_LEVEL, NOW);
	//pwr_sleep();
#if (DEBUG!=DEBUG_OFF)
	Serial.println("Awakened.");
#endif
  }
  //// Actions over time for a given mode
  if(mode == SLEEP_MODE) { // just got out of sleep mode
	mode == BLINK_MODE;
	blink_count = 5;
  } else if(mode == BLINK_MODE){
    if(millis()-timer>BLINK_WAIT){
		hb.set_light(blink_mode==OFF?OFF_LEVEL:MAX_LOW_LEVEL,0,30); // fade from 500 to 0 over 30 milliseconds
		blink_mode = blink_mode==OFF?ON:OFF;
		blink_count--;
    }
	if(blink_count == 0) mode = OFF_MODE;
  } else if(mode == ON_MODE && CURRENT_LEVEL != MAX_LOW_LEVEL) {
		hb.set_light(CURRENT_LEVEL, MAX_LOW_LEVEL, 150);
  } else if (mode == OFF_MODE) { // charging, or turning off
    if(!hb.printing_number()) {
      hb.print_charge(GLED);
    }
  }
}
