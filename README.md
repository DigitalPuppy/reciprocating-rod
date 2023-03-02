# Reciprocating rod: motion controled linear actuator device

## How it works:

A distance sensor mesure the distance between your buttock and the rod base.
The rod is then moved according to this distance to reciprocate your motions.

## Required parts

You'll need the following parts:

    - a computer running GNU/Linux
    - an arduino (tested with UNO), and USB to USB-B cable to connect it.
    - a grove Time-of-flight distance sensor (vl53l0x), with a grove-to-wire connector
    - a linear actuator of a famous brand, and another USB to USB-B cable to connect it.

## Setup

In arduino IDE add the "grove ToF sensor" to your libraries.

Then compile and upload the 'tof/tof.ino' firmware in this repository.

Wire the sensor to the arduino: 

    - Red to 5V
    - Black to ground
    - White to SDA
    - Yellow to SCL

Scotch or hotglue or whatever the sensor to the base of the rod, ensuring its direction is aligned with the rod.

Compile the program:

    g++ -o reciprocate tof-to-rod.cc

Then connect both devices to USB ports on your computer. Make note of the tty device names (check 'dmesg' or ls /dev/ttyUS* /ttyACM*).

Finally run the program with two arguments in that order:

   - device for arduino
   - device for actuator

# Adjustments

Edit the .cc file to adjust the reciprocating action: rod0 and tof0 are the matching positions in milimeters. Factor define the amplitude of the reciprocating action.

Enjoy!
