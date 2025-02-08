# ELS---Electronic-Lead-Screw
Electronic lead screw for any type of lathe, using ARDUINO UNO

Please read the Manual and instruction in the PDF file. Language is only Italian at the moment.
Working on English version of the release and firmware

This is an ELS (Electronic Lead Screw)which can be mounted on any type of lathe.
it's an automatic system to syncronize the spindle rotation with the lead screw rotation for threading and feeding
It's build for lathes with or without the feeding bar which can be configured with 4 different feeding rates.

what you need:
- 1 Arduino UNO
- 1 incremental encoder with quadrature output (A, B) - index (Z) is not supported
- 1 joystick (2 analog axis) with 3 buttons
- 1 20x4 LCD display
- 1 stepper motor suitable for driving the leadscrew/feed bar
- 1 stepper motor driver
- 1 arduino power supply (12V)
- 1 power supply for the stepper motor

Configuration:
- encoder steps/turn
- leadscrew steps/turn
- leadscrew pitch
- feed bar pitches (4 different values - optional)

Capabilities:
- threading with metric and imperial pitches (0.1mm to 2x screw pitch, with 0.05mm thread pitch resolution)
- threading with MODULE pitch (metric only)
- right-hand or left-hand threading option
- threading at a given lenght available
- feeding with cent/turn or mm/min option
- spindle Angular degree with resolution given by the encoder step/turn
- spindle rpm reading

Detailed instructions and block diagram of the system is available in the PDF documents

The code is written in C++ and it's for Arduino UNO only. Arduino IDE is needed to compile and load the sketch on the Arduino UNO board.

Discussion forum available : https://meccanicaedintorni.net/viewtopic.php?t=29866
Forum language is italian only. Registration and presentaton are mandatory to post on the forum
