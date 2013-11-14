/*
Copyright (c) 2013, haxwithaxe <me@haxwithaxe.net>
All rights reserved.
Licensed under GPLv3 (see LICENSE file in repository root)
*/


#include <Arduino.h>
#include <hexbright.h>
#include "strobe.h"
#include "morse.h"


Morse::Morse(Strobe& stb):
	strobe(stb)
{
	((void)0);
};

void Morse::to_morse(char ch) {
	unsigned int m_ch = ascii_to_morse(ch);
	if (m_ch != 0x0000) {
		uint8_t curChar = m_ch & 0x00FF;
		uint8_t nSymbols = m_ch >> 8;
		for (unsigned int j = 0; j < nSymbols; j++){
			//if dash pulse_len = 3
			if (curChar & 1) strobe.pulse(MAX_LEVEL, TAP_FREQ*3);
			//else pulse_len = 1
			else strobe.pulse(MAX_LEVEL, TAP_FREQ);
			// delay for key return
			delay(TAP_FREQ);
		};
	};
};

void Morse::send_morse(char* message) {
	for (unsigned int i = 0; i < sizeof(message); i++) {
		to_morse(message[i]);
	};
};

void Morse::sos() {
	send_morse("SOS");
}

/* ASCII to Morse mapper
 * High byte = length
 * Low byte = morse code, LSB first, 0=dot 1=dash
 */
unsigned int Morse::ascii_to_morse(char ch) {
	switch(ch) {
		// lower
		case 'a': return 0x0202; // A .-
	    case 'b': return 0x0401; // B -...
	    case 'c': return 0x0405; // C -.-.
	    case 'd': return 0x0301; // D -..
	    case 'e': return 0x0100; // E .
	    case 'f': return 0x0404; // F ..-.
	    case 'g': return 0x0303; // G --.
	    case 'h': return 0x0400; // H ....
		case 'i': return 0x0200; // I ..
		case 'j': return 0x040E; // J .---
		case 'k': return 0x0305; // K -.-
		case 'l': return 0x0402; // L .-..
		case 'm': return 0x0203; // M --
		case 'n': return 0x0201; // N -.
		case 'o': return 0x0307; // O ---
		case 'p': return 0x0406; // P .--.
		case 'q': return 0x040B; // Q --.-
		case 'r': return 0x0302; // R .-.
		case 's': return 0x0300; // S ...
		case 't': return 0x0101; // T -
		case 'u': return 0x0304; // U ..-
		case 'v': return 0x0408; // V ...-
		case 'w': return 0x0306; // W .--
		case 'x': return 0x0409; // X -..-
		case 'y': return 0x040D; // Y -.--
		case 'z': return 0x0403; // Z --..
		// upper (duplicate of lower)
		case 'A': return 0x0202; // A .-
		case 'B': return 0x0401; // B -...
		case 'C': return 0x0405; // C -.-.
		case 'D': return 0x0301; // D -..
		case 'E': return 0x0100; // E .
		case 'F': return 0x0404; // F ..-.
		case 'G': return 0x0303; // G --.
		case 'H': return 0x0400; // H ....
		case 'I': return 0x0200; // I ..
		case 'J': return 0x040E; // J .---
		case 'K': return 0x0305; // K -.-
		case 'L': return 0x0402; // L .-..
		case 'M': return 0x0203; // M --
		case 'N': return 0x0201; // N -.
		case 'O': return 0x0307; // O ---
		case 'P': return 0x0406; // P .--.
		case 'Q': return 0x040B; // Q --.-
		case 'R': return 0x0302; // R .-.
		case 'S': return 0x0300; // S ...
		case 'T': return 0x0101; // T -
		case 'U': return 0x0304; // U ..-
		case 'V': return 0x0408; // V ...-
		case 'W': return 0x0306; // W .--
		case 'X': return 0x0409; // X -..-
		case 'Y': return 0x040D; // Y -.--
		case 'Z': return 0x0403; // Z --..
		// digits
		case '0': return 0x051F; // 0 -----
		case '1': return 0x051E; // 1 .----
		case '2': return 0x051C; // 2 ..---
		case '3': return 0x0518; // 3 ...--
		case '4': return 0x0510; // 4 ....-
		case '5': return 0x0500; // 5 .....
		case '6': return 0x0501; // 6 -....
		case '7': return 0x0503; // 7 --...
		case '8': return 0x0507; // 8 ---..
		case '9': return 0x050F; // 9 ----.
		default: return 0x0000; // null
	};
};

