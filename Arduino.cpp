/*
 * Arduino.cpp
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#include "Arduino.hpp"
#include <iostream>
#include <stdint.h>
namespace firmata {

Arduino::Arduino(Connection * connection, ArduinoJS *parent) {
	firmata = new Firmata(connection);
	//firmata->startThread(this);
}
Arduino::~Arduino() {
	//firmata->stopThread();
	delete firmata;
}
void Arduino::pinMode(unsigned pin, unsigned mode) {
	firmata->setPinMode(pin, mode);
}
bool Arduino::digitalRead(unsigned pin) {
	byte port = pin / 8;
	byte bit = pin % 8;
	return (bool) ((digitalData[port] >> bit) & 0x01);
}

void Arduino::digitalWrite(unsigned pin, bool value) {
	byte port = pin / 8;
	byte bit = (pin % 8);
	byte mask = (1 << bit);
	//std::cout << "port: " << (int) port << ", bit: " << (int) bit << ", mask: " << (unsigned int) mask << std::endl;
	if (value == LOW) {
		digitalData[port] &= ~mask;
	} else { //HIGH
		digitalData[port] |= mask;
	}
	firmata->digitalWrite(port, digitalData[port]);
}

unsigned int Arduino::analogRead(unsigned pin) {
	return analogData[pin];
}
void Arduino::analogWrite(unsigned pin, unsigned value) {
	firmata->analogWrite(pin, value);
}

void Arduino::analogReadCallback(unsigned pin, unsigned value){
	analogData[pin]=value;
}
void Arduino::digitalReadCallback(unsigned port, unsigned data){
	digitalData[port]=data;
}


} /* namespace webworks */
