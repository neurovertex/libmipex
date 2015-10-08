#include "libmipex.hpp"

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <SerialStream.h>


using namespace std;
using namespace libmipex;


int main (int argc, char *argv[]) {
	string devpath = (argc > 1) ? argv[1] : "/dev/ttyUSB0";


	auto sensor = new MipexSensor(devpath);
	if (argc > 2)
		sensor->setDebugOutput(true);

	while (true) {
		usleep(1000000);
		sensor->refresh();
		cout << sensor->getConcentration() << ", "<< sensor->getMeasure() <<" : "<< sensor->getReference() << endl;
	}

	return 0;
}
