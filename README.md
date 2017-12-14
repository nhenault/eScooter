# eScooter

The base of the project is a simple Razor scooter which has been modified with a TI MSP432P401R, the board’s Booster Pack, a Bluetooth
module for transmitting data to a phone, and an infrared LED and photo-diode which are combined to form a beam break sensor. Using these
peripherals the eScooter tracks total travel distance, velocity, and direction of travel and outputs updated information to a terminal on
a phone every 2 seconds for readability. A block diagram of the system can be seen below, and the components enclosed by the box represent
both hardware and software features implemented within the MSP432.
