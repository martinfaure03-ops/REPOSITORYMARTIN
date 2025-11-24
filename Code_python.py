#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov 17 16:13:14 2025

@author: marthymgl
"""

#Utiliser une librairie qui va gérer le port-série

pip install pyserial
import serial
import time



#Mets ici le port exact trouvé dans Arduino IDE -> Tools -> Port

ser = serial.Serial("/dev/cu.usbmodem34B7DA648DC82", 1000000, timeout=1)

time.sleep(2)  # laisse le temps à l'Arduino de redémarrer

ser.write(b"#")  # caractère de synchro
print("Caractère '#' envoyé à l'Arduino.")

while :
    line = ser.readline().decode().strip()
    if line:
        print("Arduino:", line)
        
#%% CODE PROF

serialPort = serial.Serial()
serialPort.baudrate = 1000000
serialPort.port = "/dev/cu.usbmodem34B7DA648DC82"
serialPort.stopbits = serial.STOPBITS_ONE
serialPort.bytesize = serial.EIGHTBITS

try : 
    
    serialPort.open()
except SerialException as serialException:
    print(serialException)
    
    if (not serialPort.isOpen()) : 
        print('Serial port not opened')
        exit()
        
        try : 
            print("Serial port opened. Write run character.")
            cmd = "#"
            serialPort.write(cmd.encode(encoding = "ascii"))
            serialPort.close()
            print("Port closed")
            
            except Exception as exception:
                print ('exception occured while writing run character')
                print(exception)



#%%  IA

import serial
import time
from serial import SerialException

# ---- CONFIG ----
PORT = "/dev/cu.usbmodem34B7DA648DC82"   # à adapter selon ton cas
BAUDRATE = 1000000

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


# ---- SYNCHRONISATION ----
print("➡️ Envoi du caractère de synchro '#'...")
ser.write(b"#")   # caractère de démarrage
time.sleep(2)

# ---- Lecture du retour Arduino ----
while ser.in_waiting > 0:
    print(ser.readline().decode(errors='ignore').strip())

# ---- Attendre un peu avant de redémarrer ----
print("⏳ Attente avant redémarrage (5 s)...")
time.sleep(5)

# ---- Envoi du caractère de reset ----
print("➡️ Envoi du caractère '@' pour redémarrage (10 s)...")
ser.write(b"@")

# ---- Lire la réponse ----
time.sleep(1)
while ser.in_waiting > 0:
    print(ser.readline().decode(errors='ignore').strip())

# ---- Fermer le port ----
ser.close()
print("✅ Port fermé.")

        




            
            
            
           