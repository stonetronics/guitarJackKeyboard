# guitarJackKeyboard
...inspired by a post on a 3D printer user group on facebook, unfortunately i cant find the original anymore...

a keyboard that holds two keychains with resistive encoded guitar jacks attached

the enclosure is on thingiverse: https://www.thingiverse.com/thing:3571069

This Arduino Project uses a Franzis IoT Bluetooth Board which is basically a arduino nano with an hc-05 bluetooth module on the same pcb.

Schematic:

- 2 times:
              ________
Arduino 5V --|__100k__|--o--------o-------- Arduino A0/A1
                         |        |
                    82n ===       -----O
                         |           Guitar
                        _|_           Jack
                     Arduino GND  -----O
                                  |
                                 _|_
                             Arduino GND

The 100k Resistor forms a voltage divider with the resistor in the guitar plug and the encoded voltage can be read on the analog channel. The 82n capacitor serves as a filter to minimize contact ripples when pushing in or pulling out the guitar plug.
