# Ambient Led Project

Code for ambient LED project to be installed in the EE Lounge. We will use an ESP 32 microcontroller to communicate to the LED strip and host a small web interface to control effects/upload new code.

## Components

### ESP32
The Esp 32 is a microcontroller developed by Espressif, which is wifi and bluetooth enabled. Since it has two cores, network and realtime tasks can be handled without an issue. Specifically, we will be using the NodeMCU-32s board. To start with, we will flash it over USB like a normal arduino, but later on we will make it pull the firmware directly from github.

### WS2812 LED Strip
These addressable LEDs can be controlled individually through a one wire connection. This means that we can make the entire strip do cool effects using a simple microcontroller, no need to worry about power transistors or anything like that! Each LED package has its own integrated RGB controller and drives the LED component by itself.

## Development
There are a number of frameworks we could use for our program. The main ones (with better library support and documentation) are ESP-IDF and Arduino. The ESP-IDF has advantages since it allows us to get closer to hardware, at the expense of less portability. We will start with Arduino and if we encounter some limitations, move some of the critical code over to ESP-IDF.

In order to easily set up the development environment, we suggest using either Atom or Visual Studio Code together witht the PlatformIO plugin. Any of these two will work, and the set up should be similar. If you are able to come on Thursday with the that up and running, it would be great!

Refer to this link to install it: https://platformio.org/install/ide?install=vscode
