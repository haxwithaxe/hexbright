#include <Arduino.h>
#include "strobe.h"

static void morse::to_morse(char ch) {
      byte curChar = morse::morse_to_ascii[ch] & 0x00FF;
      byte nSymbols = morse::morse_to_ascii[ch] >> 8;
      for (int j = 0; j < nSymbols; j++){
        //if dash pulse_len = 3
	    if (curChar & 1) strobe.pulse(MAXLEVEL, TAP_FREQ*3);
        //else pulse_len = 1
        else strobe.pulse(MAXLEVEL, TAP_FREQ);
        // delay for key return
	    delay(TAP_FREQ);
	  }
}

static void morse::send_morse(char* message) {
      for (int i = 0; i < sizeof(message); i++) {
		  morse::to_morse(message[i]);
      }
}


