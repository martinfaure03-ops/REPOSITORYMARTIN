import serial
import time
from serial import SerialException

# ---- CONFIG ----
PORT = "/dev/cu.usbmodem34B7DA648DC82"   # adapte ce chemin si besoin
BAUDRATE = 1000000                       # 1 Mbit/s

# ---- OUVERTURE DU PORT ----
try:
    ser = serial.Serial(
        port=PORT,
        baudrate=BAUDRATE,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=0.1                     # petit timeout
    )
    print(f"Port série ouvert : {PORT}")
except SerialException as e:
    print("Erreur d’ouverture du port :", e)
    exit()

# On nettoie les buffers éventuels
ser.reset_input_buffer()
ser.reset_output_buffer()

# ---- SYNCHRO '#' ----
ser.write(b"#")     # on envoie le caractère de synchro
ser.flush()         # on force l'envoi
time.sleep(0.2)     # un tout petit peu de temps pour la réponse Arduino

# On lit rapidement les premiers messages (SYNC OK...)
start = time.time()
while time.time() - start < 1.0:   # pendant 1 seconde max
    if ser.in_waiting > 0:
        _ = ser.readline()  # on ignore ces lignes pour ne pas ralentir
    else:
        time.sleep(0.01)

# ---- RÉCEPTION DU COMPTAGE TEMPOREL ----
times = []   # tableau pour stocker les temps en secondes (float)

duree_acquisition = 5.0  # par exemple : on acquiert pendant 5 secondes
t0 = time.time()

while time.time() - t0 < duree_acquisition:
    if ser.in_waiting > 0:
        ligne = ser.readline().decode(errors='ignore').strip()
        if ligne:
            try:
                t = float(ligne)   # valeur en secondes
                times.append(t)
            except ValueError:
                # Ligne non numérique, on l'ignore
                pass
    else:
        # éviter de boucler trop vite
        time.sleep(0.001)

# ---- FIN : on peut afficher un petit résumé uniquement ----
ser.close()

print(f"Acquisition terminée, {len(times)} echantillons reçus.")
if len(times) > 0:
    print(f"Premier temps : {times[0]:.6f} s")
    print(f"Dernier temps : {times[-1]:.6f} s")
