#ifndef HIBERNATE_h
#define HIBERNATE_h

#define BTN_vect PCINT2_vect

void on_wake();

/* Hibernate the MCU
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
 */
void pwr_suspend();

// END define HIBERNATE_h
#endif
