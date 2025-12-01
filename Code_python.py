import serial                       # Gestion du port série
import time                         # Temporisations
from serial import SerialException  # Gestion propre des erreurs

# ---- CONFIG ----
PORT = "/dev/cu.usbmodem34B7DA648DC82"   # À adapter selon ta machine
BAUDRATE = 1000000                       # 1 Mbit/s (doit matcher l'Arduino)

# ---- OUVERTURE DU PORT ----
try:
    ser = serial.Serial(
        port=PORT,
        baudrate=BAUDRATE,
        bytesize=serial.EIGHTBITS,        # 8 bits de données
        parity=serial.PARITY_NONE,        # sans parité
        stopbits=serial.STOPBITS_ONE,     # 1 bit de stop
        timeout=1                         # timeout de lecture 1 seconde
    )
    print(f"✅ Port série ouvert : {PORT}")
except SerialException as e:
    print("❌ Erreur d’ouverture du port :", e)
    exit()


# ---- SYNCHRO AVEC '#' ----
print("➡️ Envoi du caractère de synchronisation '#' ...")
ser.write(b"#")          # envoi de '#'
time.sleep(1)            # on laisse le temps à l'Arduino de répondre

print("📥 Réponse Arduino après synchro :")
while ser.in_waiting > 0:
    ligne = ser.readline().decode(errors='ignore').strip()
    if ligne:
        print("  ", ligne)


# ---- ENVOI DU CARACTÈRE DE RESET 's' ----
print("➡️ Envoi du caractère 's' pour demander un reset (Arduino fera le reset dans 10 s)...")
ser.write(b"s")          # caractère spécial pour STEP 2
time.sleep(1)            # petit délai pour la réponse

print("📥 Réponse Arduino après demande de reset :")
while ser.in_waiting > 0:
    ligne = ser.readline().decode(errors='ignore').strip()
    if ligne:
        print("  ", ligne)


# ---- FERMETURE ----
ser.close()
print("✅ Port fermé.")

