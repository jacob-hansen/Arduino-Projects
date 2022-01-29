#include <Stepper.h>
#define STEPS 32
Stepper stepper(STEPS, 9,11, 10, 12);
 
bool state = 0;
bool previousState = 0;
int count = 0;
long startTime = 0;
int reset = 4000;
long endTime = startTime + reset;
bool doorOpened = 0;
long unlockedTime = 0;
int splitTime = 400; //how long in between signals to wait before resetting. 
int codeNum1 = 3; //Has to be at least 2
int codeNum2 = 4;
int code1Detected = false;
int code2Detected = false;
long resetTimer = startTime + splitTime;
int sum; 
int average; 
const int listLength = 10;
int storageList[listLength];
int largeStorageList[500];
bool darkProbe = false; 
int darkCount = 0;
int diff[listLength];
int sumX;
int averageX;
int diffSum;
int diffXTotal; 
int slope;
int counter = 0;
 
void setup() {
pinMode(A5, INPUT);
pinMode(4, OUTPUT);//red
pinMode(5, OUTPUT);//green
Serial.begin(9600);
}
 
void loop() { 
    int val = analogRead(A5); // Check the Value of the Light Sensor 
    for (int i = 0; i < listLength-1; i++) {
        storageList[i] = storageList[i+1];
    } // storing values over time to remove background noise
    storageList[listLength-1] = val;
    darkProbe = true; // when lights are off outside, darkProbe and Count keep the door from unlocking
    darkCount = 0; 
    for (int i = 0; i < 500; i++) {
        largeStorageList[i] = largeStorageList[i+1];
        if (val > 20){
          darkProbe = false; // there is sufficient light 
        }
    }
    largeStorageList[99] = val;

    // the following is a modified version of simple linear regression
    // these alterations were found just through testing and don't have clear rhyme or reason
    if (counter > listLength){
        for (int i = 0; i < listLength; i++){
           sum += storageList[i];
        }
        for (int i = 1; i < listLength+1; i++){
           sumX += i;
        }
        averageX = int(sumX/listLength);
        average = int(sum/listLength);
        for (int i = 0; i < listLength; i++){
           diff[i] = (averageX-i)*(average-storageList[i])*10;
        }
        
        for (int i = 0; i < listLength; i++){
           diffSum += (diff[i]);
        }
        
        for (int i = 0; i < listLength; i++){
           diffXTotal += (averageX-i)*(averageX-i);
        }
        
        slope = -diffSum/val;
        if (slope < 0){
           slope = 0;
        }
        if (darkProbe == true){
           slope = 0;
        }
        
        Serial.println(slope);
        counter = 0;
        sum = 0;
        sumX = 0;
        averageX = 0;
        average = 0;
        for (int i = 0; i < listLength; i++){
          diff[i] = 0;
        }
        diffSum = 0;
        diffXTotal = 0;
    }
    counter += 1; 
    
    //actions to be taken depending on the state
    delay(3);
    if(slope > 3) state = 1;
    if(slope <= 3) state = 0;
    if ((state == 1)&&(previousState != state)){
        if (count == 0) {
            startTime = millis();
            resetTimer = startTime + reset;
            endTime = startTime + splitTime;
        }
        if(millis()<endTime) {
             if ((code1Detected == false) && (codeNum1 > 1) && (count == 1)){
               splitTime = millis()-startTime+100;
             }
             count++;
             Serial.print(count);
             Serial.println("detected count:");
        }
      
        startTime = millis();
        resetTimer = startTime + reset;
        endTime = startTime + splitTime;
        Serial.println("count");
        Serial.print(count);
    }
    // detecting the correct code being entered
    if((millis()>endTime)&&(count!= 0)) {
        if ((count == codeNum1)&&(code1Detected == false)){
            code1Detected = true;
            Serial.println("code 1 detected");
            count = 0;
        }
        else if ((count == codeNum2)&&(code1Detected == true)){
            code2Detected = true;
            Serial.println("code 2 detected");
            count = 0;
        }
        else{
            code1Detected = false;
            code2Detected = false;
            Serial.println("not the code");
            splitTime = 1000;
            count = 0;
        }
    }  
    // reset the time for entering the code
    // can't have too long of a delay
    if ((millis()>resetTimer)&&(count!=0)) {
        startTime = millis();
        resetTimer = startTime + reset;
        endTime = startTime + splitTime;
        code1Detected = false;
        code2Detected = false;
        splitTime = 1000;
        count = 0;
        Serial.println("complete reset");
    }
    previousState = state;
    
    // code was entered correctly 
    if((code2Detected == true)&&(!doorOpened)){
        code1Detected = false;
        code2Detected = false;
        digitalWrite(5, HIGH);
        stepper.setSpeed(800);
        stepper.step(532);
        AllOff();
        doorOpened = 1;
        Serial.println(splitTime);
        splitTime = 1000;
        unlockedTime = millis();
        ledSequence();
    }
    // close the door again 
    if((doorOpened)&&(millis()-unlockedTime>7000)){
        stepper.setSpeed(500);
        stepper.step(-532);
        AllOff();
        doorOpened = 0;
        digitalWrite(4,LOW);
    }
}


void ledSequence(){
  // sequence for shining the lights 
  //red 3 green 5
  for(int i = 0; i<5; i++){
      digitalWrite(5,HIGH);
      delay(100);
      digitalWrite(5,LOW);
      delay(100);
  }
  digitalWrite(4,HIGH);
  delay(1000);
 
}
void AllOff(){
    //turn all stepper motor pins off
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
 }
