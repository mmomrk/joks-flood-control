//inputs
#define PIN_LOW_FLOAT 3
#define PIN_UP_FLOAT  4
//outputs
#define PIN_LED   13  //arduino LED is linked to pin 13
#define PIN_RELAY 5

long const RELAY_SWITCH_DELAY = 10000;//ms  //final kind of

long prevBlinkSwitchTime = 0;//ms. Used with millis()
long prevRelaySwitchTime = 0;//ms
long blinkFlipTime = 0; //ms - used to blink error codes

int errState = 0;

//method to set output pin to given state
void setRelay(bool onNotOff) {
  long curTime = millis();
  if (relayIsOn() != onNotOff) {  //switching is required
    if (curTime - prevRelaySwitchTime < RELAY_SWITCH_DELAY) { //too frequent switching
      return;
    }
  }
  prevRelaySwitchTime = curTime;
  if (onNotOff) { //for the sake of readability. Could have been one-line
    digitalWrite(PIN_RELAY, HIGH);
  } else {
    digitalWrite(PIN_RELAY, LOW);
  }
  delay(100); //for safety
}

bool relayIsOn() {
  return digitalRead(PIN_RELAY);
}


//I have two floats: one is angular and one straight.
//Angular breaks connection when float is in the water. Straight breaks connection when float is hung freely.
//I am using two angular floats in this proj. May require flipping outputs
bool lowerSubmerged() {
  return !digitalRead(PIN_LOW_FLOAT);
}

bool upperSubmerged() {
  return !digitalRead(PIN_UP_FLOAT);
}


//A method to flip LED which indicates errorstate if any.
//Designed to keep blinking once error occured
void pollBlink() {
  if (blinkFlipTime > 0) {
    long curTime = millis();
    if (curTime - prevBlinkSwitchTime > blinkFlipTime) {
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      prevBlinkSwitchTime = curTime;
    }
  }
  if (errState != 0) {
    switch (errState) {
      case 1:
        blinkFlipTime = 1000;
        break;
      case 2:
        blinkFlipTime = 100;
        break;
      case 3:
        blinkFlipTime = 3000;
        break;
    }
  }
}

//return number of error state
int checkErrorStates(bool relayStOn,bool lowerStSubm, bool upperStSubm) {
  if (!lowerStSubm && upperStSubm) { //inverted gravitational responce water?
    return (1);
  }
  if ( relayStOn && !lowerStSubm) { //motor is running dry. Very bad
    setRelay(false);  //try to save situation
    return (2);
  }
  if ( !relayStOn && upperStSubm ) { //water is high but motor is off
    return (3);
  }
  return 0;
}

void setup() {
  pinMode(PIN_UP_FLOAT, INPUT_PULLUP);
  pinMode(PIN_LOW_FLOAT, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
}

void loop() {
  bool relayStOn = relayIsOn();
  bool lowerStSubm = lowerSubmerged();
  bool upperStSubm = upperSubmerged();
  if (relayStOn && !lowerStSubm) {
    setRelay(false);
  }
  if (!relayStOn && upperStSubm) {
    setRelay(true);
  }
  relayStOn = relayIsOn();
  lowerStSubm = lowerSubmerged();
  upperStSubm = upperSubmerged();
  errState = checkErrorStates(relayStOn, lowerStSubm, upperStSubm);
  pollBlink();
}
