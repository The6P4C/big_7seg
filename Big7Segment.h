#ifndef BIG7SEGMENT_H
#define BIG7SEGMENT_H

#include "Arduino.h"

class Big7Segment {
public:
	Big7Segment(int latchPin, int clockPin, int dataPin, int enable04Pin, int enable15Pin, int enable26Pin, int enable3Pin);

	void SetSegments(int displayIndex, uint8_t segments);
	void SetNumber(int displayIndex, int n);
	void SetBigNumber(unsigned long n);
	void SetDecimalPoint(int displayIndex, bool isOn);

	void Update();

private:
	int m_latchPin;
	int m_clockPin;
	int m_dataPin;

	int m_enable04Pin;
	int m_enable15Pin;
	int m_enable26Pin;
	int m_enable3Pin;

	int m_displayIndex;

	uint8_t m_segments[7] = { 0 };

	uint8_t NumberToSegments(int n);
};

#endif
