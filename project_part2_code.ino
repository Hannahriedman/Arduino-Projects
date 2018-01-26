/* author: Hannah Riedman
This code allows a user to set up their own alarm system. The sensor data is analog so 
whenever the sensorvalue is below 100, that means motion is detected by the sensor.
The intruder will get 3 warning alarms before the alarm is set off. The code handles this 
with the warningcount varible. Once the warningcount is greater than 3, the alarm code will
loop until the user resets the data. You can reset the data by toggling on and off the alarm with 
the button. 
*/


int sensor = A5; //  analog motion sensor port
int speaker = 11; // pin where the speaker is
int button = 8; // pin where the button is

int buttonvalue = 0; // digital value
int sensorvalue; //  analog value

long lastDebounceTime = 0;  // handles debouncing 
long debounceDelay = 200;  // handles debouncing

int warningcount = 0; // holds the amount of times an intruder has set off warning

boolean buttonpressed =  false; // if button has been pressed or not 
boolean systemOn = true; // if the system is on 

void setup() { // the only setup we need is to inilaize the LED pins and the button
  
  pinMode(button, INPUT); // initlize the button

  for(int pinNumber =2; pinNumber <5; pinNumber++){ //initalize the LEDs
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber,LOW);
  } 

}

void loop() {
  buttonvalue = digitalRead(button);
  buttonPressed();

 if (systemOn) { // if the system is on then detect motion 
     sensorvalue = analogRead(sensor);
  
    if (sensorvalue < 100) { // if motion is detected 
      while (warningcount > 3) { // if motion has been detected more than 3 times then set off alarm
        buttonvalue = digitalRead(button);
        buttonPressed(); // check to see if button is pressed 
        if (systemOn) { // if system has not been turned off, contunue the alarm
           alarm();
        } else { // if system has been turned off then reset warningcount to get out of loop
          lightsOFF(); // turn off the lights
          warningcount = 0;
        }
         
      }
      warningcount = warningcount+1; //increase the warningcount varible
      warning(); // set off a warning
      
    } else { // if no motion is detected 
       lightsON(); // when no motion is detected, the lights are reset to all on
    }
  }
 

}

void buttonPressed() { // this will analyze if the button is being pressed 
 

  if ( (millis() - lastDebounceTime) > debounceDelay) { // accounts for debouncing 
      if (buttonvalue == HIGH && buttonpressed == false) { // if button is pressed and system is on
        systemOn = false; // set system to off
        lightsOFF(); // turn off all the lights
        buttonpressed = true; // set buttonpressed to true so next button press will turn system on
        lastDebounceTime = millis(); //set the current time
      } else if (buttonvalue == HIGH && buttonpressed == true) { // if button was pressed and system was previoulsy turned off 
        systemOn = true; // turn on system
        lightsON(); // turn on the all the lights
        buttonpressed = false; // set buttonpressed to false so next button press will turn system off
        warningcount = 0; // reset warningcount
        lastDebounceTime = millis(); // set the current time
      }
  }
  
}

void alarm() { // serious alarm when someone has taken snack
    digitalWrite(2,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(3,HIGH);
    tone(speaker,900,200);
    delay(200);
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
    digitalWrite(3,LOW);
    tone(speaker,700,200);
    delay(200); 
}


void warning() { // what to do when sensor is tripped 
    digitalWrite(2,HIGH);
    digitalWrite(4,HIGH);
    tone(speaker,500,200);
    delay(200);
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
    digitalWrite(3,HIGH);
    tone(speaker,200,200);
    delay(200);
}


void lightsON() { // this turns on all the LEDS
    for(int pinNumber =2; pinNumber <5; pinNumber++){ 
       digitalWrite(pinNumber,HIGH);
    }
}

void lightsOFF() { // this turns off all the LEDS
    for(int pinNumber =2; pinNumber <5; pinNumber++){ 
       digitalWrite(pinNumber,LOW);
    }
}
