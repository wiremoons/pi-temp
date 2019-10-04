/*
 *  MIT License
 *
 *  Copyright (c) 2019 Simon Rowe
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  Written by Simon Rowe <simon@wiremoons.com> on 03 Oct 2019
 *  Source code available from GitHub: https://github.com/wiremoons/rpi-temp.git
 *
 *  Based on firmware documents and code for similar Raspberry Pi utilities
 *  found on the internet here:
 *
 *   - https://github.com/raspberrypi/firmware/wiki
 *   - https://github.com/AndrewFromMelbourne/raspi_serialnumber
 *   - https://www.raspberrypi.org/forums/viewtopic.php?f=33&t=251766
 *
 *  Compile used on Raspbian 'Buster' on Raspberry Pi 4B:
 *  cc -Wall -O3 -pedantic -march=native -mcpu=native -mtune=native rpi-temp.c
 * -o rpi-temp
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/utsname.h> /*  uname()  */
#include <time.h>
#include <unistd.h>

char appversion[] = "1.0.1"; /* set the version of the app here */
int showFahrenheit = 0;      /* output switch for Fahrenheit temperature */

void show_help(char *prog_name); /* function to display help for app usage */

int main(int argc, char **argv)
{
        if (argc > 1) {
                // first check if we were invoked with either -h or --help
                // in which case show basic usage and exit
                if (strcmp(argv[1], "-h") == 0 ||
                    strcmp(argv[1], "--help") == 0) {
                        show_help(argv[0]);
                        return (EXIT_SUCCESS);
                }

                // check if user wants the temperature output displayed
                // in Fahrenheit as default is Celsius
                if (strcmp(argv[1], "-f") == 0 ||
                    strcmp(argv[1], "--Fahrenheit") == 0) {
                        showFahrenheit = 1;
                }
        }

        int fd = open("/dev/vcio", 0);
        if (fd == -1) {
                perror("failed to open: '/dev/vcio'");
                exit(EXIT_FAILURE);
        }

        uint32_t mb_property[10] = {
            0x00000000, 0x00000000, 0x00030006, 0x00000008, 0x00000004,
            0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

        mb_property[0] = 10 * sizeof(mb_property[0]);

        if (ioctl(fd, _IOWR(100, 0, char *), mb_property) == -1) {
                perror("unable to read mailbox values from: '/dev/vcio'");
                exit(EXIT_FAILURE);
        }

        close(fd);

        float pi_temp = 0;
        pi_temp = (float)mb_property[6] / 1000;

        if (showFahrenheit) {
                pi_temp = (pi_temp * 1.8) + 32.0;
        }

        /* obtain host details for output */
        struct utsname uname_buf;
        errno = 0;
        if (uname(&uname_buf) != 0) {
                perror("uname call failure");
                exit(EXIT_FAILURE);
        }

        /* get current time and date for output */
        time_t rawtime = time(NULL);
        if (rawtime == -1) {
                perror("Failure to obtain the current time");
                exit(EXIT_FAILURE);
        }

        /* display acquired information including temperature */
        printf("'%s' %s rel %s (%s) @ %s", uname_buf.nodename,
               uname_buf.sysname, uname_buf.release, uname_buf.machine,
               asctime(localtime(&rawtime)));
        printf("CPU => %.2f %s\n", pi_temp, showFahrenheit ? "°F" : "°C");

        return 0;
}

void show_help(char *prog_name)
{
        printf("\n"
               "rpi-temp version: %s\n\n"
               "Usage: %s [OPTIONS]\n\n"
               "       -h, --help          : display this usage information\n"
               "       -f, --Fahrenheit    : output temperature in Fahrenheit\n"
               "\n"
               "Note: the default temperature is shown in Celsius.\n\n"
               "Source code and MIT license: "
               "https://github.com/wiremoons/rpi-temp\n\n"
               "All is well\n",
               appversion, prog_name);
}
