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

#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <hexbright.h>
#include <Wire.h>
#include "pins.h"

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

void on_wake();
void pre_suspend();
void suspend();
void tail_off();
void light_off();
void lights_out();

void setup() {
	hb.init_hardware();
	pre_suspend();
} 

void loop() {
  hb.update();

  // Button actions to recognize, one-time actions to take as a result
  if(hb.button_just_released()) {
    if(hb.button_pressed_time()<600) { //<600 milliseconds
      if (mode == OFF_MODE) {
        mode = ON_MODE;
      } 
      else {
        mode = OFF_MODE;
      }
    }
  }
  if(hb.button_pressed_time()>700) { // if held for over 700 milliseconds (whether or not it's been released), go to SLEEP mode
    mode = SLEEP_MODE;
    pre_suspend();
    suspend();
#if (DEBUG!=DEBUG_OFF)
    Serial.println("Awakened.");
#endif
  }
  // Actions over time for a given mode
  if(mode == SLEEP_MODE) { // just got out of sleep mode
    mode = ON_MODE;
  }
  if(mode == ON_MODE && CURRENT_LEVEL != MAX_LOW_LEVEL) {
    hb.set_light(CURRENT_LEVEL, MAX_LOW_LEVEL, 150);
  }
  else if (mode == OFF_MODE) { // charging, or turning off
    if (CURRENT_LEVEL != OFF_LEVEL)
      hb.set_light(CURRENT_LEVEL,OFF_LEVEL,NOW);
    if(!hb.printing_number())
      hb.print_charge(GLED);
  }
}

void on_wake()
{
	// if mode set to SLEEP_MODE and the tail button was just pressed or is pressed
	if (mode == SLEEP_MODE && digitalReadFast(DPIN_RLED_SW) > 0)
	{
		hb.init_hardware();
		hb.update();
	}
}

void pre_suspend()
{
	/*
	 * Run in setup and before suspend
	 * http://www.atmel.com/Images/doc8468.pdf
	 * section 3.4
	 */
	
	//Enable pull-up on PORTD BIT0 (INT0) to connect to switch SW1
	PORTD=(1<<PD0);

	//Configure INT0 to sense rising edge
	EICRA=(EICRA&(~(1<<ISC01|1<<ISC00)))|(1<<ISC01|1<<ISC00);

	//Enable INT0
	EIMSK|=(1<<INT0);

	//Set global interrupt enable bit
	sei();

	//Set sleep mode to Power-Down mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();

}

void suspend()
{
	//Turn main light off
	
	//Turn tail lights off

	//Enter into sleep mode
	sleep_cpu();
}

void tail_off()
{
	// DPIN_RLED_SW
	digitalWriteFast(DPIN_RLED_SW, LOW);
	pinModeFast(DPIN_RLED_SW, INPUT);
	// DPIN_GLED
	digitalWriteFast(DPIN_GLED, LOW);
}

void light_off()
{
	pinModeFast(DPIN_PWR, OUTPUT);
	digitalWriteFast(DPIN_PWR, LOW);
	digitalWriteFast(DPIN_DRV_MODE, LOW);
	analogWrite(DPIN_DRV_EN, 0);
}

void lights_out()
{
  // Ensure the main light is off
  light_off();
  // Ensure the tail lights are off
  tail_off();
  // Ensure our interrupt source is readable
  pinModeFast(DPIN_RLED_SW, INPUT);
}

/* Button Press Interupt
 * PCINT2_vect is the vector of the PD0..7 pins of which the button is PD2
 * http://hackedgadgets.com/2010/09/08/how-to-use-external-interrupts-on-an-atmega168/
 * http://www.protostack.com/blog/2010/09/external-interrupts-on-an-atmega168/
 * http://arduino.cc/en/Hacking/PinMapping168
 */
ISR(INT0_vect)
{
  on_wake();
}
