LibMipex
========

This is a small library for reading data from a [Mipex-02](http://mipex-tech.com/en/infrared-sensors/mipex-02-infrared-gas-sensors.html) IR gas sensor. It requires libserial.

For now it is not functional unless run through interceptty - trying to figure it out.

Usage: `./bin/main [/path/to/serial/device (default:/dev/ttyUSB0)] [debug]` (The value of the second argument doesn't matter. Adding anything after the first will enable debug output.)
