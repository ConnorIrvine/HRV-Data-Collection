#include <Arduino.h>s

//Definitions   
const int HR_RX = 7; 
byte oldSample, sample; 

// BPM calculation variables
unsigned long lastBeatTime = 0;
unsigned long currentBeatTime = 0;
unsigned long beatInterval = 0;
int BPM = 0;
const int numReadings = 5;  // Number of readings to average
int bpmReadings[numReadings];
int readIndex = 0;
int bpmTotal = 0;
int bpmAverage = 0;

void setup() { 
  Serial.begin(9600); 
  pinMode (HR_RX, INPUT);  //Signal pin to input   
  Serial.println("Waiting for heart beat..."); 
  
  // Initialize BPM readings array
  for (int i = 0; i < numReadings; i++) {
    bpmReadings[i] = 0;
  }
  
  //Wait until a heart beat is detected   
  while (!digitalRead(HR_RX)) {};
  Serial.println ("Heart beat detected!");
  lastBeatTime = millis();
} 

void loop() { 
  sample = digitalRead(HR_RX);  //Store signal output  
  
  if (sample && (oldSample != sample)) { 
    // Beat detected
    currentBeatTime = millis();
    beatInterval = currentBeatTime - lastBeatTime;
    lastBeatTime = currentBeatTime;
    
    // Calculate BPM from interval (60000 ms = 1 minute)
    BPM = 60000 / beatInterval;
    
    // Add to running average
    bpmTotal = bpmTotal - bpmReadings[readIndex];
    bpmReadings[readIndex] = BPM;
    bpmTotal = bpmTotal + bpmReadings[readIndex];
    readIndex = (readIndex + 1) % numReadings;
    bpmAverage = bpmTotal / numReadings;
    
    // Output results
    Serial.print("Beat | BPM: ");
    Serial.print(BPM);
    Serial.print(" | Average BPM: ");
    Serial.println(bpmAverage);
  } 
  oldSample = sample;           //Store last signal received  
}