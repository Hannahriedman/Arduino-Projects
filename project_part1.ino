#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,7,6,5,4);
int button1 = 0; 
int button2 = 0;
int backlight = 9; // pin that changes the backlight 
int brightnessnum = 5000; // number that holds a brightness value

long lastDebounceTime = 0;  
long debounceDelay = 500; 

int count = 0; // count to count where you are in the string 
String num1 = ""; // varible to hold the first binary number 
String num2 = ""; // varible to hold the second binary number
int result = 0; // varible to hold the result of the two numbers

String num = "00000000"; // inital string for the binary number

boolean start =0; // if the user has started from inital screen yet
boolean secondnum = 0; // how to tell if the user is working on entering second number

void setup() {
  lcd.begin(16, 2);
  pinMode(backlight, OUTPUT); // seting pinmode for brightness
  pinMode(2, INPUT); // setting pinmode for button 1
  pinMode(3, INPUT); // setting pinmode for button 2

  //inital splash screen
  lcd.print("Press Both");
  lcd.setCursor(0,1);
  lcd.print("Buttons to start!");
  Serial.begin(9600);
  analogWrite(backlight, brightnessnum);
   
}

void loop() {
  button1 = digitalRead(2);
  button2 = digitalRead(3);
  
  if (start == true) { // if you have already completed the start screen then continue
     
      if ( (millis() - lastDebounceTime) > debounceDelay) { // debouncing
        
          if ((button1 == HIGH) && (button2 == LOW)) {  // button 1 pressed
            changenum();   // Change Number        
            
          } else if ((button1 == LOW) && (button2 == HIGH)) { // button 2 pressed
            movecursor();  // Move cursor 
            
          } else if ((button1 == HIGH) && (button2 == HIGH)) { // button 1 & 2 pressed
            brightness();  // Go to brightness screen
                 
          } 
            
       }
          
  } else { // go to start up screen 
    startup();
  }
  
}

void startup() { // initalizes the calculator 
  
    if ( (millis() - lastDebounceTime) > debounceDelay) {
      if ((button1 == HIGH) && (button2 == HIGH)) { // button 1 & 2 pressed
        start=true;
        lcd.setCursor(0,0);
        lcd.clear();
        lcd.print(num);
        lastDebounceTime = millis(); //set the current time

      }
    }
    
}

void changenum() { // changes current number cursor is at 
  lcd.clear();
  lcd.home();
  lcd.print(num);
     
  if ( num.charAt(count) == '0') { // if the char is 0 then it will change to 1
      
       num.setCharAt(count, '1');
       lcd.clear();
       lcd.home();
       lcd.print(num);
       
  } else if ( num.charAt(count) == '1') { // if it is 1 then it will change it to 0   
       num.setCharAt(count, '0');
       lcd.clear();
       lcd.home();
       lcd.print(num);
  }
  lastDebounceTime = millis(); //set the current time
  
}

void movecursor() { // moves cursor to the right 
  
  if ((count == 7)&& (secondnum == 0)) { // if it has reached the end of the first number then 
      num1 = num; // it will input the number into a varible 
      count = 0; // set the counter to 0 
      num = "00000000"; // set the num to 0
      lcd.clear(); // reload the screen >
      lcd.home();
      lcd.print(num);
      secondnum = true; // now it will complete the second number
            
  } else if ((count == 7) && (secondnum == 1)){ // if it has reached end of second number then
      num2 = num; // it will input the number into a varible
      math(); // will now do math with binary numbers
      
  } else { // move the cursor one over by increasing "count"
      count++;
  }
  lastDebounceTime = millis(); //set the current time

}

void math() { // does the binary math & shows result
  int carry=0;  // variable to account for carry
  int myresult[9]; // array to hold the result
  String scope1 = "n"; // varible to hold individual slot in number 1
  String scope2 = "n"; // varible to hold individual slot in number 2
  
  for (int i=7;i>-1;i--){ // will go through binary numbers, number by number
    scope1.setCharAt(0,(num1.charAt(i))); // grab the number in the slot, i, of number 1
    scope2.setCharAt(0,(num2.charAt(i))); // grab the number in the slot, i, of number 2
    
    if (scope1.toInt() & scope2.toInt()) { // if both are 1
      if (carry) { // check to see if there is a carry
        myresult[i]=1;
        carry= 1;
      } else {
        myresult[i]=0;
        carry= 1;
      }
    } else if (scope1.toInt()  | scope2.toInt()) { // if one is 1
      if (carry) {
        myresult[i]=0;
        carry= 1;
      } else {
        myresult[i]=1;
        carry= 0;
      }
    } else { // if both are 0
      if (carry) {
        myresult[i]=1;
        carry= 0;
      } else {
        myresult[i]=0;
        carry= 0;
      }
    }
    if ((i == 0) && (carry)) { // to account for additonanl carry 
          myresult[9]=1;
        }
  }
  
  lcd.clear();  // show result screen >
  lcd.setCursor(0,0);
  lcd.print("RESULT:");
  lcd.setCursor(0,1);
  lcd.print(myresult[9]);
  for (int i = 0; i < 8; i++) { // displays the result on LCD
    lcd.setCursor(i+1,1);
    lcd.print(myresult[i]);
  }

  lastDebounceTime = millis(); // sets the current time
  initalize(); // resets variables 
  startup(); // restarts program 
    
}

void initalize() { // resets all the varibles so you can start over  
    
    count = 0; 
    num1 = "";
    num2 = ""; 
    result = 0; 
    num = "00000000"; 
    start = 0; 
    secondnum = 0;
}

void brightness() { // brightness settings
  
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("SET BRIGHTNESS");
    lcd.setCursor(0,1);
    lcd.print("1:dec  2:inc");
    delay(500);
    
  while (true) {
    
    button1 = digitalRead(2);
    button2 = digitalRead(3);
    
    if  ((millis() - lastDebounceTime) > debounceDelay) {
      
      if ((button1 == HIGH) && (button2 == HIGH)){ // breaks out of brightness screen
          lcd.clear();
          lcd.home();
          lcd.print(num);
          lastDebounceTime = millis();//set the current time
          break;
          
      }else if ((button1 == HIGH) && (button2 == LOW)) { // decrease brightness 
          if (brightnessnum > 500) { // checking to see if brightness has reached min
            brightnessnum -= 1500;
          } 
          analogWrite(backlight,(brightnessnum));
          lastDebounceTime = millis(); //set the current time
          
      } else if ((button1 == LOW) && (button2 == HIGH)) { // increase brightness
          if (brightnessnum < 9500) { // checking to see if brightness has reached max
            brightnessnum += 1500;
          }
          analogWrite(backlight,(brightnessnum));
          lastDebounceTime = millis(); //set the current time
          
      } 
  
    }
  }
   
}

