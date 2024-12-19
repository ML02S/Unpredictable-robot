#include <Servo.h>

Servo leftServo;
Servo rightServo;

const int leftServoPin = 9;
const int rightServoPin = 10;

const int LEFT_FORWARD_SPEED = 30;   // Pas dit aan voor linker servo snelheid
const int LEFT_BACKWARD_SPEED = 150;
const int RIGHT_FORWARD_SPEED = 165; // Pas dit aan voor rechter servo snelheid
const int RIGHT_BACKWARD_SPEED = 15;
const int STOP_SPEED = 90;            // Stoppositie

// Bepaalt de kans dat de robot het commando volgt
const float FOLLOW_COMMAND_PROBABILITY = 0.2; // 20% kans om te luisteren

void setup() {
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
  
  Serial.begin(9600);
  Serial.println("Gebruik commando's: V[tijd], A[tijd], L[tijd], R[tijd]");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    char direction = command.charAt(0);
    float duration = command.substring(1).toFloat();

    // Genereer een willekeurig getal om te beslissen of de robot zal luisteren
    if (random(0, 100) < FOLLOW_COMMAND_PROBABILITY * 100) {
      // Luister naar het commando
      executeCommand(direction, duration);
    } else {
      // Negeer het commando en doe iets willekeurigs
      executeRandomAction(duration);
    }
  }
}

// Functie om het originele commando uit te voeren
void executeCommand(char direction, float duration) {
  switch (direction) {
    case 'V':
      Serial.print("Vooruit luisteren voor: ");
      moveForward(duration);
      break;
    case 'A':
      Serial.print("Achteruit luisteren voor: ");
      moveBackward(duration);
      break;
    case 'L':
      Serial.print("Links draaien luisteren voor: ");
      turnLeft(duration);
      break;
    case 'R':
      Serial.print("Rechts draaien luisteren voor: ");
      turnRight(duration);
      break;
    default:
      Serial.println("Ongeldig commando.");
      break;
  }
}

// Functie om een willekeurige actie uit te voeren
void executeRandomAction(float duration) {
  int randomAction = random(0, 4); // Kies een willekeurige actie (0 t/m 3)
  
  Serial.print("Verward gedrag: ");
  switch (randomAction) {
    case 0:
      Serial.println("Vooruit gaan (tegenstrijdig commando)");
      moveForward(duration);
      break;
    case 1:
      Serial.println("Achteruit gaan (tegenstrijdig commando)");
      moveBackward(duration);
      break;
    case 2:
      Serial.println("Links draaien (tegenstrijdig commando)");
      turnLeft(duration);
      break;
    case 3:
      Serial.println("Rechts draaien (tegenstrijdig commando)");
      turnRight(duration);
      break;
  }
}

// Functies voor bewegingen
void moveForward(float seconds) {
  leftServo.write(LEFT_FORWARD_SPEED);
  rightServo.write(RIGHT_FORWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

void moveBackward(float seconds) {
  leftServo.write(LEFT_BACKWARD_SPEED);
  rightServo.write(RIGHT_BACKWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

void turnLeft(float seconds) {
  leftServo.write(LEFT_BACKWARD_SPEED);
  rightServo.write(RIGHT_FORWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

void turnRight(float seconds) {
  leftServo.write(LEFT_FORWARD_SPEED);
  rightServo.write(RIGHT_BACKWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

void stopMoving() {
  leftServo.write(STOP_SPEED);
  rightServo.write(STOP_SPEED);
  delay(500);
}
