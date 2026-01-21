#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int LDR_PINS[] = { A0, A1, A2, A3 };
const int NB_STEPS = 4;

int trackMapping[] = { 1, 2, 3, 4 };
bool isPlaying[] = { false, false, false, false };
int threshold = 800;

SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true)
      ;
  }
  myDFPlayer.volume(30);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if (data.startsWith("M")) {
      int stepIdx = data.substring(1, 2).toInt() - 1;
      int trackNum = data.substring(3).toInt();
      if (stepIdx >= 0 && stepIdx < NB_STEPS) {
        trackMapping[stepIdx] = trackNum;
      }
    }
  }
    for (int i = 0; i < NB_STEPS; i++) {
    int val = analogRead(LDR_PINS[i]);
    if (val <= threshold) {
      if (!isPlaying[i]) {
        myDFPlayer.play(trackMapping[i]);
        isPlaying[i] = true;
        Serial.println("STEP_ON:" + String(i + 1));
      }
    } else {
      if (isPlaying[i]) {
        isPlaying[i] = false;
      }
    }
  }
  delay(50);
}