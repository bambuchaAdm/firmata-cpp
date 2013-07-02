/*
 * ArduinoJS.h
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#ifndef ARDUINOJS_H_
#define ARDUINOJS_H_

#include <sstream>
#include <string>
#ifdef QNX
#include "../public/plugin.hpp"
#endif
#include "Arduino.hpp"

class ArduinoJS {

public:
    explicit ArduinoJS(const std::string& id);
    ~ArduinoJS();
    bool CanDelete();
    std::string InvokeMethod(const std::string& command);
    void NotifyEvent(const std::string& event);

private:
    std::string m_id;
    firmata::Arduino *arduino;

    std::string convertInt(unsigned int number);
};

#endif /* ARDUINOJS_H_ */
