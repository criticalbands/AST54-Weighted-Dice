// set to 1 if we're debugging
#define DEBUG 0

// 6 consecutive digital pins for the LEDs
int first = 2;
int second = 3;
int third = 4;
int fourth = 5;
int fifth = 6;

int ORout = 7;
int ORpin1 = 8;
int ORpin2 = 9;
int ORpin3 = 10;
int ORpin4 = 11;
int ORpin5 = 12;
int OrStatus1 = 0;
int OrStatus2 = 0;
int OrStatus3 = 0;
int OrStatus4 = 0;
int OrStatus5 = 0;

int potpin1 = A7;  // analog pin used to connect the potentiometer
int val;

// pin for the button switch
int button = 13;
// value to check state of button switch
int pressed = 0;

void setup() {
  pinMode(ORout, OUTPUT);
  pinMode(ORpin1, INPUT);
  pinMode(ORpin2, INPUT);
  pinMode(ORpin3, INPUT);
  pinMode(ORpin4, INPUT);
  pinMode(ORpin5, INPUT);

  // set all LED pins to OUTPUT
  for (int i = first; i <= fifth; i++) {
    pinMode(i, OUTPUT);
  }
  // set buttin pin to INPUT
  pinMode(button, INPUT);

  // initialize random seed by noise from analog pin 0 (should be unconnected)
  randomSeed(analogRead(0));

  // if we're debugging, connect to serial
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void buildUpTension() {
  // light LEDs from left to right and back to build up tension
  // while waiting for the dice to be thrown
  // left to right
  for (int i = first; i <= fifth; i++) {
    if (i != first) {
      digitalWrite(i - 1, LOW);
    }
    digitalWrite(i, LOW);
    delay(val);
  }
  // right to left
  for (int i = fifth; i >= fifth; i--) {
    if (i != fifth) {
      digitalWrite(i + 1, LOW);
    }
    digitalWrite(i, LOW);
    delay(val);
  }
}

void showNumber(int number) {
  digitalWrite(first, HIGH);
  if (number >= 2) {
    digitalWrite(second, HIGH);
  }
  if (number >= 3) {
    digitalWrite(third, HIGH);
  }
  if (number >= 4) {
    digitalWrite(fourth, HIGH);
  }
  if (number >= 5) {
    digitalWrite(fifth, HIGH);
  }
}

int throwDice() {
  // get a random number in the range [1,6]
  int randNumber = random(1, 6);

#ifdef DEBUG
  Serial.println(randNumber);
#endif
  return randNumber;
}

void setAllLEDs(int value) {
  for (int i = first; i <= fifth; i++) {
    digitalWrite(i, value);
  }
}

void loop() {
  OrStatus1 = digitalRead(ORpin1);
  OrStatus2 = digitalRead(ORpin2);
  OrStatus3 = digitalRead(ORpin3);
  OrStatus4 = digitalRead(ORpin4);
  OrStatus5 = digitalRead(ORpin5);

  if (OrStatus1 == HIGH || OrStatus2 == HIGH || OrStatus3 == HIGH || OrStatus4 == HIGH || OrStatus5 == LOW )
  { digitalWrite(ORout, HIGH);
  } else {
    digitalWrite(ORout, LOW);
  }

  val = analogRead(potpin1);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 170);
  delay(val);
  // if button is pressed - throw the dice
  pressed = digitalRead(button);

  if (pressed == HIGH) {
    // remove previous number
    setAllLEDs(LOW);

    buildUpTension();
    int thrownNumber = throwDice();
    showNumber(thrownNumber);
  }
}
