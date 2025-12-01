// GIT STEP 4 - Transmission du comptage temporel
// - Synchro par '#' (comme les steps précédents)
// - Comptage du temps en µs via micros()
// - Transmission périodique du temps au PC (en secondes, formaté avec sprintf + Serial.println)
// - Pas de delay() dans le code
// - Utilisation de Serial.flush()
// - Possibilité de reset logiciel 10 s après réception de 's' (optionnel, continuité step 2/3)

bool restart = false;                  // reset demandé ?
unsigned long tResetStartMs = 0;       // instant où 's' a été reçu
const unsigned long DELAI_RESET_MS = 10000; // 10 s

unsigned long t0_us = 0;              // temps de référence en µs (remis à zéro à chaque démarrage)
unsigned long lastSendUs = 0;         // dernière fois où on a envoyé une mesure
const unsigned long PERIODE_ENVOI_US = 100000; // période d'envoi = 100 000 µs = 0,1 s

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // en attente de synchro

  Serial.begin(1000000, SERIAL_8N1);

  Serial.println("STEP 4 : En attente du caractere '#' pour synchro...");
  Serial.flush();

  // --- Attente du caractere '#' pour démarrer loop() ---
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '#') {
        Serial.println("SYNC OK (STEP 4)");
        Serial.flush();
        digitalWrite(LED_BUILTIN, LOW); // synchro OK
        break;
      }
    }
  }

  // Initialisation du compteur de temps
  t0_us = micros();
  lastSendUs = 0;

  Serial.println("Compteur temps en µs initialise. Transmission vers PC.");
  Serial.flush();
}

void loop() {
  unsigned long nowMs = millis();
  unsigned long nowUs = micros();

  // --- 1) Lecture du port série : reset optionnel avec 's' ---
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 's') {
      restart = true;
      tResetStartMs = nowMs;
      Serial.println("Caractere 's' recu : reset logiciel dans 10 secondes...");
      Serial.flush();
    }
  }

  // --- 2) Gestion éventuelle du reset différé ---
  if (restart) {
    // LED clignote rapidement pendant l'attente
    if ((nowMs / 200) % 2 == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }

    if (nowMs - tResetStartMs >= DELAI_RESET_MS) {
      Serial.println("10 secondes ecoulees → RESET logiciel !");
      Serial.flush();
      __NVIC_SystemReset(); // reset logiciel (si ta carte le supporte)
    }
  } else {
    // LED clignotement lent pour montrer que tout va bien
    if ((nowMs / 1000) % 2 == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  // --- 3) Transmission périodique du temps au PC ---
  unsigned long temps_ecoule_us = nowUs - t0_us;

  // On envoie une valeur toutes les PERIODE_ENVOI_US µs
  if (temps_ecoule_us - lastSendUs >= PERIODE_ENVOI_US) {
    lastSendUs = temps_ecoule_us;

    // Conversion en secondes (float) côté Arduino
    float temps_s = temps_ecoule_us / 1000000.0f;

    char buffer[32];
    // Exemple : "0.123456"
    sprintf(buffer, "%.6f", temps_s);

    Serial.println(buffer);
    Serial.flush(); // s'assurer que la trame est bien envoyée
  }

  // AUCUN delay() dans loop()
}
