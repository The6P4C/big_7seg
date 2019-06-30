#include <TimerOne.h>
#include "Big7Segment.h"

// Latch, clock, data, enable 04, enable 15, enable 26, enable 3
Big7Segment disp(8, 12, 11, 5, 6, 7, 4);

void UpdateDisplay() {
	disp.Update();
}

void setup() {
	Serial.begin(9600);

	Timer1.initialize(1000);
	Timer1.attachInterrupt(UpdateDisplay);

	disp.SetNumber(0, 1);
	disp.SetNumber(1, 8);
	disp.SetDecimalPoint(1, true);
	disp.SetNumber(2, 5);
	disp.SetNumber(3, 5);
	disp.SetDecimalPoint(3, true);
	disp.SetNumber(4, 3);

	disp.SetSegments(6, 0b01110111);

	int i = 0;
	while (true) {
		disp.SetNumber(5, i);

		++i;
		i %= 10;

		delay(1000);
	}
}

void loop() {
	// PART 1
	disp.SetSegments(0, 0b01110110);
	disp.SetNumber(1, 0xE);
	disp.SetSegments(2, 0b00111000);
	disp.SetSegments(3, 0b00111000);
	disp.SetNumber(4, 0);
	delay(2000);

	// reset
	for (int i = 0; i < 7; ++i) {
		disp.SetSegments(i, 0);
	}

	// PART 2
	for (int i = 0; i < 7; ++i) {
		disp.SetNumber(i, i);
		delay(100);
	}

	for (int i = 0; i < 7; ++i) {
		disp.SetNumber(i, -1);
		delay(100);
	}

	// PART 3
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 7; ++j) {
			disp.SetNumber(j, i);
		}
		delay(200);
	}

	// reset
	for (int i = 0; i < 7; ++i) {
		disp.SetSegments(i, 0);
	}

	// PART 4
	const uint8_t TOP = 0b00000001;
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			disp.SetSegments(j, i == j ? TOP : 0);
		}
		delay(100);
	}

	disp.SetSegments(6, 0b00000010);
	delay(100);
	disp.SetSegments(6, 0b00000100);
	delay(100);

	const uint8_t BOTTOM = 0b00001000;
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			disp.SetSegments(6 - j, i == j ? BOTTOM : 0);
		}
		delay(100);
	}

	disp.SetSegments(0, 0b00010000);
	delay(100);
	disp.SetSegments(0, 0b00100000);
	delay(100);

	// PART 5
	const uint8_t BALL_TOP = 0b01100011;
	const uint8_t BALL_BOTTOM = 0b01011100;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 7; ++i) {
			disp.SetSegments(i, (i + j) % 2 == 0 ? BALL_TOP : BALL_BOTTOM);
		}

		delay(200);
	}

	// PART 6
	uint8_t v = 1;
	while (v != 0) {
		for (int i = 0; i < 7; ++i) {
			disp.SetSegments(i, v);
		}

		v <<= 1;

		delay(200);
	}

	// reset
	for (int i = 0; i < 7; ++i) {
		disp.SetSegments(i, 0);
	}

	delay(2000);
}
