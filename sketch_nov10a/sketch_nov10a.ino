// GIT STEP 3 - Comptage temporel + Reset logiciel
// - Synchro par '#' depuis le PC
// - Reception de 's' dans loop() → reset logiciel après ~10 s
// - Comptage du temps en microsecondes depuis le dernier démarrage
// - t0_us est réinitialisé à chaque reset (setup() relancé)

bool restart = false;               // true si un reset est demandé
unsigned long tResetStartMs = 0;    // moment où 's' a été reçu
const unsigned long DELAI_RESET_MS = 10000; // 10 000 ms = 10 s

unsigned long t0_us = 0;           // temps de référence en µs (remis à zéro à chaque démarrage)

void setup() {
  // Configuration de la LED interne
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // en attente de synchro

  // Port série : 1 Mbit/s, 8N1
  Serial.begin(1000000, SERIAL_8N1);

  Serial.println("STEP 3 : En attente du caractere de synchronisation '#'...");

  // --- Attente du caractère '#' pour démarrer loop() ---
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '#') {
        Serial.println("SYNC OK (STEP 3)");
        digitalWrite(LED_BUILTIN, LOW); // synchro OK
        break;
      }
    }
  }

  // --- Initialisation du compteur de temps en µs ---
  t0_us = micros();
  Serial.println("Compteur temps en microsecondes remis a zero.");
  Serial.println("Envoyer 's' pour demander un reset dans 10 secondes.");
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

  // --- 2) Gestion du délai de 10 s avant reset ---
  if (restart) {
    unsigned long tActuel = millis();

    // LED clignote rapidement pendant l'attente du reset
    if ((millis() / 200) % 2 == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }

    if (tActuel - tResetStartMs >= DELAI_RESET_MS) {
      Serial.println("10 secondes ecoulees → RESET logiciel maintenant !");
      delay(100);                // laisse le temps au message de partir
      __NVIC_SystemReset();      // reset logiciel (si ta carte le supporte)
    }
  } else {
    // LED clignotement lent = fonctionnement normal
    if ((millis() / 1000) % 2 == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  // --- 3) Comptage temporel en µs depuis le démarrage ---
  unsigned long temps_ecoule_us = micros() - t0_us;

  Serial.print("Temps ecoule (us) = ");
  Serial.println(temps_ecoule_us);

  // Petite pause pour ne pas saturer la liaison série
  delay(500);
}



