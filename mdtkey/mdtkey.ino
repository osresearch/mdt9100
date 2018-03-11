/** \file
 * MDT 9100 keyboard interface
 *
 * The keyboard has two parts, one in the body and one in the CRT.
 * This connects to both halves via the ribbon cable.
 *
 * This sketch is designed for a Teensy 3.2 since it needs pulldown
 * due to the way the diodes are wired.  Be sure to select
 * Tools - USB Type - Serial + Keyboard + Mouse
 * 
 * The rows are ribbon cable pins 1-8
 * The columns are ribbon cable pins 9-18
 *
 * Caps lock LED is 19
 * Keyboard ground is 20
 *
 * The adapter board as built alternates sides to make it easier
 * to wire up.  Caps lock LED is active high on pin 0
 */

// these are teensy pin numbers, connected to the anode of the diodes
// reversing the ribbon cable order gives us a more natural layout
static const uint8_t rows[] = {
	20,	// ribbon 8
	7,	// ribbon 7
	19,	// ribbon 6
	8,	// ribbon 5
	18,	// ribbon 4
	9,	// ribbon 3
	17,	// ribbon 2
	10,	// ribbon 1
};

// these are teensy pin numbers, connected to the switch, debounce and cathode
static const uint8_t cols[] = {
	6,	// ribbon 9
	21,	// ribbon 10
	5,	// ribbon 11
	22,	// ribbon 12
	4,	// ribbon 13
	23,	// ribbon 14
	3,	// ribbon 15
	14,	// ribbon 16 // oops
	2,	// ribbon 17
	1,	// ribbon 18
};

static const unsigned num_rows = sizeof(rows)/sizeof(*rows);
static const unsigned num_cols = sizeof(cols)/sizeof(*cols);

// top row: esc home numlock prtscr scrlk pause insert delete "main app"
// 00 01 02 03 04 05 06 07 20
// numbers 1234567890-= back pageup
// 21 22 23 24 25 26 27 30 31 32 33 34 35 36
// tab qwertyuio[] pagedown
// 37 40 41 42 43 44 45 46 47 50 51 52 53 54
// caps asdfghjkl;' return
// 55 56 57 60 61 62 63 64 65 66 67 70 17
// lshift zxcvbnm,.? rshigt up end
// 71 72 73 74 75 76 77 81 82 83 84 85 86
// rctnrl alt space ,/ left down right
// 87 90 91 92 93 94 95 96

static const int keymap[][8] = {
	/* 0 */ { KEY_ESC, KEY_HOME, KEY_NUM_LOCK, KEY_PRINTSCREEN, KEY_SCROLL_LOCK, KEY_PAUSE, KEY_DELETE },
	/* 1 */ { 0, 0, 0, 0, 0, 0, 0, KEY_ENTER },
	/* 2 */ { KEY_HOME, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7 },
	/* 3 */ { KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_PAGE_UP, KEY_TAB },
	/* 4 */ { KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I },
	/* 5 */ { KEY_O, KEY_P, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_PAGE_DOWN, KEY_CAPS_LOCK, KEY_A, KEY_S },
	/* 6 */ { KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON },
	/* 7 */ { KEY_QUOTE, MODIFIERKEY_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N },
	/* 8 */ { KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, MODIFIERKEY_RIGHT_SHIFT, KEY_UP, KEY_END, MODIFIERKEY_CTRL },
	/* 9 */ { MODIFIERKEY_ALT, KEY_SPACE, KEY_TILDE, KEY_BACKSLASH, KEY_LEFT, KEY_DOWN, KEY_RIGHT, 0 },
};


// keep track of which keys are pressed so that we can signal
// when they are released.
static uint8_t pressed[num_cols][num_rows];



void setup()
{
	for(unsigned i = 0 ; i < num_rows ; i++)
		pinMode(rows[i], INPUT);
	for(unsigned i = 0 ; i < num_cols ; i++)
		pinMode(cols[i], INPUT_PULLDOWN);

	Serial.begin(115200);
}

void loop()
{
	uint8_t found = 0;

	// scan the rows, setting one hot and seeing if any of the
	// columns is also hot
	for(uint8_t i = 0 ; i < num_rows ; i++)
	{
		// flush any stray capitance
		for(uint8_t j = 0 ; j < num_cols ; j++)
		{
			digitalWrite(cols[j], 0);
			pinMode(cols[j], OUTPUT);
		}
		delay(10);
		for(uint8_t j = 0 ; j < num_cols ; j++)
		{
			pinMode(cols[j], INPUT_PULLDOWN);
		}

		const uint8_t row = rows[i];
		pinMode(row, OUTPUT);
		digitalWrite(row, 1);
		delay(5);

		for(uint8_t j = 0 ; j < num_cols ; j++)
		{
			const uint8_t col = cols[j];
			const uint8_t val = digitalRead(col);
			if (!val)
			{
				if (pressed[j][i])
					Keyboard.release(keymap[j][i]);
				pressed[j][i] = 0;
				continue;
			}

			found++;
			const int key = keymap[j][i];
			if (1 || key == 0)
			{
				Serial.print(j);
				Serial.print(" ");
				Serial.println(i);
				continue;
			}

			Keyboard.press(key);
			pressed[j][i] = 1;
		}

		// restore the row pin to floating
		digitalWrite(row, 0);
		pinMode(row, INPUT);
	}
}
