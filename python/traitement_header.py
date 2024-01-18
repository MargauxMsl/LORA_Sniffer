import serial

# Spécifiez le port série sur lequel votre Arduino est connecté
port = '/dev/ttyACM0'  # Remplacez cela par le bon port sur votre système (peut être COMx sur Windows)

# Spécifiez le taux de bauds qui correspond à celui de votre Arduino
baud_rate = 9600

# Initialisez la connexion série
ser = serial.Serial(port, baud_rate)

# Initialisez le dictionnaire pour stocker les données
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
i = 0
try:
    while True:
        print("Hello", i)
        # Attendez la ligne "Packet Details:"
        line = ser.readline().decode('utf-8').strip()
        print("Recu : ", line)
        if line == "Packet Details:":
            # Lisez les 10 lignes suivantes avec les valeurs
            for key in data_dict.keys():
                value_line = ser.readline().decode('utf-8').strip()
                # Extrait la valeur de chaque ligne et stocke dans le dictionnaire
                data_dict[key] = int(value_line)

            # Ajoutez votre traitement des données ici
            # Par exemple, imprimez le dictionnaire mis à jour
            print("Données traitées:", data_dict)
        i += 1

except KeyboardInterrupt:
    # Gérez l'interruption du clavier (Ctrl+C) pour fermer proprement la connexion série
    ser.close()
    print("Connexion série fermée.")

print("ICI")
print(data_dict)