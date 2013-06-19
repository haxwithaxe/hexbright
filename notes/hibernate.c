#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/* Button Press Interupt
 * PCINT2_vect is the vector of the PD0..7 pins of which the button is #2
 * http://hackedgadgets.com/2010/09/08/how-to-use-external-interrupts-on-an-atmega168/
 * http://www.protostack.com/blog/2010/09/external-interrupts-on-an-atmega168/
 * http://arduino.cc/en/Hacking/PinMapping168
 */
ISR(BTN_vect, ISR_NOBLOCK)
{
	on_wake();
}

/* Hibernate the MCU
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
 */
void pwr_suspend()
{                
  // Ensure our interrupt source is readable
  pinMode(BTN_PIN, INPUT);

  // Power-down board
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  cli();

  // Set Sleep Enabled Bit
  sleep_enable();

  // Power Down Modules?
  power_all_disable()
  // OR
  // Disable ADC?
  ADCSRA &= ~(1 << ADEN);

  // Power down functions
  PRR = 0xFF;

  // Disable BOD (Brown Out Detector)
  sleep_bod_disable();

  // Enable Interrupts (sets global interrupt mask)
  sei();

  // Sleep
  sleep_cpu();

  // Clear Sleep Enabled Bit
  sleep_disable();

  // Enable Interrupts (sets global interrupt mask)
  sei(); 
}
