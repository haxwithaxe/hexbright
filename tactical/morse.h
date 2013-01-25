#ifndef HEXBRIGHT_MORSE
#define HEXBRIGHT_MORSE

// milliseconds
#define TAP_FREQ 75

// default message is "SOS" (universal distress)
#define MESSAGE "SOS"

class morse {
  public:
    /* Morse to ASCII table
     * High byte = length
     * Low byte = morse code, LSB first, 0=dot 1=dash
     */
    word morse_to_ascii[128];
    // lower
    morse_to_ascii[97] = 0x0202; // A .-
    morse_to_ascii[98] = 0x0401; // B -...
    morse_to_ascii[99] = 0x0405; // C -.-.
    morse_to_ascii[100] = 0x0301; // D -..
    morse_to_ascii[101] = 0x0100; // E .
    morse_to_ascii[102] = 0x0404; // F ..-.
    morse_to_ascii[103] = 0x0303; // G --.
    morse_to_ascii[104] = 0x0400; // H ....
    morse_to_ascii[105] = 0x0200; // I ..
    morse_to_ascii[106] = 0x040E; // J .---
    morse_to_ascii[107] = 0x0305; // K -.-
    morse_to_ascii[108] = 0x0402; // L .-..
    morse_to_ascii[109] = 0x0203; // M --
    morse_to_ascii[110] = 0x0201; // N -.
    morse_to_ascii[111] = 0x0307; // O ---
    morse_to_ascii[112] = 0x0406; // P .--.
    morse_to_ascii[113] = 0x040B; // Q --.-
    morse_to_ascii[114] = 0x0302; // R .-.
    morse_to_ascii[115] = 0x0300; // S ...
    morse_to_ascii[116] = 0x0101; // T -
    morse_to_ascii[117] = 0x0304; // U ..-
    morse_to_ascii[118] = 0x0408; // V ...-
    morse_to_ascii[119] = 0x0306; // W .--
    morse_to_ascii[120] = 0x0409; // X -..-
    morse_to_ascii[121] = 0x040D; // Y -.--
    morse_to_ascii[122] = 0x0403; // Z --..
    // upper (duplicate of lower)
    morse_to_ascii[65] = 0x0202; // A .-
    morse_to_ascii[66] = 0x0401; // B -...
    morse_to_ascii[67] = 0x0405; // C -.-.
    morse_to_ascii[68] = 0x0301; // D -..
    morse_to_ascii[69] = 0x0100; // E .
    morse_to_ascii[70] = 0x0404; // F ..-.
    morse_to_ascii[71] = 0x0303; // G --.
    morse_to_ascii[72] = 0x0400; // H ....
    morse_to_ascii[73] = 0x0200; // I ..
    morse_to_ascii[74] = 0x040E; // J .---
    morse_to_ascii[75] = 0x0305; // K -.-
    morse_to_ascii[76] = 0x0402; // L .-..
    morse_to_ascii[77] = 0x0203; // M --
    morse_to_ascii[78] = 0x0201; // N -.
    morse_to_ascii[79] = 0x0307; // O ---
    morse_to_ascii[80] = 0x0406; // P .--.
    morse_to_ascii[81] = 0x040B; // Q --.-
    morse_to_ascii[82] = 0x0302; // R .-.
    morse_to_ascii[83] = 0x0300; // S ...
    morse_to_ascii[84] = 0x0101; // T -
    morse_to_ascii[85] = 0x0304; // U ..-
    morse_to_ascii[86] = 0x0408; // V ...-
    morse_to_ascii[87] = 0x0306; // W .--
    morse_to_ascii[88] = 0x0409; // X -..-
    morse_to_ascii[89] = 0x040D; // Y -.--
    morse_to_ascii[90] = 0x0403; // Z --..
    // digits
    morse_to_ascii[48] = 0x051F; // 0 -----
    morse_to_ascii[49] = 0x051E; // 1 .----
    morse_to_ascii[50] = 0x051C; // 2 ..---
    morse_to_ascii[51] = 0x0518; // 3 ...--
    morse_to_ascii[52] = 0x0510; // 4 ....-
    morse_to_ascii[53] = 0x0500; // 5 .....
    morse_to_ascii[54] = 0x0501; // 6 -....
    morse_to_ascii[55] = 0x0503; // 7 --...
    morse_to_ascii[56] = 0x0507; // 8 ---..
    morse_to_ascii[57] = 0x050F; // 9 ----.
    static void to_morse(char ch);
    static void send_morse(char* message);
}

#endif
