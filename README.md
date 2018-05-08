# Joks-flood-control

This code is compatible with Arduino UNO and is supposed to control water pump in the building basement.

## Cirquit

Arduino is connected to two float binary sensors. One is called lower and another one is upper. Mine had "AMP-X221" label on the package. 
Sensors are attached to GND with one wire and to digital 3 and 4 with another wire. 

Another connection is solid-state relay module capable of using arduino outpus as control input. For example HHG1-1032F-38100Z.
Its input '+' is connected to digital 4.

### Inputs

#define PIN_LOW_FLOAT 3
#define PIN_UP_FLOAT  4

### Outputs

#define PIN_LED   13  //arduino LED is linked to pin 13
#define PIN_RELAY 5


## Logic

If lower and upper floats are triggered then switch motor on. If both are untriggered then switch motor off. Minimal time between motor switches is set via
long const RELAY_SWITCH_DELAY = 10000;//ms  
so that motor is not flipped for short periods if it happens.

### Error messages

LED on the board is coded to display error state of the system:

int checkErrorStates(bool relayStOn,bool lowerStSubm, bool upperStSubm) {
  if (!lowerStSubm && upperStSubm) { //inverted gravitational responce water?
    return (1);
  }
  if ( relayStOn && !lowerStSubm) { //motor is running dry. Very bad
    setRelay(false);  //try to save situation. May bump into RELAY SWITCH DELAY rule but by that time things are messy already.
    return (2);
  }
  if ( !relayStOn && upperStSubm ) { //water is high but motor is off
    return (3);
  }
  return 0;
}

By the time this readme is written errors are triggered without a good reason for that. WIP


## License

MIT licence.

Copyright (c) 2018 Mark Bochkov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


## Be nice
