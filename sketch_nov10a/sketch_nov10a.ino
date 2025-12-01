// GIT STEP 2 - Synchronisation 2 + Reset logiciel après 10 s
// - Synchro par '#' (comme STEP 1)
// - Dans loop(), reception du caractere 's' → reset logiciel après ~10 secondes
// - Reset initié côté PC (qui envoie 's')

bool restart = false;               // true si un reset a été demandé
unsigned long tResetStartMs = 0;    // instant où 's' a été reçu
const unsigned long DELAI_RESET_MS = 10000; // 10 000 ms = 10 s

void setup() {
  // Configuration de la LED interne
  pinMode(LED_BUILTIN, OUTPUT);

  // LED allumée = on est dans setup(), en attente de synchro
  digitalWrite(LED_BUILTIN, HIGH);

  // Port série : 1 Mbit/s, 8N1
  Serial.begin(1000000, SERIAL_8N1);

  Serial.println("STEP 2 : En attente du caractere de synchronisation '#'...");

  // --- Attente du caractère '#' pour démarrer loop() ---
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '#') {   // caractère de synchro venant du PC
        Serial.println("SYNC OK (STEP 2)");
        digitalWrite(LED_BUILTIN, LOW);  // synchro OK
        break;   // sort du while et entre dans loop()
      }
    }
  }
}

void loop() {
  // --- 1) Lecture du port série : detection du caractere 's' ---
  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == 's') {   // caractère spécial pour demander le reset
      Serial.println("Caractere 's' recu : reset logiciel dans 10 secondes...");
      restart = true;
      tResetStartMs = millis();  // mémorise l'instant de la demande
    }
  }

  // --- 2) Si un reset est demandé, attendre 10 s puis reset ---
  if (restart) {
    unsigned long tActuel = millis();

    // LED clignote rapidement pour indiquer l'attente avant reset
    if ((millis() / 200) % 2 == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }

    if (tActuel - tResetStartMs >= DELAI_RESET_MS) {
      Serial.println("10 secondes ecoulees → RESET logiciel maintenant !");
      delay(100);                // laisse le temps au message de partir
      __NVIC_SystemReset();      // reset logiciel (Cortex-M / STM32, etc.)
      // si ça ne compile pas sur ta carte : demander à ton prof la méthode attendue
    }
  } else {
    // --- 3) Comportement "normal" pendant que rien n'est demandé ---
    digitalWrite(LED_BUILTIN, HIGH);  // LED allumée en continu
    Serial.println("STEP 2 : loop() en cours (aucun reset demande)...");
    delay(500);
  }
}


