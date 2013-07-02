/*  Serial port object for use with Firmata library
 *  Copyright 2013, Jakub Kramarz (lenwe@lenwe.net)
 *  Copyright 2010, Paul Stoffregen (paul@pjrc.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ADKConnection.hpp"
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <string>
#include <cstring>
namespace firmata {
	using std::fstream;
	using std::string;
	ADKConnection::ADKConnection() {
	}


	void ADKConnection::Set_baud()
	{
	}

	bool ADKConnection::connect(){
	
		
		std::string error_msg;
		#ifdef QNX
			std::string name = "/dev/aap0";
			adk = open("/dev/aap0", O_RDWR | O_NOCTTY | O_NONBLOCK);
		#else
			std::string name = "/dev/ttyACM0";
			adk = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NONBLOCK);
		#endif
		
		
		
		struct serial_struct kernel_serial_settings;
		int bits;
		if (adk < 0) {
			if (errno == EACCES) {
				error_msg = "Unable to access " + name + ", insufficient permission";
				// TODO: we could look at the permission bits and owner
				// to make a better message here
			} else if (errno == EISDIR) {
				error_msg = "Unable to open " + name +
					", Object is a directory, not a serial port";
			} else if (errno == ENODEV || errno == ENXIO) {
				error_msg = "Unable to open " + name +
					", Serial port hardware not installed";
			} else if (errno == ENOENT) {
				error_msg = "Unable to open " + name +
					", Device name does not exist";
			} else {
				error_msg = "Unable to open " + name;
			}
			return false;
		}
		if (ioctl(adk, TIOCMGET, &bits) < 0) {
			close(adk);
			error_msg = "Unable to query serial port signals";
			return -1;
		}
		bits &= ~(TIOCM_DTR | TIOCM_RTS);
		if (ioctl(adk, TIOCMSET, &bits) < 0) {
			close(adk);
			error_msg = "Unable to control serial port signals";
			return -1;
		}
		if (tcgetattr(adk, &settings_orig) != 0) {
			close(adk);
			error_msg = "Unable to query serial port settings (perhaps not a serial port)";
			return -1;
		}
		memset(&settings, 0, sizeof(settings));
		settings.c_iflag = IGNBRK | IGNPAR;
		settings.c_cflag = CS8 | CREAD | HUPCL | CLOCAL;
		speed_t spd = B57600;
		cfsetospeed(&settings, spd);
		cfsetispeed(&settings, spd);
		tcsetattr(adk, TCSANOW, &settings);
		if (ioctl(adk, TIOCGSERIAL, &kernel_serial_settings) == 0) {
			kernel_serial_settings.flags |= ASYNC_LOW_LATENCY;
			ioctl(adk, TIOCSSERIAL, &kernel_serial_settings);
		}
		tcflush(adk, TCIFLUSH);
		sleep(1); //TODO: It's a kind of magic!

		return isConnected();
	}
	bool ADKConnection::disconnect(){
		close(adk);
		return !isConnected();
	}

	void ADKConnection::writeByte(byte data){
		write(adk, &data, sizeof(byte));
		tcflush(adk, TCOFLUSH);
	}
	byte ADKConnection::readByte(){
		char data;
		read(adk, &data, 1);
		return (byte) data;
	}
	bool ADKConnection::isConnected(){
		return adk > 0;
	}

	ADKConnection::~ADKConnection() {
		disconnect();
	}

} /* namespace webworks */
