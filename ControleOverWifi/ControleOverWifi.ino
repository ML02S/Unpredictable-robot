#include <WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// WiFi-instellingen
const char* ssid = "x";
const char* password = "x";

// Maak servo-objecten voor beide servo's
Servo leftServo;
Servo rightServo;

// Definieer de pinnen voor de servo's
const int leftServoPin = 9; // Verander indien nodig
const int rightServoPin = 10; // Verander indien nodig

// UDP-instellingen
WiFiUDP Udp;
unsigned int localPort = 4210;      // Lokale poort waarop de Arduino luistert
char incomingPacket[255];            // Buffer voor inkomende packets

// Snelheidsinstellingen
const int LEFT_FORWARD_SPEED = 0;   // Pas dit aan voor linker servo snelheid
const int LEFT_BACKWARD_SPEED = 180;
const int RIGHT_FORWARD_SPEED = 180; // Pas dit aan voor rechter servo snelheid
const int RIGHT_BACKWARD_SPEED = 0;
const int STOP_SPEED = 90;            // Stoppositie

void setup() {
  Serial.begin(9600);
  Serial.println("Verbinden met WiFi...");

  // Verbind met WiFi-netwerk
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Verbinding is gemaakt
  Serial.println("\nVerbonden met WiFi!");

  // Toon het IP-adres
  Serial.print("IP-adres: ");
  Serial.println(WiFi.localIP()); // Toon het IP-adres
  Udp.begin(localPort); // Begin met het luisteren naar UDP-pakketten op de lokale poort
  Serial.print("Luisteren op UDP-poort ");
  Serial.println(localPort);

  // Verbind de servo's met hun pinnen
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
}

void loop() {
  // Controleer of er een nieuw pakket is ontvangen
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Lees het pakket in
    int n = Udp.read(incomingPacket, 255);
    if (n > 0) {
      incomingPacket[n] = 0; // Voeg een null-terminator toe
    }
    Serial.print("Ontvangen commando: ");
    Serial.println(incomingPacket); // Gebruik println om de ontvangen gegevens weer te geven
    
    // Verwerk de ontvangen commando's
    if (incomingPacket[0] == 'V') { // Vooruit
      int duration = atoi(&incomingPacket[1]); // Haal de tijd op
      moveForward(duration);
    } else if (incomingPacket[0] == 'A') { // Achteruit
      int duration = atoi(&incomingPacket[1]); // Haal de tijd op
      moveBackward(duration);
    } else if (incomingPacket[0] == 'L') { // Links draaien
      int duration = atoi(&incomingPacket[1]); // Haal de tijd op
      turnLeft(duration);
    } else if (incomingPacket[0] == 'R') { // Rechts draaien
      int duration = atoi(&incomingPacket[1]); // Haal de tijd op
      turnRight(duration);
    } else {
      Serial.println("Ongeldig commando. Gebruik V, A, L, of R gevolgd door de tijd.");
    }
  }
}

// Functie om vooruit te bewegen
void moveForward(int seconds) {
  leftServo.write(LEFT_FORWARD_SPEED);
  rightServo.write(RIGHT_FORWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

// Functie om achteruit te bewegen
void moveBackward(int seconds) {
  leftServo.write(LEFT_BACKWARD_SPEED);
  rightServo.write(RIGHT_BACKWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

// Functie om links te draaien
void turnLeft(int seconds) {
  leftServo.write(LEFT_BACKWARD_SPEED);
  rightServo.write(RIGHT_FORWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

// Functie om rechts te draaien
void turnRight(int seconds) {
  leftServo.write(LEFT_FORWARD_SPEED);
  rightServo.write(RIGHT_BACKWARD_SPEED);
  delay(seconds * 1000);
  stopMoving();
}

// Functie om te stoppen
void stopMoving() {
  leftServo.write(STOP_SPEED);
  rightServo.write(STOP_SPEED);
  delay(500);
}
