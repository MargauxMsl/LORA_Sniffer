# PROJET SIR - SNIFFER LORAWAN
# Noé Caringi - Soukaina Khalil - Margaux Massol - Hillel Partouche
# Dans le cadre du cursus d'ingénieur en télécommunications au sein de l'INSA Lyon

## Traitement des données ##

import serial
from scapy.all import Ether, Raw, wrpcap

# Port sur lequel l'Arduino est connecté
port = '/dev/ttyACM0'

# Bauds qui correspond à celui de votre Arduino
baud_rate = 9600

# Initialisation de la connexion série
ser = serial.Serial(port, baud_rate)

i = 0
data_dict = {
    'Rx Count': None,
    'Rx@millis': None,
    'Last RSSI': None,
    'From Addr': None,
    'To Addr': None,
    'MsgId': None,
    'Hdr Flags': None,
    'isAck': None,
    'Packet Len': None,
    'Packet Contents': None
}

try:
    while True:
        # Attente de la ligne "Packet Details:"
        line = ser.readline().decode('utf-8').strip()
        print("Recu : ", line)
        if line == "Packet Details:":
            # Création d'un paquet Scapy
            packet = Ether()
            
            # Lecture des 10 lignes suivantes avec les valeurs
            for key in data_dict.keys():
                value_line = ser.readline().decode('utf-8').strip()
                # Extrait la valeur de chaque ligne et stockage dans le dictionnaire
                data_dict[key] = int(value_line)
                
                # Ajout de la valeur au paquet Scapy
                packet.add_payload(Raw(load=value_line.encode()))

            print("Données traitées:", data_dict)
            
            # Enregistrement du paquet Scapy dans un fichier PCAP
            wrpcap(f'pkt/data_packet_{i}.pcap', packet)

            i += 1

except KeyboardInterrupt:
    # Interruption du clavier (Ctrl+C) pour fermer proprement la connexion série
    ser.close()
    print("Connexion série fermée.")

