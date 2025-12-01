// GIT STEP 1 - Synchronisation 1
// - UART : 1 000 000 bauds, 8N1 (8 bits, pas de parité, 1 bit de stop)
// - Attendre la réception du caractère '#' pour démarrer loop()
// - LED_BUILTIN utilisée pour vérifier qu'on est bien dans loop()

void setup() {
  // Configuration de la LED interne
  pinMode(LED_BUILTIN, OUTPUT);

  // LED allumée = on est dans setup(), en attente de synchro
  digitalWrite(LED_BUILTIN, HIGH);

  // Configuration de la liaison série : 1 Mbit/s, 8N1
  Serial.begin(1000000, SERIAL_8N1);

  // Message pour le moniteur série (facultatif mais pratique)
  Serial.println("STEP 1 : En attente du caractere de synchronisation '#'...");

  // Attente du caractère spécial pour démarrer loop()
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();

      if (c == '#') {   // caractère de synchro venant du PC
        Serial.println("SYNC OK (STEP 1)");
        // LED éteinte = synchro OK, on va entrer dans loop()
        digitalWrite(LED_BUILTIN, LOW);
        break;   // sort du while et entre dans loop()
      }
    }
  }
}

void loop() {
  // On est ici UNIQUEMENT après réception de '#'

  // Faire clignoter la LED pour montrer que loop() tourne
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);  // 500 ms

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);  // 500 ms

  // Message de debug (tu peux le commenter si ça spam trop)
  Serial.println("STEP 1 : loop() en cours...");
}

