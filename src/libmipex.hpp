#ifndef MIPEX_HPP_INCLUDED
#define MIPEX_HPP_INCLUDED

#include <SerialStream.h>
#include <iostream>
#include <cstdint>

namespace libmipex {
	typedef enum _mipexcommand {
		DATA, DATAE, DATAE2
	} MipexCommand;

	class MipexSensor
	{

	public:
		MipexSensor(std::string path);
		~MipexSensor();
		void sendCommand(std::string command);
		long readASCIIInt();
		long readHexInt(int len);
		void refresh();
		std::int32_t getConcentration();
		std::int32_t getMeasure();
		std::int32_t getReference();
		std::int32_t getStatus();
		void setDebugOutput(bool debug);

		static const char * const _commands[];
		static const char * const _status[];

	private:
		std::string path;
		std::uint32_t last_concentration, last_status, last_um, last_ur;
		LibSerial::SerialStream sensor;
		bool debugout_;
		char last_char;
		char readNext();
	};

	bool check_dev_path(std::string path);
}

#endif
