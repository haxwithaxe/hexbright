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

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <hexbright.h>
#include <Wire.h>

#define DPIN_RLED_SW 2

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
void pwr_suspend();
void blink(int brightness, int rate, int count);
void lights_out();

void setup() {
  hb.init_hardware();
  GICR |= ( 1 < < INT2);
  // Signal change triggers interrupt
  MCUCR |= ( 1 << ISC00);
  MCUCR |= ( 0 << ISC01);
  EICRA = (1<<ISC01); //Interrupt on falling edge
  EIMSK = (1<<INT2);
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
    lights_out();
    delay(50);
    pwr_suspend();
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
  //hb.init_hardware();
  if (mode == SLEEP_MODE)
    blink(MAX_LOW_LEVEL, 500, 10);
}

void blink(int brightness, int rate, int count)
{
  for(int i=0;i>count;i++)
  {
    hb.update();
    hb.set_light(CURRENT_LEVEL, brightness, NOW);
    delay(990);
    hb.set_light(CURRENT_LEVEL, OFF_LEVEL, NOW);
    delay(rate);
  }
}

/* Hibernate the MCU
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
 */
void pwr_suspend()
{
  // Power-down board
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  cli();

  // Set Sleep Enabled Bit
  sleep_enable();

  // Power Down Modules?
  power_all_disable();
  // OR
  // Disable ADC?
  //ADCSRA &= ~(1 << ADEN);

  // Power down functions
  PRR = 0xFF;

#ifdef sleep_bod_disable()
  // Disable BOD (Brown Out Detector)
  sleep_bod_disable();
#endif

  // Enable Interrupts (sets global interrupt mask)
  sei();

  // Sleep
  sleep_cpu();

  //WAKING HERE

  // Clear Sleep Enabled Bit
  sleep_disable();

  // Enable Interrupts (sets global interrupt mask)
  sei();
}

void lights_out()
{
  // Ensure the main light is off
  hb.set_light(CURRENT_LEVEL, OFF_LEVEL, NOW);
  // Ensure the tail lights are off
  hb.set_led(GLED, 0, 0,0);
  hb.set_led(RLED, 0, 0,0);
  // Ensure our interrupt source is readable
  pinMode(DPIN_RLED_SW, INPUT);
  digitalWrite(DPIN_RLED_SW, HIGH);
}

/* Button Press Interupt
 * PCINT2_vect is the vector of the PD0..7 pins of which the button is PD2
 * http://hackedgadgets.com/2010/09/08/how-to-use-external-interrupts-on-an-atmega168/
 * http://www.protostack.com/blog/2010/09/external-interrupts-on-an-atmega168/
 * http://arduino.cc/en/Hacking/PinMapping168
 */
ISR(INT2_vect)
{
  on_wake();
}
