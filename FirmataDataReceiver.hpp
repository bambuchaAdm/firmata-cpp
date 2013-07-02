/*
 * FirmataDataReceiver.h
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#ifndef FIRMATADATARECEIVER_H_
#define FIRMATADATARECEIVER_H_

namespace firmata {
class FirmataDataReceiver {
public:
	virtual void analogReadCallback(unsigned pin, unsigned value) = 0;
	virtual void digitalReadCallback(unsigned port, unsigned data) = 0;
	virtual ~FirmataDataReceiver(){};
};
}
#endif /* FIRMATADATARECEIVER_H_ */
