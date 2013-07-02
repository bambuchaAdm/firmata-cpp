/*
 * ADKConnection.h
 *
 *  Created on: 14-03-2013
 *      Author: lenwe
 */

#ifndef ADKCONNECTION_H_
#define ADKCONNECTION_H_

#include <fstream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "Connection.hpp"

namespace firmata{
	class ADKConnection : public Connection {
	public:
		ADKConnection();
		~ADKConnection();

		bool connect();
		bool isConnected();
		bool disconnect();

		void writeByte(byte data); //non-blocking
		byte readByte(); //blocking
	private:
		int adk;
		void Set_baud();
		struct termios settings_orig;
		struct termios settings;
	};

}
#endif /* ADKCONNECTION_H_ */
