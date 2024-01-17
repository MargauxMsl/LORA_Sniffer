import serial
import re

# Spécifiez le port série sur lequel votre Arduino est connecté
port = '/dev/ttyUSB0'  # Remplacez cela par le bon port sur votre système (peut être COMx sur Windows)

# Spécifiez le taux de bauds qui correspond à celui de votre Arduino
baud_rate = 9600

# Initialisez la connexion série
ser = serial.Serial(port, baud_rate)

# Expression régulière pour extraire les informations du paquet
packet_regex = re.compile(r"Packet Details:\s*- Rx Count: (\d+)\s*- Rx@millis: (\d+)\s*- Last RSSI: (\d+)\s*- From Addr: (\d+)\s*- To Addr: (\d+)\s*- MsgId: (\d+)\s*-Hdr Flags: (\d+)\s*-isAck: (\d+)\s*-Packet Len: (\d+)\s*-Packet Contents: (.*)")

# Initialisez la liste pour stocker les données de tous les messages
all_data_list = []

try:
    while True:
        # Lisez une ligne depuis le port série
        line = ser.readline().decode('utf-8').strip()
        
        # Affichez la ligne lue
        print("Données reçues depuis l'Arduino:", line)

        # Utilisez l'expression régulière pour extraire les informations du paquet
        match = packet_regex.match(line)
        if match:
            # Créez un dictionnaire avec les valeurs extraites
            data_dict = {
                'Rx Count': int(match.group(1)),
                'Rx@millis': int(match.group(2)),
                'Last RSSI': int(match.group(3)),
                'From Addr': int(match.group(4)),
                'To Addr': int(match.group(5)),
                'MsgId': int(match.group(6)),
                'Hdr Flags': int(match.group(7)),
                'isAck': int(match.group(8)),
                'Packet Len': int(match.group(9)),
                'Packet Contents': match.group(10)
            }

            # Ajoutez le dictionnaire à la liste
            all_data_list.append(data_dict)

            # Ajoutez votre traitement des données ici
            # Par exemple, imprimez la liste mise à jour
            print("Toutes les données traitées:", all_data_list)

except KeyboardInterrupt:
    # Gérez l'interruption du clavier (Ctrl+C) pour fermer proprement la connexion série
    ser.close()
    print("Connexion série fermée.")
