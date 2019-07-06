/**
 * File: keyboard-2.cpp
 *       Keep in mind that each keyboard key has one and only one make code and
 *       one and only one break code. However, to the same keyboard key can
 *       correspond more than one ASCII char, based on some special keys
 *       (e.g. SHIFT, ALT, CTRL) combinations (if pressed or not).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include <libcqlk.h>
#include <kbd.h>

/**
 * Developer harness test shows how to retrieve keycodes from the keyboard and
 * print their binary representation the video output.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
	char c;

	for (;;) {
		c = char_read(); // puo' restituire 0
		if (c == 0x1B) // carattere ASCII esc
			break;
		char_write(c);	// non effettua azioni se c vale 0
	}
	return 0;
}

