import socket

# Vervang dit door het IP-adres van je Arduino
UDP_IP = "....."  
UDP_PORT = 4210

# Maak een UDP-socket aan
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    command = input("Voer een commando in (bijv. 'V2' voor vooruit 2 seconden, 'A2' voor achteruit 2 seconden): ")
    if command:  # Zorg ervoor dat er een invoer is
        sock.sendto(command.encode(), (UDP_IP, UDP_PORT))
        print(f"Verzonden commando: {command}")
