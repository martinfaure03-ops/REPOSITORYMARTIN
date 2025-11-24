void setup() {
  // Configuration de la LED interne
  pinMode(LED_BUILTIN, OUTPUT);

  // Configuration du port série à 1 000 000 bauds (1 Mbit/s), 8N1
  Serial.begin(1000000, SERIAL_8N1);

  // Message initial pour savoir que setup() démarre
  Serial.println("En attente du caractere de synchronisation...");
  
  // Attente du caractère spécial pour démarrer loop()
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '#') {   // caractère de synchro venant du PC
        Serial.println("SYNC OK");
        digitalWrite(LED_BUILTIN, HIGH);  // confirmation visuelle
        break;   // sort du while et entre dans loop()
      }
    }
  }

}

void loop() {
   // Code exécuté seulement après synchro
  Serial.println("Loop en cours...");
  delay(500);
}

















