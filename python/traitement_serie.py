## CODE QUI PERMET DE TRAITER LA RECEPTION EN SERIE D'UN ARDUINO
#pip install pyserial


import serial

# Spécifiez le port série sur lequel votre Arduino est connecté
port = '/dev/ttyUSB0'  # Remplacez cela par le bon port sur votre système (peut être COMx sur Windows)

# Spécifiez le taux de bauds qui correspond à celui de votre Arduino
baud_rate = 9600

# Initialisez la connexion série
ser = serial.Serial(port, baud_rate)

try:
    while True:
        # Lisez une ligne depuis le port série
        line = ser.readline().decode('utf-8').strip()
        
        # Affichez la ligne lue
        print("Données reçues depuis l'Arduino:", line)

        # Ajoutez votre traitement des données ici

except KeyboardInterrupt:
    # Gérez l'interruption du clavier (Ctrl+C) pour fermer proprement la connexion série
    ser.close()
    print("Connexion série fermée.")
