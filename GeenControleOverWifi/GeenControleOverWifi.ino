#include <Servo.h> // Inclusie van de Servo-bibliotheek voor servo-aansturing
#include <WiFiNINA.h> // Inclusie van de WiFi-bibliotheek voor netwerkcommunicatie

// WiFi instellingen
const char* ssid = "xxxxx"; // Naam van het WiFi-netwerk
const char* password = "xxxxx"; // Wachtwoord voor het WiFi-netwerk

WiFiUDP udp; // Maak een UDP-object voor netwerkcommunicatie
unsigned int localUdpPort = 4210; // UDP poort voor het ontvangen van commando's

// Servo instellingen
Servo leftServo; // Object voor de linker servo
Servo rightServo; // Object voor de rechter servo
const int leftServoPin = 9; // Pin waar de linker servo is aangesloten
const int rightServoPin = 10; // Pin waar de rechter servo is aangesloten

// Constanten voor servo snelheden
const int LEFT_FORWARD_SPEED = 30; // Snelheid voor de linker servo bij vooruit
const int LEFT_BACKWARD_SPEED = 150; // Snelheid voor de linker servo bij achteruit
const int RIGHT_FORWARD_SPEED = 165; // Snelheid voor de rechter servo bij vooruit
const int RIGHT_BACKWARD_SPEED = 15; // Snelheid voor de rechter servo bij achteruit
const int STOP_SPEED = 90; // Snelheid voor stoppen

// SHARP afstandssensor instellingen
const int distanceSensorPin = A0; // Pin waar de SHARP afstandssensor is aangesloten
const int distanceThreshold = 5; // Drempelafstand in cm (hoe dichtbij een object kan komen)

char incomingPacket[255]; // Array voor het opslaan van binnenkomende UDP-berichten
bool isRandomBehaviorActive = true; // Variabele voor willekeurig gedrag van de robot

void setup() {
    Serial.begin(9600); // Start seriële communicatie op 9600 baud

    // Verbinden met WiFi
    Serial.print("Verbinding maken met WiFi...");
    WiFi.begin(ssid, password); // Start verbinding met het WiFi-netwerk
    // Wacht tot de verbinding is gemaakt
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000); // Wacht 1 seconde
        Serial.print("."); // Print een punt terwijl je wacht
    }
    Serial.println(" Verbonden met WiFi!"); // Meld dat de verbinding is gemaakt
    Serial.print("IP-adres: ");
    Serial.println(WiFi.localIP()); // Print het IP-adres van de Arduino

    // Start UDP-communicatie
    udp.begin(localUdpPort); // Begin met luisteren op de opgegeven UDP-poort
    Serial.print("Luisteren op UDP poort ");
    Serial.println(localUdpPort); // Print de UDP-poort
    // Servo's instellen
    leftServo.attach(leftServoPin); // Koppel de linker servo aan de juiste pin
    rightServo.attach(rightServoPin); // Koppel de rechter servo aan de juiste pin
}

void loop() {
    // Lees afstand van de SHARP-sensor
    int distance = readSharpDistance(); // Roep de functie aan om de afstand te lezen
    // Controleer of een object te dichtbij is
    if (distance > 0 && distance < distanceThreshold) {
        Serial.println("Object te dichtbij! Stoppen."); // Meld dat er een object te dichtbij is
        stopMoving(); // Stop de robot
        delay(1000); // Wacht 1 seconde voordat je verdergaat
    } else {
        // Controleer op inkomende UDP-pakketten
        int packetSize = udp.parsePacket(); // Kijk of er een pakket ontvangen is
        if (packetSize) { // Als er een pakket ontvangen is
            int len = udp.read(incomingPacket, 255); // Lees het pakket in de buffer
            if (len > 0) incomingPacket[len] = 0; // Zorg ervoor dat de string eindigt
            Serial.print("Ontvangen commando: ");
            Serial.println(incomingPacket); // Print het ontvangen commando
            processCommandWithRandomness(incomingPacket); // Verwerk het commando
        }
    }
}

// Functie om de SHARP-afstandssensor te lezen
int readSharpDistance() {
    int analogValue = analogRead(distanceSensorPin); // Lees de analoge waarde van de sensor
    
    // Omrekenen van analoge waarde naar afstand (in cm)
    float voltage = analogValue * (5.0 / 1023.0); // Zet de analoge waarde om naar spanning
    int distance = 27.86 / (voltage - 0.1); // Schatting van de afstand op basis van de spanning voor de SHARP 2Y0A21
    return distance; // Geef de afstand terug
}

// Functie om met random gedrag commando's te verwerken
void processCommandWithRandomness(String command) {
    char direction = command.charAt(0); // Eerste karakter geeft de richting aan
    float duration = command.substring(1).toFloat(); // Rest van de string geeft de duur aan

    if (isRandomBehaviorActive) { // Controleer of willekeurig gedrag actief is
        int chance = random(0, 10); // Genereer een willekeurig getal tussen 0 en 9
        if (chance < 7) { // 70% kans dat de robot niet luistert
            Serial.println("Luistert niet"); // Print dat de robot niet luistert
            randomMove(duration); // Voer een willekeurige beweging uit
            return; // Verlaat de functie
        }
    }

    // Als het commando geldig is, voer dan de juiste beweging uit
    if (direction == 'V') moveForward(duration); // Vooruit
    else if (direction == 'A') moveBackward(duration); // Achteruit
    else if (direction == 'L') turnLeft(duration); // Links draaien
    else if (direction == 'R') turnRight(duration); // Rechts draaien
    else Serial.println("Ongeldig commando."); // Meld dat het commando ongeldig is
}

// Willekeurige bewegingen functie
void randomMove(float seconds) {
    int randomAction = random(0, 4); // Genereer een willekeurige actie (0-3)
    switch (randomAction) {
        case 0: moveForward(seconds); break; // Beweeg vooruit
        case 1: moveBackward(seconds); break; // Beweeg achteruit
        case 2: turnLeft(seconds); break; // Draai links
        case 3: turnRight(seconds); break; // Draai rechts
    }
}

// Functie om vooruit te bewegen
void moveForward(float seconds) {
    Serial.println("Vooruit bewegen"); // Print dat de robot vooruit beweegt
    leftServo.write(LEFT_FORWARD_SPEED); // Zet de snelheid van de linker servo
    rightServo.write(RIGHT_FORWARD_SPEED); // Zet de snelheid van de rechter servo
    delay(seconds * 1000); // Wacht de opgegeven tijd
    stopMoving(); // Stop de robot
}

// Functie om achteruit te bewegen
void moveBackward(float seconds) {
    Serial.println("Achteruit bewegen"); // Print dat de robot achteruit beweegt
    leftServo.write(LEFT_BACKWARD_SPEED); // Zet de snelheid van de linker servo
    rightServo.write(RIGHT_BACKWARD_SPEED); // Zet de snelheid van de rechter servo
    delay(seconds * 1000); // Wacht de opgegeven tijd
    stopMoving(); // Stop de robot
}

// Functie om links te draaien
void turnLeft(float seconds) {
    Serial.println("Links draaien"); // Print dat de robot naar links draait
    leftServo.write(LEFT_BACKWARD_SPEED); // Zet de snelheid van de linker servo
    rightServo.write(RIGHT_FORWARD_SPEED); // Zet de snelheid van de rechter servo
    delay(seconds * 1000); // Wacht de opgegeven tijd
    stopMoving(); // Stop de robot
}

// Functie om rechts te draaien
void turnRight(float seconds) {
    Serial.println("Rechts draaien"); // Print dat de robot naar rechts draait
    leftServo.write(LEFT_FORWARD_SPEED); // Zet de snelheid van de linker servo
    rightServo.write(RIGHT_BACKWARD_SPEED); // Zet de snelheid van de rechter servo
    delay(seconds * 1000); // Wacht de opgegeven tijd
    stopMoving(); // Stop de robot
}

// Functie om te stoppen
void stopMoving() {
    leftServo.write(STOP_SPEED); // Zet beide servo's naar stoppositie
    rightServo.write(STOP_SPEED); // Zet beide servo's naar stoppositie
    delay(500); // Wacht een halve seconde voordat je verder gaat
}
