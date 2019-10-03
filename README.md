[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/hyperium/hyper/master/LICENSE)

# rpi-temp
Small utility to display the CPU temperature of a Raspberry Pi 4B that is written in C.

The inspiration and some source code ideas for the program were obtained from:
- https://github.com/AndrewFromMelbourne/raspi_serialnumber
- https://www.raspberrypi.org/forums/viewtopic.php?f=33&t=251766

## Compile
The program is written in C and uses the normal Linux Raspbian 'Buster' development libraries
that can be installed with the following command:
```
sudo apt -y install build-essential git
``` 

The program source code is in a single file: `rpi-temp.c`. An easy way to download the latest version is to clone this repo from GitHub to your local computer with the command:
```
git clone https://github.com/wiremoons/rpi-temp.git
```

Compiled on Raspbian 'Buster' on Raspberry Pi 4B with command:
```
cc -Wall -O3 -pedantic -march=native -mcpu=native -mtune=native rpi-temp.c -o rpi-temp
```

## Installation
Once compiled, copy the `rpi-temp` binary file somewhere in your path.

## Run Time Options

When run without any option `rpi-temp` displays the current CPU temperature in degrees Celcuis. Exmaple default output is:
```
user@pi4:~$ rpi-temp 
'pi4' Linux rel 4.19.75-v8+ (aarch64) @ Thu Oct  3 22:55:28 2019
CPU => 42.00 °C
```

The program can also display the current temperature in degrees Fahrenheit with the command: `rpi-temp -f`. Example output is:
```
user@pi4:~$ rpi-temp -f
'pi4' Linux rel 4.19.75-v8+ (aarch64) @ Thu Oct  3 22:58:38 2019
CPU => 107.60 °F
```

The options available can be displayed with the command: `rpi-temp -h`. Example out is belowL
```
user@pi4:~$ rpi-temp -h

rpi-temp version: 1.0.0

Usage: rpi-temp [OPTIONS]

       -h, --help          : display this usage information
       -f, --Fahrenheit    : output temperature in Fahrenheit

Note: the default temperature is shown in Celsius.

Source code and MIT license: https://github.com/wiremoons/rpi-temp

All is well
```

## License

The program is licensed with the MIT License.

- http://opensource.org/licenses/mit

