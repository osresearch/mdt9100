/** \file
 * MDT 9100 keyboard interface
 *
 * The keyboard has two parts, one in the body and one in the CRT.
 * This connects to both halves via the ribbon cable.
 *
 * This sketch is designed for a Teensy 2.
 *
 * The rows are ribbon cable pins 1-8
 * The columns are ribbon cable pins 9-18
 * Caps lock LED is 19
 * Keyboard ground is 20
 */

// these are teensy pin numbers, connected to the anode of the diodes
static const uint8_t rows[] = {
	13,	// ribbon 1
	8,	// ribbon 2
	14,	// ribbon 3
	7,	// ribbon 4
	15,	// ribbon 5
	6,	// ribbon 6
	16,	// ribbon 7
	5,	// ribbon 8
};

// these are teensy pin numbers, connected to the switch, debounce and cathode
static const uint8_t cols[] = {
	17,	// ribbon 9
	4,	// ribbon 10
	18,	// ribbon 11
	3,	// ribbon 12
	19,	// ribbon 13
	2,	// ribbon 14
	20,	// ribbon 15
	1,	// ribbon 16
	21,	// ribbon 17
	0,	// ribbon 18
};

static const unsigned num_rows = sizeof(rows)/sizeof(*rows);
static const unsigned num_cols = sizeof(cols)/sizeof(*cols);

void setup()
{
	for(unsigned i = 0 ; i < num_rows ; i++)
		pinMode(rows[i], INPUT);
	for(unsigned i = 0 ; i < num_cols ; i++)
		pinMode(cols[i], INPUT);

	Serial.begin(115200);
}

void loop()
{
	// scan the rows, setting one hot and seeing if any of the
	// columns is also hot
	for(uint8_t i = 0 ; i < num_rows ; i++)
	{
		const uint8_t row = rows[i];
		pinMode(row, OUTPUT);
		digitalWrite(row, 1);

		for(uint8_t j = 0 ; j < num_rows ; j++)
		{
			const uint8_t col = cols[j];
			const uint8_t val = digitalRead(col);
			if (val)
			{
				Serial.print(row);
				Serial.print(" ");
				Serial.println(col);
			}
		}

		digitalWrite(row, 0);
		pinMode(row, INPUT);
	}
}
