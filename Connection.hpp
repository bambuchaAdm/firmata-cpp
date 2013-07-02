/*
 * Connection.h
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

typedef unsigned char byte;

namespace firmata{


class Connection {
public:
	virtual ~Connection(){}

	virtual bool connect() = 0;
	virtual bool isConnected() = 0;
	virtual bool disconnect() = 0;

	virtual void writeByte(byte data) = 0; //non-blocking
	virtual byte readByte() = 0; //blocking
};

}
#endif /* CONNECTION_H_ */
