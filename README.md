# LKM - Raspberry PI
Linux Kernel Module to control multiple interrupt events on a Raspberry PI.


## Table of Contents
1. [General Info](#general-info)
2. [Technologies](#technologies)
3. [Installation](#installation)

## General Info
***
This University project consists of creating a LKM in a Raspberry Pi with a Debian based OS. The LKM controls the interrupt events generated through 4 buttons connected to the Raspberry PI GPIO's. Each time a button is pressed, a Bash script is executed, and the state of the LEDs connected to the Raspberry PI are modified.

## Technologies
***
A list of technologies used within the project:
* [Linux Kernel Programming with C](http://derekmolloy.ie/kernel-gpio-programming-buttons-and-leds/)
* [Bash Scripting](https://devhints.io/bash)

## Installation
***
A little intro about the installation of the LKM in a RaspberryPI with a Raspberry PI OS installed. 
```
$ git clone https://github.com/OscarCubeles/RPI-LKM.git
$ make
$ make install
```
After that, the LKM should work as expected in your machine

Note: To use the application in a Raspberry PI, a [Raspberry PI OS](https://www.raspberrypi.com/software/operating-systems/) should be used.



