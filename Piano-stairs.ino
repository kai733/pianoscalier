#include "Arduino.h" // Import des bibliothèques
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int LDR_PINS[] = { A0, A1, A2, A3 }; // Définit les broches des capteurs
const int NB_STEPS = 4; // Nombre de marches

int trackMapping[] = { 1, 2, 3, 4 }; // Associe chaque capteur à un numéro de piste MP3
bool isPlaying[] = { false, false, false, false }; // Un booléen permet de savoir si un son est déjà en cours de déclenchement.
int threshold = 800; // Seuil de luminosité: Si les photorésistances passent en dessous de cette valeur cela veut dire que le laser est interrompu

SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true)
      ;
  }
  myDFPlayer.volume(30); // Volume du son (Max à 30)
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); // Lit la ligne envoyée
    if (data.startsWith("M")) { // Si ça commence par "M" (ex: M1:05)
      int stepIdx = data.substring(1, 2).toInt() - 1; // Récupère l'index (0 à 3)
      int trackNum = data.substring(3).toInt(); // Récupère le numéro de piste
      if (stepIdx >= 0 && stepIdx < NB_STEPS) { 
        trackMapping[stepIdx] = trackNum; // Met à jour le tableau
      }
    }
  }
    for (int i = 0; i < NB_STEPS; i++) {
    int val = analogRead(LDR_PINS[i]); 
    if (val <= threshold) {  // Si un pied est détecté
      if (!isPlaying[i]) {  // ET qu'on n'était pas déjà en train de jouer
        myDFPlayer.play(trackMapping[i]); // --> On joue le son UNE SEULE FOIS
        isPlaying[i] = true;  // --> On verrouille l'état
        Serial.println("STEP_ON:" + String(i + 1));
      }
    } else {
      if (isPlaying[i]) {
        isPlaying[i] = false; // --> On déverrouille quand le pied part
      }
    }
  }
  delay(50); // --> Petit délai
}