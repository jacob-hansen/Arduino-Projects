// Expected Voltage: 12v
//Camera run seperatly
//Enter camands (letter)# 
//Letter is f for fan, l for lights, and +/- to control camera focus

//#include <millis.h>
//millisDelay tempReportTimer;
#include <dht11.h>

dht11 DHT;
#define DHT11_PIN 4
int tempPin = 4;

//int tempReportTime = 6000; //How many milliseconds to wait in between regular temperature and humidity reports
//unsigned long previousMillis = 0;
//bool tempReportStatus = false;

//Thermometor pin is port 4
//Lights Pin is port 5
// Heat Pad pin is port 3
//Fly Fan pin is port 6, runs best 80-255

int lightPort = 5;
int flyFanPort = 6; 
int heatPort = 3; 
int chk;
#include <Stepper.h>
int stepper1pin1 = 12;
int stepper1pin2 = 11;
int stepper1pin3 = 10;
int stepper1pin4 = 9;
Stepper stepper1(2048, stepper1pin1, stepper1pin3, stepper1pin2, stepper1pin4);

char serial_test;

int cameraTurnDistance = 1000;

//initial conditions
int fanLevel = 0;
int heatPadLevel = 0;
int lightsLevel = 0;



void setup() {
  // put your setup code here, to run once:
  delay(5000);
  pinMode(lightPort, OUTPUT);
  pinMode(flyFanPort, OUTPUT);
  pinMode(heatPort, OUTPUT);
  pinMode(stepper1pin1, OUTPUT);
  pinMode(stepper1pin2, OUTPUT);
  pinMode(stepper1pin3, OUTPUT);
  pinMode(stepper1pin4, OUTPUT);
  pinMode(tempPin, INPUT);
  stepper1.setSpeed(7);
  Serial.begin(9600);
}

void loop() {
  
  if (Serial.available())
    {
        serial_test = Serial.read();
        
        /*if (serial_test == 'r')
        {
          // 'f' calls a change in fly fan speed
          tempReportStatus = true;
          //tempReportTimer.start(tempReportTime);
        }

        if (serial_test == 's')
        {
          // 'f' calls a change in fly fan speed
          tempReportStatus = false;
        }*/
        
        
        if (serial_test == 'f')
        {
          // 'f' calls a change in fly fan speed
          setSpeedTo('f', flyFanPort);
        }
        
        else if (serial_test == 'l')
        {
          // 'l' calls a change in light brightness
          setSpeedTo('l', lightPort);
        }

        else if (serial_test == 'h')
        {
          // 'h' calls a change in the heat pad level
          setSpeedTo('h', heatPort);
        }

        else if (serial_test == '+')
        {
        //'+' calls stepper motor to adjust for higher pressure focus
            moveCamera(true);
            Serial.print("registered");
        }

        else if (serial_test == '-')
        {
        //'-' calls stepper motor to adjust for lower pressure focus
            moveCamera(false);
        }

        else if (serial_test == '?')
        {
         // '?' calls checking the temperature
           Serial.print('\n');
           Serial.print("Temperature = ");
           int chk = DHT.read(DHT11_PIN);
           Serial.print(DHT.temperature);
        }

        else if (serial_test == '=')
        {
           Serial.print('\n');
           Serial.print("Relative Humidity = ");
           int chk = DHT.read(DHT11_PIN);
           Serial.print(DHT.humidity);
        }
        
    }



 // optional rempReportStatus 
 /* if(tempReportStatus == true){
    checkConditions();
  }*/
}




void moveCamera (bool direct)
{
    if (direct)
    {
        stepper1.step(cameraTurnDistance);
        digitalWrite(stepper1pin1, LOW);
        digitalWrite(stepper1pin2, LOW);
        digitalWrite(stepper1pin3, LOW);
        digitalWrite(stepper1pin4, LOW);
        Serial.print("+");
    }
    else
    {
        stepper1.step(-cameraTurnDistance);
        
        digitalWrite(stepper1pin1, LOW);
        digitalWrite(stepper1pin2, LOW);
        digitalWrite(stepper1pin3, LOW);
        digitalWrite(stepper1pin4, LOW);
        Serial.print("-");
    }
}




void setSpeedTo(char device, int pin) {
  //Expected Input of what device is to be set at a different Power (either fan, lights, or heat pad)
  //Stays in a loop until an expected value 0-9 is given 
  Serial.print('\n');
  Serial.print("Please enter ");
  Serial.print(device);
  Serial.print(" strength 1-9 (0 for off, x to escape):");
  while (Serial.available())
  {
    char waste = Serial.read();
    Serial.print("waste");
  }

  while (true) {
      if (Serial.available())
      {
            serial_test = Serial.read();
            Serial.print("serial was available");
            Serial.print(serial_test);
            Serial.print(".");
            if (device == 'f')
            {
                if (serial_test == '0')
                {
                    analogWrite(pin, LOW);
                    fanLevel = 0;
                }
                else if (serial_test == '1')
                {
                    analogWrite(pin, 55);
                    fanLevel = 1;
                }
                else if (serial_test == '2')
                {
                    analogWrite(pin, 80);
                    fanLevel = 2;
                }
                else if (serial_test == '3')
                {
                    analogWrite(pin, 105);
                    fanLevel = 3;
                }
                else if (serial_test == '4')
                {
                    analogWrite(pin, 130);
                    fanLevel = 4;
                }
                else if (serial_test == '5')
                {
                    analogWrite(pin, 155);
                    fanLevel = 5;
                }
                else if (serial_test == '6')
                {
                    analogWrite(pin, 180);
                    fanLevel = 6;
                }
                else if (serial_test == '7')
                {
                    analogWrite(pin, 205);
                    fanLevel = 7;
                }
                else if (serial_test == '8')
                {
                    analogWrite(pin, 230);
                    fanLevel = 8;
                }
                else if (serial_test == '9')
                {
                    analogWrite(pin, 255);
                    fanLevel = 9;
                }
                else if (serial_test == 'x')
                {
                    Serial.print('\n');
                    Serial.print("Fan speed left at ");
                    Serial.print(fanLevel);
                    break;
                }
                else
                {
                    Serial.print('\n');
                    Serial.print("Command not understood.");
                    setSpeedTo(device, pin);
                    break;
                }

                Serial.print('\n');
                Serial.print("Fan speed set to ");
                Serial.print(fanLevel);
                break;
            }


            if (device == 'l')
            {
                if (serial_test == '0')
                {
                    analogWrite(pin, LOW);
                    lightsLevel = 0;
                }
                else if (serial_test == '1')
                {
                    analogWrite(pin, 15);
                    lightsLevel = 1;
                }
                else if (serial_test == '2')
                {
                    analogWrite(pin, 45);
                    lightsLevel = 2;
                }
                else if (serial_test == '3')
                {
                    analogWrite(pin, 75);
                    lightsLevel = 3;
                }
                else if (serial_test == '4')
                {
                    analogWrite(pin, 105);
                    lightsLevel = 4;
                }
                else if (serial_test == '5')
                {
                    analogWrite(pin, 135);
                    lightsLevel = 5;
                }
                else if (serial_test == '6')
                {
                    analogWrite(pin, 165);
                    lightsLevel = 6;
                }
                else if (serial_test == '7')
                {
                    analogWrite(pin, 195);
                    lightsLevel = 7;
                }
                else if (serial_test == '8')
                {
                    analogWrite(pin, 225);
                    lightsLevel = 8;
                }
                else if (serial_test == '9')
                {
                    analogWrite(pin, HIGH);
                    lightsLevel = 9;
                }
                else if (serial_test == 'x')
                {
                    Serial.print('\n');
                    Serial.print("Light strength left at ");
                    Serial.print(fanLevel);
                    break;
                }
                else
                {
                    Serial.print('\n');
                    Serial.print("Command not understood.");
                    setSpeedTo(device, pin);
                    break;
                }

                Serial.print('\n');
                Serial.print("Light strength set to ");
                Serial.print(fanLevel);
                break;
            }

            if (device == 'h')
            {
                if (serial_test == '0')
                {
                    analogWrite(pin, LOW);
                    heatPadLevel = 0;
                }
                else if (serial_test == '1')
                {
                    analogWrite(pin, 15);
                    heatPadLevel = 1;
                }
                else if (serial_test == '2')
                {
                    analogWrite(pin, 30);
                    heatPadLevel = 2;
                }
                else if (serial_test == '3')
                {
                    analogWrite(pin, 45);
                    heatPadLevel = 3;
                }
                else if (serial_test == '4')
                {
                    analogWrite(pin, 60);
                    heatPadLevel = 4;
                }
                else if (serial_test == '5')
                {
                    analogWrite(pin, 75);
                    heatPadLevel = 5;
                }
                else if (serial_test == '6')
                {
                    analogWrite(pin, 90);
                    heatPadLevel = 6;
                }
                else if (serial_test == '7')
                {
                    analogWrite(pin, 195);
                    heatPadLevel = 7;
                }
                else if (serial_test == '8')
                {
                    analogWrite(pin, 120);
                    heatPadLevel = 8;
                }
                else if (serial_test == '9')
                {
                    analogWrite(pin, 135);
                    heatPadLevel = 9;
                }
                else if (serial_test == 'x')
                {
                    Serial.print('\n');
                    Serial.print("Heat pad level left at ");
                    Serial.print(fanLevel);
                    break;
                }
                else
                {
                    Serial.print('\n');
                    Serial.print("Command not understood.");
                    setSpeedTo(device, pin);
                    break;
                }

                Serial.print('\n');
                Serial.print("Heat pad level set to ");
                Serial.print(fanLevel);
                break;
            }
            break; 
    }
  }
}



// Optional Periodic Condition Reporter
/*void checkConditions(){
  if (millis() > (previousMillis + tempReportTime)){
    Serial.print("Success");
    int chk = DHT.read(DHT11_PIN);
    Serial.print('\n');
    Serial.print("Current Conditions: Temperature "); Serial.print(DHT.temperature); Serial.print("°C, Humidity "); 
    Serial.print(DHT.humidity); Serial.print("%.");
    //tempReportTimer.start(tempReportTimer);
    previousMillis = millis();
  }
}*/




  
