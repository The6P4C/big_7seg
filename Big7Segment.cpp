#include "Big7Segment.h"

const uint8_t DECIMAL_POINT = 0b10000000;

Big7Segment::Big7Segment(int latchPin, int clockPin, int dataPin, int enable04Pin, int enable15Pin, int enable26Pin, int enable3Pin)
	: m_latchPin(latchPin), m_clockPin(clockPin), m_dataPin(dataPin), m_enable04Pin(enable04Pin), m_enable15Pin(enable15Pin), m_enable26Pin(enable26Pin), m_enable3Pin(enable3Pin),
	  m_displayIndex(0) {

	pinMode(m_latchPin, OUTPUT);
	pinMode(m_clockPin, OUTPUT);
	pinMode(m_dataPin, OUTPUT);

	pinMode(m_enable04Pin, OUTPUT);
	pinMode(m_enable15Pin, OUTPUT);
	pinMode(m_enable26Pin, OUTPUT);
	pinMode(m_enable3Pin, OUTPUT);
}

void Big7Segment::SetSegments(int displayIndex, uint8_t segments) {
	m_segments[displayIndex] = segments;
}

void Big7Segment::SetNumber(int displayIndex, int n) {
	m_segments[displayIndex] = (m_segments[displayIndex] & DECIMAL_POINT) | NumberToSegments(n);
}

unsigned long ipow(unsigned long base, unsigned long exponent) {
	unsigned long result = 1;

	for (unsigned long i = 0; i < exponent; ++i) {
		result *= base;
	}

	return result;
}

void Big7Segment::SetBigNumber(unsigned long n) {
	if (n > 9999999) {
		for (int i = 0; i < 7; ++i) {
			SetNumber(i, -1);
		}
	} else {
		int digits[7];

		for (int i = 0; i < 7; ++i) {
			digits[6 - i] = (n / ipow(10, i)) % 10;
		}

		bool numEncountered = false;
		for (int i = 0; i < 7; ++i) {
			if (digits[i] == 0 && !numEncountered) {
				digits[i] = -1;
			}

			if (digits[i] > 0) {
				numEncountered = true;
			}
		}

		for (int i = 0; i < 7; ++i) {
			SetNumber(i, digits[i]);
		}
	}
}

void Big7Segment::SetDecimalPoint(int displayIndex, bool isOn) {
	if (isOn) {
		m_segments[displayIndex] |= DECIMAL_POINT;
	} else {
		m_segments[displayIndex] &= ~DECIMAL_POINT;
	}
}

void Big7Segment::Update() {
	int enablePin;
	int data1Idx;
	int data2Idx;

	switch (m_displayIndex) {
	case 0:
		enablePin = m_enable04Pin;
		data1Idx = 0;
		data2Idx = 4;
		break;

	case 1:
		enablePin = m_enable15Pin;
		data1Idx = 1;
		data2Idx = 5;
		break;

	case 2:
		enablePin = m_enable26Pin;
		data1Idx = 2;
		data2Idx = 6;
		break;

	case 3:
		enablePin = m_enable3Pin;
		
		// data for display 3 is taken from the second byte, so this byte can
		// basically be ignored
		data1Idx = 0;
		data2Idx = 3;
		break;
	}

	int data1Segs = m_segments[data1Idx];
	int data2Segs = m_segments[data2Idx];

	digitalWrite(m_enable04Pin, LOW);
	digitalWrite(m_enable15Pin, LOW);
	digitalWrite(m_enable26Pin, LOW);
	digitalWrite(m_enable3Pin, LOW);

	digitalWrite(m_latchPin, LOW);
	shiftOut(m_dataPin, m_clockPin, MSBFIRST, data1Segs);
	shiftOut(m_dataPin, m_clockPin, MSBFIRST, data2Segs);
	digitalWrite(m_latchPin, HIGH);

	digitalWrite(enablePin, HIGH);

	++m_displayIndex;
	m_displayIndex = m_displayIndex % 4;
}

uint8_t Big7Segment::NumberToSegments(int n) {
	switch (n) {
	case 0:
		return 0b00111111;
	case 1:
		return 0b00000110;
	case 2:
		return 0b01011011;
	case 3:
		return 0b01001111;
	case 4:
		return 0b01100110;
	case 5:
		return 0b01101101;
	case 6:
		return 0b01111101;
	case 7:
		return 0b00000111;
	case 8:
		return 0b01111111;
	case 9:
		return 0b01101111;
	case 10:
		return 0b01110111;
	case 11:
		return 0b01111100;
	case 12:
		return 0b00111001;
	case 13:
		return 0b01011110;
	case 14:
		return 0b01111001;
	case 15:
		return 0b01110001;
	default:
		return 0b00000000;
	}
}
