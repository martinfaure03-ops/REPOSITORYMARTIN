import serial                       # Bibliothèque pySerial pour gérer le port série
import time                         # Pour les temporisations
from serial import SerialException  # Pour gérer proprement les erreurs

# ---- CONFIG ----
PORT = "/dev/cu.usbmodem34B7DA648DC82"   # À adapter selon ta machine
BAUDRATE = 1000000                       # 1 Mbit/s (doit être identique à l'Arduino)

# ---- OUVERTURE DU PORT ----
try:
    ser = serial.Serial(
        port=PORT,
        baudrate=BAUDRATE,
        bytesize=serial.EIGHTBITS,        # 8 bits de données
        parity=serial.PARITY_NONE,        # Pas de parité
        stopbits=serial.STOPBITS_ONE,     # 1 bit de stop
        timeout=1                         # Timeout de lecture (1 seconde)
    )
    print(f"✅ Port série ouvert : {PORT}")
except SerialException as e:
    print("❌ Erreur d’ouverture du port :", e)
    exit()


# ---- ENVOI DU CARACTÈRE DE SYNCHRO ----
print("➡️ Envoi du caractère de synchronisation '#' ...")
ser.write(b"#")   # Envoi du caractère '#'
time.sleep(1)     # On laisse un peu de temps à l'Arduino pour répondre


# ---- LECTURE DE LA RÉPONSE ARDUINO ----
print("📥 Réponse Arduino :")
while ser.in_waiting > 0:
    ligne = ser.readline().decode(errors='ignore').strip()
    if ligne:
        print("  ", ligne)

# ---- FIN ----
ser.close()
print("✅ Port fermé.")
