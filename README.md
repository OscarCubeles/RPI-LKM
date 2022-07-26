# LKM - Raspberry PI
Linux Kernel Module to control multiple interrupt events on a Raspberry PI.


## Table of Contents
1. [General Info](#general-info)
2. [Technologies](#technologies)
3. [Installation](#installation)

## General Info
***
Write down general information about your project. It is a good idea to always put a project status in the readme file. This is where you can add it. 
This project consists of creating a LKM in a Raspberry Pi with a Debian based OS. The LKM controlls the interrupt events generated through 4 buttons connected to the Raspberry PI GPIO's. Each time a button is pressed, a Bash script is executed and the state of the LEDs connected to the RaspberryPI are modified.

## Technologies
***
A list of technologies used within the project:
* [Linux Kernel Programming with C](http://derekmolloy.ie/kernel-gpio-programming-buttons-and-leds/)
* [Bash Scripting]((https://devhints.io/bash))


To run the code of this repository follow the next steps:
· Step 0: Make sure you follow all the previous installation steps from the Report_oscar.cubeles.pdf file.
Step 1: Then, clone the repository on your machine
Step 2: Run the command: make 
Step 3: Run the command make install

After that, the LKM should work as expected in your machine

