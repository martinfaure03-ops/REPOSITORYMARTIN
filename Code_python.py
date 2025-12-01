import serial
import time
from serial import SerialException

# ---- CONFIG ----
PORT = "/dev/cu.usbmodem34B7DA648DC82"   # à adapter
BAUDRATE = 1000000                       # 1 Mbit/s

# ---- OUVERTURE DU PORT ----
try:
    ser = serial.Serial(
        port=PORT,
        baudrate=BAUDRATE,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=1
    )
    print(f"✅ Port série ouvert : {PORT}")
except SerialException as e:
    print("❌ Erreur d’ouverture du port :", e)
    exit()

# ---- SYNCHRO '#' ----
print("➡️ Envoi du caractère de synchronisation '#' ...")
ser.write(b"#")
time.sleep(1)

print("📥 Réponse Arduino après synchro :")
for _ in range(5):  # on lit quelques lignes (temps en µs)
    if ser.in_waiting > 0:
        ligne = ser.readline().decode(errors='ignore').strip()
        if ligne:
            print("  ", ligne)
    time.sleep(0.5)

# ---- DEMANDE DE RESET 's' ----
print("➡️ Envoi du caractère 's' pour demander un reset (Arduino fera le reset dans 10 s)...")
ser.write(b"s")
time.sleep(1)

print("📥 Réponse Arduino après demande de reset :")
for _ in range(10):
    if ser.in_waiting > 0:
        ligne = ser.readline().decode(errors='ignore').strip()
        if ligne:
            print("  ", ligne)
    time.sleep(0.5)

# ---- FERMETURE ----
ser.close()
print("✅ Port fermé.")
