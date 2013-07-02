/*
 * Arduino.h
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#include "Firmata.hpp"

class ArduinoJS;

namespace firmata {

	class Arduino : public FirmataDataReceiver {
	public:
		enum DigitalState {
			HIGH = 1,
			LOW = 0
		};
		Arduino(Connection * connection, ArduinoJS * parent);

		void pinMode(unsigned pin, unsigned mode);

		bool digitalRead(unsigned pin);
		void digitalWrite(unsigned pin, bool value);

		unsigned int analogRead(unsigned pin);
		void analogWrite(unsigned pin, unsigned value);

		//void shiftOut(int dataPin, int clockPin, int bitOrder, byte value);
		//byte shiftIn(int dataPin, int clockPin, int bitOrder);
		//int  pulseIn(int pin, int value);
		//int  pulseIn(int pin, int value, int timeout);
		//void tone(int pin, int frequency);
		//void noTone(int pin);

		//FirmataDataReceiver
		void analogReadCallback(unsigned pin, unsigned value);
		void digitalReadCallback(unsigned port, unsigned data);

		~Arduino();
	//protected:
		Arduino();
		Firmata * firmata;
		ArduinoJS * parent;
		byte digitalData[16];
		byte analogData[16];
	};
}
#endif /* ARDUINO_H_ */
