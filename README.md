# UFCFVK-15-2 - IoT - Challenge 2

## Overview


## Task Specification
(This information is copied from the Assignement brief found on Blackboard).
Implement a simple communication protocol that transmits data between 2 BBC Micro:bit
devices, using GPIOs.

Your solution should be implemented using C++.

Your solution must be committed in UWE's Gitlab. It will naturally be time stamped and you
must be careful to not make commits after the submission deadline.

The basic protocol, defined below, is Morse code, however, implementing these bare
requirements will enable you to reach a maximum of 70%. To get beyond the 70% your
protocol should be more, for example you might choose to implement the transmission of
characters messages that is encrypted with a simply cypher.

Your solution's source code should follow a coding convention, it should be well
commented, and include a README.md on how to build it and what and how to use your
solution.

## Hardware
The application will only work on a BBC Micro:Bit. The diagram below shows the button 
and pin positions, which may be of use when installing and using the application.
![Micro:Bit Hardware Diagram](https://tech.microbit.org/docs/hardware/assets/microbit-overview-1-5.png)

## Installation of application
### Prerequisites
In order to run this application, you will need the following hardware/software:
- At two BBC Micro:Bits, two cables with crocodile clip attachments at both ends and a Micro USB cable (you can purchase 
one from Amazon, for example, https://tinyurl.com/ycs96p9e)
- A running copy of this virtual machine: https://tinyurl.com/ya59un4m You can get login 
credentials from Blackboard (University of the West of England)

### How-To
1. Open a Terminal window within the VM you've downloaded (see Prerequisites, above).
2. `cd` to the folder you wish to store the code for the game
3. `git clone https://gitlab.uwe.ac.uk/p3-goddard/ufcfvk-15-2-iot-challenge-2.git`
4. `cd ufcfvk-15-2-iot-challenge-2/application_files/`
5. `yt target bbc-microbit-classic-gcc`
6. `yt build`
7. Connect your Micro:Bit via USB to your computer.
8. `cp build/bbc-microbit-classic-gcc/source/iot-morse-code-combined.hex /media/student/MICROBIT`
9. Remove the Micro:Bit and repeat steps 7 and 8 for the second Micro:Bit

10. Connect each Micro:Bit to a power source.
11. Connect one of your cables to the Pin 1 position on both devices.
12. Connect the other cable to the Ground position on both devices.

Now you are ready to send some Morse Code! See App Operation for instructions on how to send and receive messages.

## App Operation
Please ensure that the devices are connected appropriately, as per the above Installation instructions.


## Technical Details
### Class Breakdown
![Class Diagram](https://gitlab.uwe.ac.uk/p3-goddard/ufcfvk-15-2-iot-challenge-2/raw/master/class diagram/ClassDiagram.png)

#### Tree

#### Interface

## Encryption


## References

Lancaster University (2018) *micro:bit runtime.* Available from: https://lancaster-university.github.io/microbit-docs/ [Accessed 11 February 2019].

Micro:bit Educational Foundation (2018) *Hardware.* Available from: Hardware [Accessed 11 February 2019].
