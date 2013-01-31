#ifndef HEXBRIGHT_Morse_h
#define HEXBRIGHT_Morse_h

// milliseconds
#define TAP_FREQ 75

// default message is "SOS" (universal distress)
#define DEFAULT_MESSAGE "SOS"

class Morse {
	Strobe& strobe;

	public:
		Morse(Strobe& stb);

		/* ASCII to Morse mapper
		* High byte = length
		* Low byte = morse code, LSB first, 0=dot 1=dash
		*/
		unsigned int ascii_to_morse(char ch);

		/* send the morse code for given char
		 */
		void to_morse(char ch);

		/* send string as morse code
		 */
		void send_morse(char* message);

		void sos();
};
#endif
