/*
 * Firmata.h
 *
 *  Created on: 13-03-2013
 *      Author: Jakub Kramarz (http://lenwe.net)
 */

#ifndef FIRMATA_H_
#define FIRMATA_H_

#include <pthread.h>
#include "FirmataDataReceiver.hpp"
#include "Connection.hpp"
#include <iostream>

namespace firmata {

	enum Messages {
		// message command bytes (128-255/0x80-0xFF)
		DIGITAL_MESSAGE = 0x90,// send data for a digital pin
		ANALOG_MESSAGE = 0xE0,// send data for an analog pin (or PWM)
		REPORT_ANALOG = 0xC0,// enable analog input by pin #
		REPORT_DIGITAL = 0xD0,// enable digital input by port pair
		SET_PIN_MODE = 0xF4,// set a pin to INPUT/OUTPUT/PWM/etc
		REPORT_VERSION = 0xF9,// report protocol version
		SYSTEM_RESET = 0xFF,// reset from MIDI
		START_SYSEX = 0xF0,// start a MIDI Sysex message
		END_SYSEX = 0xF7,// end a MIDI Sysex message
		// extended command set using sysex (0-127/0x00-0x7F)
		// 0x00-0x0F reserved for user-defined commands
		SERVO_CONFIG = 0x70,// set max angle, minPulse, maxPulse, freq
		// STRING_DATA           = 0x71, // a string message with 14-bits per char
		// SHIFT_DATA            = 0x75, // a bitstream to/from a shift register
		// I2C_REQUEST           = 0x76, // send an I2C read/write request
		// I2C_REPLY             = 0x77, // a reply to an I2C read request
		// I2C_CONFIG            = 0x78, // config I2C settings such as delay times and power pins
		EXTENDED_ANALOG = 0x6F,// analog write (PWM, Servo, etc) to any pin
		// PIN_STATE_QUERY       = 0x6D, // ask for a pin's current mode and value
		// PIN_STATE_RESPONSE    = 0x6E, // reply with pin's current mode and value
		CAPABILITY_QUERY = 0x6B,// ask for supported modes and resolution of all pins
		CAPABILITY_RESPONSE = 0x6C,// reply with supported modes and resolution
		// ANALOG_MAPPING_QUERY  = 0x69, // ask for mapping of analog to pin numbers
		// ANALOG_MAPPING_RESPONSE=0x6A, // reply with mapping info
		// REPORT_FIRMWARE       = 0x79, // report name and version of the firmware
		SAMPLING_INTERVAL = 0x7A,// set the poll rate of the main loop
		// SYSEX_NON_REALTIME    = 0x7E, // MIDI Reserved for non-realtime messages
		// SYSEX_REALTIME        = 0x7F, // MIDI Reserved for realtime messages
	};
	enum PinStates {
		INPUT = 0x00,
		OUTPUT = 0x01,
		ANALOG = 0x02,
		PWM = 0x03,
		SERVO = 0x04,
		SHIFT = 0x05,
		I2C = 0x06
	};

	class Firmata {
	public:
		Firmata(Connection * connection);
		~Firmata();
		
		//Simple messages
		void analogWrite(byte pin, byte value);
		void digitalWrite(byte port, byte value);
		void setPinMode(byte pin, byte state);
		void setReportAnalog(byte pin, bool state);
		void setReportDigital(byte port, bool state);
		void systemReset();
		void reportVersion();
		//Sysex messages
		void analogExtendedWrite(byte pin, byte value);
		void capabilitiesQuery();
		void setSamplinginterval(byte interval_ms);
		void servoConfig(byte pin, byte minpulse, byte maxpulse);

		bool startThread(FirmataDataReceiver * receiver);
		void stopThread();
		bool isThreadHalt();
	private:
		Connection * connection;
		FirmataDataReceiver * receiver;
		static void * thread(void * parent);
		void startSysex();
		void endSysex();
		void writeByte(byte data);
		byte readByte();
		void sendValueAsTwoBytes(byte value);

		pthread_t m_thread;
		pthread_cond_t cond;
		pthread_mutex_t mutex;
		bool threadHalt;
	};

}
#endif /* FIRMATA_H_ */
