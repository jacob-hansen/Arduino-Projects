//stepper motors run best at 10rpm
// Current Wiring: 
//Stepper Motor 1 (top motor) wired to driver board wired to pins 8, 10, 11, 12 (1, 2, 3, 4)
//Stepper Motor 2 (bottom motor) wired to driver board wired to pins 2, 3, 4, 5, (1, 2, 3, 4)
//Pump 1  (fan side) wired to motor controller ports Out1, Out2 wired to arduino A3, A4
//Pump 2 (thermometer side) wired to motor controller ports Out4, Out3 wired to arduino A5, A6
//Thermometer and Humidity sensor wired to arduino 7, 5v power and ground
//Fan is wired to mosfet which is wired to arduino 6 PWM
//Heating pad is wired to arduino 9 PWM

#include <millisDelay.h>
#include <dht.h>
dht DHT;
#define DHT11_PIN 7

#include <Stepper.h>
int stepper1pin1 = 8;
int stepper1pin2 = 10;
int stepper1pin3 = 11;
int stepper1pin4 = 12;
int stepper2pin1 = 2;
int stepper2pin2 = 3;
int stepper2pin3 = 4;
int stepper2pin4 = 5;
Stepper stepper2(2048, stepper1pin1, stepper1pin3, stepper1pin2, stepper1pin4);
Stepper stepper1(2048, stepper2pin1, stepper2pin3, stepper2pin2, stepper2pin4);


//Initial Conditions:
int pumpFixTime = 2600; //time to run the pumps for fixing wells
int purgeTime = 8000;



int minHumidity = 80;
int minTemperature = 30;
int desiredTemp = 36; 
int heatPadPin = 9;

bool bubble_toggle1 = false; //Do you want reservoir 1 to have air blown through it (reservoir 1 is on the fan side)
bool bubble_toggle2 = false; //Do you want reservoir 2 to have air blown through it (reservoir 2 is on the themometer side)
bool pump2_toggle = false; //Set to true if you want to start with using pump 2

int fanPin = 6;
bool fan_toggle = true;
bool heat_toggle = false;





//Serial Receiving Values
char moveTo_1 = "a"; // well 1 (chamber 1, W1 (bottom right right chamnber))
char moveTo_2 = "b"; // well 2 (chamber 1, W2 (bottom left right chamber))
char moveTo_3 = "c"; // well 3 (chamber 1, W3)
char moveTo_4 = "d"; // well 4 (chamber 1, W4)
char moveTo_5 = "e"; // well 5 (chamber 1, W5)
char moveTo_6 = "f"; // well 6 (chamber 1, W6)
char moveTo_7 = "g"; // well 7 (chamber 1, W7)
char moveTo_8 = "h"; // well 8 (chamber 1, W8)
char moveTo_9 = "i"; // well 9 (chamber 2, W1)
char moveTo_10 = "j"; // well 10 (chamber 2, W2)
char moveTo_11 = "k"; // well 11 (chamber 2, W3)
char moveTo_12 = "l"; // well 12 (chamber 2, W4)
char moveTo_13 = "m"; // well 13 (chamber 2, W5)
char moveTo_14 = "n"; // well 14 (chamber 2, W6)
char moveTo_15 = "o"; // well 15 (chamber 2, W7)
char moveTo_16 = "p"; // well 16 (chamber 2, W8)

char purg_command = ":"; // well 16 (chamber 2, W8)

millisDelay delayStart;
int tempReportTimer = 60000; //How many milliseconds to wait in between regular temperature and humidity reports

char getTemperature = "?";
char getHumidity = "=";
int currentTemp;
int oldTemp;
int tempDiffCo = 255;
int tempDiffCoStorage = tempDiffCo;
int heatMultiFactor = 1;
int coolMultiFactor = 1;




char fan_on = "@";
char fan_off = "*";
char heat_on = "+";
char heat_off = "-";

bool shake_toggle = false;
char shake_begin = "r";
char shake_end = "s";

char bubble1_on = "w"; // start bubbling in reservoir connected to pump 1
char bubble1_off = "x";
char bubble2_on = "y"; // start bubbling in reservoir connected to pump 1
char bubble3_off = "z";




int pump1_onpin =  A3;
int pump1_offpin = A4;
int pump2_onpin =  A2;
int pump2_offpin = A1;


int shake_amplitude = 100;
int shake_speed = 18;

int step1_position1 = 0;
int step1_position2 = 0;
int step1_position9 = 0;
int step1_position10 = 0;
int step1_position3 = -650;
int step1_position4 = -650;
int step1_position11 = -650;
int step1_position12 = -650;
int step1_position5 = -1300;
int step1_position6 = -1300;
int step1_position13 = -1300; 
int step1_position14 = -1300;
int step1_position7 = -1950;
int step1_position8 = -1950;
int step1_position15 = -1950;
int step1_position16 = -1950;

int step2_position1 = 0;
int step2_position3 = 0;
int step2_position5 = 0;
int step2_position7 = 0;
int step2_position2 = -600;
int step2_position4 = -600;
int step2_position6 = -600;
int step2_position8 = -600;
int step2_position9 = -1400;
int step2_position11 = -1400;
int step2_position13 = -1400;
int step2_position15 = -1400;
int step2_position10 = -1830;
int step2_position12 = -1830;
int step2_position14 = -1830;
int step2_position16 = -1830;


int serial_test;
void setup() 
{
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  delay(500);
  Serial.begin(9600);
  stepper1.setSpeed(10);
  stepper2.setSpeed(10);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A6, LOW);

  analogWrite(fanPin, 255);

  delayStart.start(tempReportTimer);

}

void loop() 
{
  if (Serial.available())
    {
        serial_test = Serial.read();
        if (serial_test == 'a')
        {
           fixWells(step1_position1, step2_position1, 1);
        }

        if (serial_test == 'b')
        {
           fixWells(step1_position2, step2_position2, 2);
        }

        if (serial_test == 'c')
        {
           fixWells(step1_position3, step2_position3, 3);
        }

        if (serial_test == 'd')
        {
           fixWells(step1_position4, step2_position4, 4);
        }

        if (serial_test == 'e')
        {
           fixWells(step1_position5, step2_position5, 5);
        }

        if (serial_test == 'f')
        {
           fixWells(step1_position6, step2_position6, 6);
        }
        if (serial_test == 'g')
        {
           fixWells(step1_position7, step2_position7, 7);
        }

        if (serial_test == 'h')
        {
           fixWells(step1_position8, step2_position8, 8);
        }

        if (serial_test == 'i')
        {
           fixWells(step1_position9, step2_position9, 9);
        }
        if (serial_test == 'j')
        {
           fixWells(step1_position10, step2_position10, 10);
        }

        if (serial_test == 'k')
        {
           fixWells(step1_position11, step2_position11, 11);
        }

        if (serial_test == 'l')
        {
           fixWells(step1_position12, step2_position12, 12);
        }

        if (serial_test == 'm')
        {
           fixWells(step1_position13, step2_position13, 13);
        }

        if (serial_test == 'n')
        {
           fixWells(step1_position14, step2_position14, 14);
        }

        if (serial_test == 'o')
        {
           fixWells(step1_position15, step2_position15, 15);
        }
        if (serial_test == 'p')
        {
           fixWells(step1_position16, step2_position16, 16);
        }



        if (serial_test == 'u')
        {
           pump2_toggle = true;
           Serial.print('\n');
           Serial.print("Now using pump 2");
           
        }
        if (serial_test == 'v')
        {
           pump2_toggle = false;
           Serial.print('\n');
           Serial.print("Now using pump 1");
        }

        if (serial_test == 'w')
        {
           bubble_toggle1 = true;
           digitalWrite(pump1_offpin, HIGH);
           Serial.print('\n');
           Serial.print("Reservoir 1 bubbling turned on");
        }

        if (serial_test == 'x')
        {
           bubble_toggle1 = false;
           digitalWrite(pump1_offpin, LOW);
           Serial.print('\n');
           Serial.print("Reservoir 1 bubbling turned off");
        }


        if (serial_test == 'y')
        {
           bubble_toggle2 = true;
           digitalWrite(pump2_offpin, HIGH);
           Serial.print('\n');
           Serial.print("Reservoir 2 bubbling turned on");
        }

        if (serial_test == 'z')
        {
           bubble_toggle2 = false;
           digitalWrite(pump2_offpin, LOW);
           Serial.print('\n');
           Serial.print("Reservoir 2 bubbling turned off");
        }



        if (serial_test == '?')
        {
           Serial.print('\n');
           Serial.print("Temperature = ");
           int chk = DHT.read(DHT11_PIN);
           Serial.print(DHT.temperature);
        }

        if (serial_test == '=')
        {
           Serial.print('\n');
           Serial.print("Relative Humidity = ");
           int chk = DHT.read(DHT11_PIN);
           Serial.print(DHT.humidity);
        }



        if (serial_test == '@')
        {
           Serial.print('\n');
           
           if (fan_toggle == false){
              Serial.print("Fan turned on");
           }
           else {
              Serial.print("Fan is already  on");
           }
           fan_toggle = true;
           analogWrite(fanPin, 255);
        }

        if (serial_test == '*')
        {
           Serial.print('\n');
           if (fan_toggle == false){
              Serial.print("Fan is already off");
           }
           else {
              Serial.print("Fan turned off");
           }
           fan_toggle = false;
           analogWrite(fanPin, LOW);
        }



        if (serial_test == '+')
        {
           Serial.print('\n');
           
           if (heat_toggle == false){
              Serial.print("Temperature will now be kept above ");
              Serial.print(minTemperature);
           }
           else {
              Serial.print("Temperature is already kept above ");
              Serial.print(minTemperature);
           }
           heat_toggle = true;
        }

        if (serial_test == '-')
        {
           Serial.print('\n');
           if (heat_toggle == false){
              Serial.print("Temperature has not been controlled nor will ");
           }
           else {
              Serial.print("Temperature will no longer be controlled");
           }
           heat_toggle = false;
           analogWrite(heatPadPin, LOW);
        }
        if (serial_test == 'r')
        {
          shake_toggle = true;
          Serial.print('\n');
          Serial.print("Shaking Started");
          stepper2.setSpeed(shake_speed);
          while (shake_toggle == true){
            if (Serial.available()){
               serial_test = Serial.read();
               if (serial_test == ('s')){
                shake_toggle = false;
                turnSteppersOff();
                Serial.print('\n');
                Serial.print("Shaking Stopped");
                stepper2.setSpeed(10);
                delay(10);
               }
               else {
                Serial.print('\n');
                Serial.print("Still shaking, to stop enter 's'");
               }
            }
            stepper2.step(-shake_amplitude);
            stepper2.step(shake_amplitude);
            checkConditions();
          }
      }


      if (serial_test == ':'){
        Serial.print('\n');
        Serial.print("Purging Lines, please wait...");
        digitalWrite(pump1_offpin, LOW);
        digitalWrite(pump1_onpin, HIGH);
        digitalWrite(pump2_offpin, LOW);
        digitalWrite(pump2_onpin, HIGH);
        delay(purgeTime);
        digitalWrite(pump1_onpin, LOW);
        digitalWrite(pump1_offpin, HIGH);
        digitalWrite(pump2_onpin, LOW);
        digitalWrite(pump2_offpin, HIGH);
        delay(pumpFixTime);
        if (bubble_toggle1 == false) {
            digitalWrite(pump1_offpin, LOW);
         }
         if (bubble_toggle1 == false) {
            digitalWrite(pump2_offpin, LOW);
         }
         Serial.print('\n');
         Serial.print("Lines Purged and cleared, ready to run!");
      }
  }

  
  if (currentTemp < desiredTemp && heat_toggle == true) {
      analogWrite(heatPadPin, 255);
  }
  else {
    analogWrite(heatPadPin, LOW);
  }


  checkConditions();
  


  /* Code in progress to make the arduino more precisly control temperature
    if (heat_toggle == true){
    //if ((tempDiffCoStorage > 20 && heatMultiFactor > 0)or (tempDiffCoStorage){
    tempDiffCoStorage = tempDiffCoStorage + heatMultiFactor*(desiredTemp-currentTemp) - heatMultiFactor*(currentTemp - oldTemp);
    
    heatMultiFactor = 
    tempDiffCo = min(tempDiffCoStorage, 255);
    analogWrite(heatPadPin, tempDiffCo);
    oldTemp = currentTemp;
  }

  if (){
    
    delayStart = millis();   // start delay
    delayRunning = true;
  }*/
}
        
        

  //Serial.println(stepper1_position);
void fixWells(int stepper1Go, int stepper2Go, int wellNumber){
     if (pump2_toggle == false)
     {
        Serial.print('\n');
        Serial.print("fixing well ");
        Serial.print(wellNumber);
        Serial.print("... wait...");
        stepper1.step(stepper1Go);
        turnSteppersOff(); //new!!!!
        stepper2.step(stepper2Go);
        turnSteppersOff();
        digitalWrite(pump1_offpin, LOW);
        digitalWrite(pump1_onpin, HIGH);
        delay(pumpFixTime);
        digitalWrite(pump1_onpin, LOW);
        digitalWrite(pump1_offpin, HIGH);
        delay(pumpFixTime);
        if (bubble_toggle1 == false) 
          {
            digitalWrite(pump1_offpin, LOW);
          }
        stepper1.step(-stepper1Go);
        stepper2.step(-stepper2Go);
        Serial.print('\n');
        Serial.print("well "); Serial.print(wellNumber); Serial.print(" fixed from reservoir 1");
        int chk = DHT.read(DHT11_PIN);
        Serial.print("Conditions: Temperature "); Serial.print(DHT.temperature); Serial.print("°C, Humidity "); 
        Serial.print(DHT.humidity); Serial.print("%.");
        turnSteppersOff();

     }
     else
     {
        Serial.print('\n');
        Serial.print("fixing well ");
        Serial.print(wellNumber);
        Serial.print("... wait...");
        stepper1.step(stepper1Go);
        stepper2.step(stepper2Go);
        turnSteppersOff();
        digitalWrite(pump2_offpin, LOW);
        digitalWrite(pump2_onpin, HIGH);
        delay(pumpFixTime);
        digitalWrite(pump2_onpin, LOW);
        digitalWrite(pump2_offpin, HIGH);
        delay(pumpFixTime);
        if (bubble_toggle2 == false) 
          {
            digitalWrite(pump2_offpin, LOW);
          }
        stepper1.step(-stepper1Go);
        turnSteppersOff(); //New!!!!!
        stepper2.step(-stepper2Go);
        Serial.print('\n');
        Serial.print("well "); Serial.print(wellNumber); Serial.print(" fixed from reservoir 2");
        turnSteppersOff();
        int chk = DHT.read(DHT11_PIN);
        Serial.print("Conditions: Temperature "); Serial.print(DHT.temperature); Serial.print("°C, Humidity "); 
        Serial.print(DHT.humidity); Serial.print("%.");
     }
     delayStart.start(tempReportTimer);
}


void checkConditions(){
  if (delayStart.justFinished()){
    int chk = DHT.read(DHT11_PIN);
    currentTemp = DHT.temperature;
    delayStart.start(tempReportTimer);
    if (currentTemp >= minTemperature){
    Serial.print('\n');
    Serial.print("Current Conditions: Temperature "); Serial.print(DHT.temperature); Serial.print("°C, Humidity "); 
    Serial.print(DHT.humidity); Serial.print("%.");

      
    }
    else{
      Serial.print('\n');
      Serial.print("Warning, temperatures are below "); Serial.print(minTemperature); Serial.print("°C, Current Conditions: Temperature "); 
      Serial.print(DHT.temperature); Serial.print("°C, Humidity "); 
      Serial.print(DHT.humidity); Serial.print("%.");
      
    }
  }
}


void turnSteppersOff () {
  digitalWrite(stepper1pin1, LOW);
  digitalWrite(stepper2pin1, LOW);
  digitalWrite(stepper1pin2, LOW);
  digitalWrite(stepper2pin2, LOW);
  digitalWrite(stepper1pin3, LOW);
  digitalWrite(stepper2pin3, LOW);
  digitalWrite(stepper1pin4, LOW);
  digitalWrite(stepper2pin4, LOW);
  
}
