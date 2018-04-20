//*****************************************//
//  KeyFive.cpp
//  by Timotheos Samartzidis 2018
//  uses the RtMidi Library from Gary Scavone, 2003-2004.
//
//  implements a single hand 5 finger writing system with midi input
//
//*****************************************//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "RtMidi.h"
#include <ctime>

#define WINVER 0x0500
#include <windows.h>


// This structure will be used to create the keyboard
// input event.
INPUT ip;

// charts
int chart[5] = { 0,0,0,0,0 };
int chart_offchk[5] = { 0,0,0,0,0 };
int last_key = -1;
char char_array[32] = { 'i','o','ä','t','ü','n','l','e','b','y','j','s','v','h','d','a','x','y','f','m','w','c','p','u','ö','d','\n','r','z','g',' ' };
WORD word_array[32] =	{ 0x49,0x4F,0x41,0x54,0x55,0x4E,0x4C,0x45,0x42,0x59,0x4A,0x53,0x56,0x48,0x44,0x41,0x58,0x59,0x46,0x4D,0x57,0x43,0x50,0x55,0x2E,0x44,0x08,0x52,0x5A,0x47,0x20};
WORD word_n_array[32] = { 0x6D,0x38,0x6B,0x34,0x2C,0x2C,0x2C,0x32,0x2C,0x30,0x2C,0x36,0x6F,0x6A,0x2C,0x31,0x2C,0x39,0x2C,0x35,0x2C,0x2C,0x2C,0x33,0x2E,0x2C,0x08,0x37,0x2C,0x2C,0x20};

int current_keymap = 0; // whitch word_array to use currently just 0, 1

// measure time between keystrokes
clock_t begin = clock();
clock_t end = clock();
double elapsed_secs = 0.0;

// midi numbers of the key pressed
int midi_code[5] = { 60,62,64,65,67 };

// initialize virtual Keyboard
void init_keyboard() {
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
}

// send virtual Keypress to pc
void send_keypress(WORD key) {
	ip.ki.wVk = key;
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

// How to use command-line arguments
void usage(void) {
	// Error function in case of incorrect command-line
	// argument specifications.
	std::cout << "\nuseage: KeyFive <port>\n";
	std::cout << "    where port = the device to use (default = 0).\n\n";
	exit(0);
}

// Callback on recieved data from midi channel
void mycallback(double deltatime, std::vector< unsigned char > *message, void */*userData*/)
{
	
	unsigned int nBytes = message->size();
	unsigned int crux_byte = 0;
	unsigned int crux_value = 0;
	bool renew = false;
	unsigned int arraycode = 0;

	if (nBytes > 1)
	{
		// check significant byte to determine the key pressed
		crux_byte = (int)message->at(1);
		// check significant byte to determine wether the key is released
		crux_value = (int)message->at(2);

		if (crux_value != 0) {


			for (int i = 0; i < 5; i++) {
				if (crux_byte == midi_code[i]) chart[i] = 1;
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (crux_byte == midi_code[i]) chart_offchk[i] = 1;
			}
		}

		if (!renew) { 
			renew = true;
			for (int i = 0; i < 5; i++) {
				if (chart[i] > 0) {
					if (chart[i] != chart_offchk[i]) {
						renew = false;
					}
				}
			}
		}

		if (renew)
		{
			arraycode = 0;
			// set bits to generate the 32 possible combinations 5 bits can be set for the numbers 0...32
			for (int i = 0; i < 5; i++) {
				if (chart[i] > 0) {
					arraycode |= (1u << (4-i));
				}
			}
			arraycode--; // correct arraycode from 1-32 to 0-31
			//std::cout << arraycode << std::endl;
			//std::cout << chart[0] << chart[1] << chart[2] << chart[3] << chart[4] << " char: " << char_array[arraycode] << std::endl;
			//std::cout << char_array[arraycode];
			
			// clean chart arrays
			for (int i = 0; i < 5; i++) {
				chart[i] = 0;
				chart_offchk[i] = 0;
			}

			// measure time between valid keypresses
			end = clock();
			elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			begin = clock();
			std::cout << elapsed_secs << std::endl;
			// check for doublepresses of specific keys (double space switches to secondary keyboard, numbers)
			if (elapsed_secs < 0.3 && last_key == 0x20 && last_key == word_array[arraycode]) {
				if (current_keymap < 1) current_keymap++;
				else current_keymap = 0;
				std::cout << "Current keymap: " << current_keymap << std::endl;
			}
			else {
 				if (current_keymap == 0) send_keypress(word_array[arraycode]); last_key = word_array[arraycode];
				if (current_keymap == 1) send_keypress(word_n_array[arraycode]); last_key = word_n_array[arraycode];

			}
		}
	}
	
}

// function inmported from test midi input by Gary Scavone, 2003-2004.
// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort(RtMidiIn *rtmidi);

int main(int argc, char ** /*argv[]*/)
{
	RtMidiIn *midiin = 0;

	// Minimal command-line check.
	if (argc > 2) usage();

	// load settings of the key position to use on the keyboard
	int t_midi_code[5] = { 0,0,0,0,0 };
	bool cont = false;
	try {
		
		std::ifstream settings("config.ini");
		
		for (int i = 0; i < 5; i++) {
			std::string x = "";
			settings >> x;
			t_midi_code[i] = std::stoi(x);
		}
		cont = true;
	}
	catch (RtMidiError &error) {
		error.printMessage();
	}
	if (cont) std::copy(std::begin(t_midi_code), std::end(t_midi_code), std::begin(midi_code)); // copy loaded array to actual array used if loaded correctly

	try {

		// RtMidiIn constructor
		midiin = new RtMidiIn();

		// Call function to select port.
		if (chooseMidiPort(midiin) == false) goto cleanup;

		// Set our callback function.  This should be done immediately after
		// opening the port to avoid having incoming messages written to the
		// queue instead of sent to the callback function.
		midiin->setCallback(&mycallback);

		// Don't ignore sysex, timing, or active sensing messages.
		midiin->ignoreTypes(false, false, false);

		std::cout << "\nInitializing virtual Keyboard...";
		init_keyboard();
		std::cout << " ...Initialized\n";

		std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
		char input;
		std::cin.get(input);

	}
	catch (RtMidiError &error) {
		error.printMessage();
	}

cleanup:

	delete midiin;

	return 0;
}

// function inmported from test midi input by Gary Scavone, 2003-2004.
bool chooseMidiPort(RtMidiIn *rtmidi)
{
	std::string keyHit;
	std::string portName;
	unsigned int i = 0, nPorts = rtmidi->getPortCount();
	if (nPorts == 0) {
		std::cout << "No input ports available!" << std::endl;
		return false;
	}

	if (nPorts == 1) {
		std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
	}
	else {
		for (i = 0; i<nPorts; i++) {
			portName = rtmidi->getPortName(i);
			std::cout << "  Input port #" << i << ": " << portName << '\n';
		}

		do {
			std::cout << "\nChoose a port number: ";
			std::cin >> i;
		} while (i >= nPorts);
		std::getline(std::cin, keyHit);  // used to clear out stdin
	}

	rtmidi->openPort(i);

	return true;
}
