/*
 * Firmata.cpp
 *
 *  Created on: 13-03-2013
 *      Author: Jakub Kramarz (http://lenwe.net)
 */

#include "Firmata.hpp"

namespace firmata {

Firmata::Firmata(Connection * connection) {
	if (!connection->isConnected())
		connection->connect();
	this->connection = connection;
}

Firmata::~Firmata() {
	connection->disconnect();
	delete connection;
}

void Firmata::analogWrite(byte pin, byte value) {
	writeByte(ANALOG_MESSAGE + pin);
	sendValueAsTwoBytes(value);
}
void Firmata::digitalWrite(byte port, byte value) {
	writeByte(DIGITAL_MESSAGE + port);
	sendValueAsTwoBytes(value);
}
void Firmata::setPinMode(byte pin, byte state) {
	writeByte(SET_PIN_MODE);
	writeByte(pin);
	writeByte(state);
}
void Firmata::setReportAnalog(byte pin, bool state) {
	writeByte(REPORT_ANALOG + pin);
	writeByte(state);
}
void Firmata::setReportDigital(byte port, bool state) {
	writeByte(REPORT_DIGITAL + port);
	writeByte(state);
}
void Firmata::systemReset() {
	writeByte(SYSTEM_RESET);
}
void Firmata::reportVersion() {
	writeByte(REPORT_VERSION);
}

void * Firmata::thread(void * parent){
	Firmata * firmata =  static_cast<Firmata*>(parent);
	Connection * connection = firmata->connection;
	FirmataDataReceiver * receiver = firmata->receiver;
	while(!firmata->isThreadHalt() and connection->isConnected()){
		byte data = connection->readByte();

		{
			byte temp = data;
			char al[] = "0123456789ABCDEF";
			for(int i = 0; i < 4; i++){
				byte c = (temp & 0xC0) >> 6;
				temp = temp << 2;
				std::cout << al[(int)c];
			}
			std::cout << std::endl;
		}
		//TODO
		if(data < START_SYSEX){
			byte command = data & START_SYSEX;
			byte channel = data & 0x0F;
		}
	}
	return NULL;
}

bool Firmata::startThread(FirmataDataReceiver * receiver) {
	if (!m_thread) {
		byte rc;
		rc = pthread_mutex_lock(&mutex);
		threadHalt = false;
		rc = pthread_cond_signal(&cond);
		rc = pthread_mutex_unlock(&mutex);

		pthread_attr_t thread_attr;
		pthread_attr_init(&thread_attr);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

		pthread_create(&m_thread, &thread_attr, thread, receiver);
		pthread_attr_destroy(&thread_attr);
		return false;
	} else {
		return true;
	}
}
void Firmata::stopThread() {
	byte rc;
	// Request thread to set prevent sleep to false and terminate
	rc = pthread_mutex_lock(&mutex);
	threadHalt = true;
	rc = pthread_cond_signal(&cond);
	rc = pthread_mutex_unlock(&mutex);

	// Wait for the thread to terminate.
	void *exit_status;
	rc = pthread_join(m_thread, &exit_status);

	// Clean conditional variable and mutex
	pthread_cond_destroy (&cond);
	pthread_mutex_destroy (&mutex);

	m_thread = 0;
	threadHalt = true;
}
bool Firmata::isThreadHalt() {
	byte rc;
	bool isThreadHalt;
	rc = pthread_mutex_lock(&mutex);
	isThreadHalt = threadHalt;
	rc = pthread_mutex_unlock(&mutex);
	return isThreadHalt;
}

//Sysex messages
void Firmata::analogExtendedWrite(byte pin, byte value) {
	startSysex();
	writeByte(EXTENDED_ANALOG);
	writeByte(pin);
	sendValueAsTwoBytes(value);
	endSysex();
}
void Firmata::capabilitiesQuery() {
	startSysex();
	writeByte(CAPABILITY_QUERY);
	endSysex();
}
void Firmata::setSamplinginterval(byte interval_ms) {
	startSysex();
	writeByte(SAMPLING_INTERVAL);
	sendValueAsTwoBytes(interval_ms);
	endSysex();
}
void Firmata::servoConfig(byte pin, byte minpulse, byte maxpulse) {
	startSysex();
	writeByte(SERVO_CONFIG);
	writeByte(pin);
	sendValueAsTwoBytes(minpulse);
	sendValueAsTwoBytes(maxpulse);
	endSysex();
}
void Firmata::startSysex() {
	writeByte(START_SYSEX);
}
void Firmata::endSysex() {
	writeByte(END_SYSEX);
}
void Firmata::writeByte(byte data) {
	connection->writeByte((byte) data);
}
byte Firmata::readByte() {
	return (byte) connection->readByte();
}
void Firmata::sendValueAsTwoBytes(byte value) {
	byte lsb = value & 0x7F;
	byte msb = (value & 0x80) >> 7;
	writeByte(lsb);
	writeByte(msb);
}

} /* namespace webworks */
