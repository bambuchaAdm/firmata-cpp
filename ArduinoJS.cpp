/*
 * ArduinoJS.cpp
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#include "ArduinoJS.hpp"
#include "Arduino.hpp"
#include "ADKConnection.hpp"
#include <string>
#include <stdlib.h>
#ifdef QNX
#include "../public/tokenizer.hpp"
#endif
using namespace std;
ArduinoJS::ArduinoJS(const std::string& id) :
		m_id(id) {
	arduino = new firmata::Arduino(
			(firmata::Connection *) new firmata::ADKConnection(), this);
}

ArduinoJS::~ArduinoJS() {
	if (arduino)
		delete arduino;
}

char* onGetObjList() {
	static char name[] = "ArduinoJS";
	return name;
}
#ifdef QNX
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "ArduinoJS") {
		return new ArduinoJS(id);
	}
	return NULL;
}
#endif
bool ArduinoJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
std::string ArduinoJS::InvokeMethod(const std::string& command) {
	unsigned int arg1=0, arg2=0;
	size_t index = command.find_first_of(" ");
	std::string strCommand = command.substr(0, index);
	std::string arg = command.substr(index + 1, command.length());
	index = arg.find_first_of(" ");
	if(index == std::string::npos){
		arg1 = atoi(arg.c_str());
	}else{
		arg1 = atoi(arg.substr(0, index).c_str());
		arg2 = atoi(arg.substr(index+1, arg.length()).c_str());
	}

	if (strCommand == "pinMode") {
		arduino->pinMode(arg1, arg2);
	} else if (strCommand == "digitalWrite") {
		arduino->digitalWrite(arg1, arg2);
	} else if (strCommand == "digitalRead") {
		return convertInt(arduino->digitalRead(arg1));
	} else if (strCommand == "analogWrite") {
		arduino->analogWrite(arg1, arg2);
	} else if (strCommand == "analogRead") {
		return convertInt(arduino->analogRead(arg1));
	}

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}
#ifdef QNX
// Notifies JavaScript of an event
void ArduinoJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
#endif
std::string ArduinoJS::convertInt(unsigned int number) {
	std::stringstream ss; //create a stringstream
	ss << number; //add number to the stream
	return ss.str(); //return a string with the contents of the stream
}
