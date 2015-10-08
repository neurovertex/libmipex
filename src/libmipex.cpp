#include "libmipex.hpp"

#include <string.h>
#include <iomanip>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>

#define PRINTHEX(val)	setfill('0') << setw(sizeof(val)*2) << hex << ((long)val) << dec

using namespace std;
using namespace LibSerial;

namespace libmipex {
	const char* const MipexSensor::_commands[] = {"DATA", "DATA3", "DATAE2"};
	const char* const MipexSensor::_status[] = {""};

	MipexSensor::MipexSensor(string devpath) {
		if (!check_dev_path(devpath))
			throw "Invalid devpath";

		this->path = devpath;
		this->debugout_ = false;
		this->sensor.SetBaudRate(SerialStreamBuf::BAUD_9600);
		this->sensor.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
		this->sensor.SetNumOfStopBits(1);
		this->sensor.SetParity(SerialStreamBuf::PARITY_NONE);
		this->sensor.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);


		this->last_concentration = this->last_um = this->last_ur = this->last_status = 0;

		cout << "Connecting to "<< devpath << endl;
		this->sensor.Open(devpath);
	}

	MipexSensor::~MipexSensor() {
		this->sensor.Close();
		cout << "Exitting" << endl;
	}

	void MipexSensor::sendCommand(string command) {
		this->sensor << command << '\r' << flush;
	}

	long MipexSensor::readASCIIInt() {
		int val = 0;
		bool neg = false;

		if (!isdigit(this->last_char) && this->last_char != '-') {
			if (this->debugout_)
				cout << "Not a digit : " << PRINTHEX(this->last_char) << endl;
			this->readNext();
		}

		if (this->last_char == '-') {
			neg = true;
			this->readNext();
		}

		while (isdigit(this->last_char)) {
			val = 10*val + (this->last_char-'0');
			this->readNext();
		}
		return neg ? -val : val;
	}

	long MipexSensor::readHexInt(int len) {
		long val = 0;
		for (int i = 0; i < len; i ++) {
			val = (val<<8) + this->readNext();
		}
		return val;
	}

	void MipexSensor::refresh() {
		this->sendCommand("F");
		this->readNext();
		do {
			if (this->debugout_) {
				cout << "Term : \t" << this->readASCIIInt() << endl;
				cout << "S : \t" << this->readASCIIInt() << endl;
				cout << "Ur : \t" << (this->last_ur = this->readASCIIInt()) << endl;
				cout << "Um : \t" << (this->last_um = this->readASCIIInt()) << endl;
			} else {
				this->readASCIIInt();
				this->readASCIIInt();
				this->last_ur = this->readASCIIInt();
				this->last_um = this->readASCIIInt();
			}
			while (last_char != '\r') {
				this->readNext();
				if (isdigit(last_char)) {
					auto val = this->readASCIIInt();
					if (this->debugout_)
						cout << "Value : "<< val << endl;
				}
			}
		} while (last_char != '\r');
	}

	std::int32_t MipexSensor::getConcentration() {
		return this->last_concentration;
	}

	std::int32_t MipexSensor::getMeasure() {
		return this->last_um;
	}

	std::int32_t MipexSensor::getReference() {
		return this->last_ur;
	}

	std::int32_t MipexSensor::getStatus() {
		return this->last_status;
	}

	void MipexSensor::setDebugOutput(bool debug) {
		this->debugout_ = debug;
	}

	char MipexSensor::readNext() {
		this->sensor.get(this->last_char);
		if (this->debugout_)
			cout << "read "<< PRINTHEX((int8_t)(this->last_char)) << endl;
		return this->last_char;
	}


	bool check_dev_path(string path) {
		struct stat s;
		if (stat(path.c_str(), &s) != 0) {
			cerr << path <<" : "<< strerror(errno) << endl;
			return false;
		} else if (!S_ISCHR(s.st_mode)) {
			cerr << path << " : not a character device" << endl;
			return false;
		}
		return true;
	}

}
