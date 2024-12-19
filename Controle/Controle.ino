/*
Robot met twee servo's die als motors worden gebruikt. 
De robot kan verschillende commando's ontvangen om vooruit te bewegen, achteruit te gaan, 
en naar links of rechts te draaien. Dus de kant waar het op beweegt en hoelang hij het moet doen.
Het aanroepen: 
  - 'V[tijd]' om vooruit te gaan voor een bepaalde tijd (bijv. V2 betekent 2 seconden vooruit).
  - 'A[tijd]' om achteruit te gaan.
  - 'L[tijd]' om links te draaien.
  - 'R[tijd]' om rechts te draaien.

Spullen die ik gebruik:
- Twee 360-graden servo's (FT90R of soortgelijk) aangesloten op pinnen 9 en 10 van de Arduino.
*/

#include <Servo.h>

// Maak servo-objecten voor de linker en rechter servo
Servo leftServo;
Servo rightServo;

const int leftServoPin = 9;   
const int rightServoPin = 10; 

//snelheidsinstellingen voor elke servo, Als ze niet gelijk lopen moet je hier dan de snelheden veranderen
const int LEFT_FORWARD_SPEED = 30;     // Vooruit-snelheid voor linker servo
const int LEFT_BACKWARD_SPEED = 150;   // Achteruit-snelheid voor linker servo
const int RIGHT_FORWARD_SPEED = 165;   // Vooruit-snelheid voor rechter servo
const int RIGHT_BACKWARD_SPEED = 15;   // Achteruit-snelheid voor rechter servo
const int STOP_SPEED = 90;             // Stoppositie voor beide servo's (geen beweging)

void setup() {
  // Koppel de servo's aan de aangegeven pinnen
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
  
  // Start seriële communicatie om commando's te kunnen ontvangen
  Serial.begin(9600);
  Serial.println("Gebruik commando's: V[tijd], A[tijd], L[tijd], R[tijd]");
}

void loop() {
  // Controleer of er seriële data beschikbaar is
  if (Serial.available() > 0) {
    // Lees het binnengekomen commando tot het einde van de regel
    String command = Serial.readStringUntil('\n');
    command.trim(); // Verwijder eventuele witruimtes

    // Eerste teken van het commando geeft de richting aan, aanroepen doe je zo: V2 of V6 dat betekent dan vooruit voor 2 of 6 seconde
    char direction = command.charAt(0); // ChartAt(0) charAt leest wat je hebt getypt de (0) staat voor de locatie. Als je V2 typt is de CharAt() een V en de (1) een 2
    // Rest van het commando is de tijdsduur als float (seconden)
    float duration = command.substring(1).toFloat();

    // Hier kijkt het nu als er een letter is opgegeven welke functie er dan bij hoort
    // Controleer het type commando en voer de bijbehorende functie uit
    if (direction == 'V') {
      Serial.print("Vooruit voor: ");
      Serial.print(duration);
      Serial.println(" seconden");
      moveForward(duration);  // Roep functie aan om vooruit te bewegen
    } 
    else if (direction == 'A') {
      Serial.print("Achteruit voor: ");
      Serial.print(duration);
      Serial.println(" seconden");
      moveBackward(duration);  // Roep functie aan om achteruit te bewegen
    } 
    else if (direction == 'L') {
      Serial.print("Links draaien voor: ");
      Serial.print(duration);
      Serial.println(" seconden");
      turnLeft(duration);  // Roep functie aan om naar links te draaien
    } 
    else if (direction == 'R') {
      Serial.print("Rechts draaien voor: ");
      Serial.print(duration);
      Serial.println(" seconden");
      turnRight(duration);  // Roep functie aan om naar rechts te draaien
    } // Dit doet het programma wanneer die het niet kan lezen als je bijvoorbeeld v2 doet maar geen hoofdletter V hebt gedaan.
    else {
      Serial.println("Ongeldig commando. Gebruik V, A, L, of R gevolgd door de tijd.");
    }
  }
}

// Functie om de robot vooruit te bewegen
void moveForward(float seconds) {
  // Stel de snelheid van beide servo's in om vooruit te rijden
  leftServo.write(LEFT_FORWARD_SPEED);
  rightServo.write(RIGHT_FORWARD_SPEED);
  // Wacht voor de aangegeven tijdsduur
  delay(seconds * 1000);
  // Stop de beweging na de tijdsduur
  stopMoving();
}

// Functie om de robot achteruit te bewegen
void moveBackward(float seconds) {
  // Stel de snelheid van beide servo's in om achteruit te rijden
  leftServo.write(LEFT_BACKWARD_SPEED);
  rightServo.write(RIGHT_BACKWARD_SPEED);
  // Wacht voor de aangegeven tijdsduur
  delay(seconds * 1000);
  // Stop de beweging na de tijdsduur
  stopMoving();
}

// Functie om de robot naar links te draaien
void turnLeft(float seconds) {
  // Stel de linker servo in om achteruit te draaien en de rechter servo om vooruit te draaien
  leftServo.write(LEFT_BACKWARD_SPEED);
  rightServo.write(RIGHT_FORWARD_SPEED);
  // Wacht voor de aangegeven tijdsduur
  delay(seconds * 1000);
  // Stop de beweging na de tijdsduur
  stopMoving();
}

// Functie om de robot naar rechts te draaien
void turnRight(float seconds) {
  // Stel de linker servo in om vooruit te draaien en de rechter servo om achteruit te draaien
  leftServo.write(LEFT_FORWARD_SPEED);
  rightServo.write(RIGHT_BACKWARD_SPEED);
  // Wacht voor de aangegeven tijdsduur
  delay(seconds * 1000);
  // Stop de beweging na de tijdsduur
  stopMoving();
}

// Functie om de robot te stoppen
void stopMoving() {
  // Stel beide servo's in op de stoppositie
  leftServo.write(STOP_SPEED);
  rightServo.write(STOP_SPEED);
  // Voeg een korte pauze toe om te zorgen dat de robot stopt voordat hij een nieuw commando krijgt
  delay(500);
}
