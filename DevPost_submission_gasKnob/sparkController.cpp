
int green = D3;
int amber = D1;
int red = D0;

int outputpin = A1;

Servo servo;  //  create  a new Servo object
int servoPin  = D2;  //  a PWM pin

#define SERVO_DELAY 15

void setup()
{
  pinMode(green, OUTPUT);
  pinMode(amber, OUTPUT);
  pinMode(red, OUTPUT);
  Spark.function("led", sparkController);
  // servo.attach(servoPin);
}

typedef enum {
  off,
  simmer,
  medium,
  full
} flameLevel;

void loop()
{
  int rawvoltage = analogRead(outputpin);
  float millivolts = (rawvoltage / 1024.0) * 5000;
  float fahrenheit = millivolts / 10;
  fahrenheit = 48.5;
  int c = (fahrenheit - 32) / 1.8;
  c = c / 10;

  // Trigger the integration
  String data = String(c);
  Particle.publish("temp", data, PRIVATE);
  // Wait 60 seconds
  delay(10000);
  blinkAmber();
}


int sparkController(String command) {
  blinkAmber();
  if (command == "eggs") {
    eggs();
    Particle.publish("office-motion", "Eggs are boiled! Have some protien :)", 60, PRIVATE);
  } else if (command == "fries") {
    fries();
    Particle.publish("office-motion", "Fries are done! Grab some ketchup, I cant do that for you yet :D", 60, PRIVATE);
  } else if (command == "milk") {
    milk();
    Particle.publish("office-motion", "Milk is boiled! Get a glass, I cant do that for you yet :D", 60, PRIVATE);
  } else if (command == "water") {
    water();
    Particle.publish("office-motion", "Boiling water is done!", 60, PRIVATE);
  } else if (command == "rice") {
    rice();
    Particle.publish("office-motion", "Rice is done! :) Are you making sushi ?! :D ", 60, PRIVATE);
  }
}


void resetFlameIndicator() {
  digitalWrite(green, LOW);
  digitalWrite(amber, LOW);
  digitalWrite(red, LOW);
}

void flameIndicator(int mode) {
  resetFlameIndicator();
  /*Indicates Simmer flame*/
  if (mode == 1) {
    digitalWrite(green, HIGH);
  } else if (mode == 2) {
    /*Indicates medium flame*/
    digitalWrite(green, HIGH);
    digitalWrite(amber, HIGH);
  } else if (mode == 3) {
    /*Indicates high flame*/
    digitalWrite(green, HIGH);
    digitalWrite(amber, HIGH);
    digitalWrite(red, HIGH);
  }
}

void blinkAmber() {
  digitalWrite(amber, HIGH);
  delay(1500);
  digitalWrite(amber, LOW);
  delay(1500);
}

int knobPosition = off;

void simmerFlame() {
  int angle = 0;
  servo.attach(servoPin);
  if (knobPosition == off) {
    for (angle = 0; angle < 45; angle++)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if (knobPosition == simmer) {
    /*Do Nothing*/
  } else if (knobPosition == medium) {
    for (angle = 90; angle > 45; angle--)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if ( knobPosition == full) {
    for (angle = 180; angle > 45; angle--)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  }
  knobPosition = simmer;
  servo.detach();
}

void mediumFlame() {
  int angle = 0;
  servo.attach(servoPin);
  if (knobPosition == off) {
    for (angle = 0; angle < 90; angle++)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if (knobPosition == medium) {
    /*Do Nothing*/
  } else if (knobPosition == simmer) {
    for (angle = 45; angle < 90; angle++)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if ( knobPosition == full) {
    for (angle = 180; angle > 90; angle--)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  }
  knobPosition = medium;
  servo.detach();
}

void fullFlame() {
  int angle = 0;
  servo.attach(servoPin);
  if (knobPosition == off) {
    for (angle = 0; angle < 180; angle++)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if (knobPosition == full) {
    /*Do Nothing*/
  } else if (knobPosition == simmer) {
    for (angle = 45; angle < 180; angle++)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if ( knobPosition == medium) {
    for (angle = 90; angle < 180; angle++)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  }
  knobPosition = full;
  servo.detach();
}

void turnOff() {
  int angle = 0;
  servo.attach(servoPin);
  if (knobPosition == off) {
    /*Do Nothing*/
  } else if (knobPosition == medium) {
    for (angle = 90; angle > 0; angle--)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if (knobPosition == full) {
    for (angle = 180; angle > 0; angle--)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  } else if (knobPosition == simmer) {
    for (angle = 45; angle > 0; angle--)
    {
      servo.write(angle);
      delay(SERVO_DELAY);
    }
  }
  knobPosition = off;
  servo.detach();
}


void eggs()
{
  fullFlame();
  flameIndicator(3);

  delay(10000);

  mediumFlame();
  flameIndicator(2);

  delay(5000);

  turnOff();
  resetFlameIndicator();
}

void fries()
{
  simmerFlame();
  flameIndicator(1);

  delay(10000);

  mediumFlame();
  flameIndicator(2);

  delay(5000);

  fullFlame();
  flameIndicator(3);

  delay(5000);

  turnOff();
  resetFlameIndicator();
}

void milk()
{

  mediumFlame();
  flameIndicator(2);
  delay(10000);

  simmerFlame();
  flameIndicator(1);
  delay(5000);

  turnOff();
  resetFlameIndicator();

}

void water()
{
  fullFlame();
  flameIndicator(3);

  delay(15000);

  turnOff();
  resetFlameIndicator();

}

void rice() {

  fullFlame();
  flameIndicator(3);

  delay(15000);

  mediumFlame();
  flameIndicator(2);
  delay(5000);

  turnOff();
  resetFlameIndicator();
}

