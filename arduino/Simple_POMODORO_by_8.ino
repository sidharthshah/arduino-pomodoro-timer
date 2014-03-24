//Vars relating to IO
int START_PIN = 10;
int DONE_PIN = 9;
int WIP_PIN = 8; 
int BUZZER_PIN = 13;
int BUTTON_PIN = 2; 

//Vars related to Button
int buttonState = HIGH;
int lastButtonState = HIGH; 
long lastDebounceTime = 0;  // the last time the output pin was toggled
//long debounceDelay = 1000;    // the debounce time; increase if the output flickers
long debounceDelay = 125;

//Vars related to Timer
long startTime;
//long TIMEOUT = 1500000;
long TIMEOUT = 187500; 
/*
  There will be three states
 -1: Reset
 0: Counting Down Start
 1: Countdown In Progress
 2: Interrupted  
 */
int timerState;

void setup(){
  pinMode(START_PIN, OUTPUT);
  pinMode(DONE_PIN, OUTPUT);
  pinMode(WIP_PIN, OUTPUT);  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  timerState = -1;
}

void shortBeep(){
  digitalWrite(BUZZER_PIN, HIGH);
  //  delay(250);
  delay(31.25);
  digitalWrite(BUZZER_PIN, LOW);
  //delay(250);  
  delay(31.25);
}

void longBeep(){
  digitalWrite(BUZZER_PIN, HIGH);
  //  delay(2000);
  delay(250);
  digitalWrite(BUZZER_PIN, LOW);
  // delay(2000);
  delay(250); 
}

void blinkPin(int PIN){
  digitalWrite(PIN, HIGH);
  //delay(500);
  delay(62.5);

  digitalWrite(PIN, LOW);
  //  delay(500);
  delay(62.5);
}

void waitingState(){
  blinkPin(START_PIN);
}

void wipState(){
  blinkPin(WIP_PIN);
}

void resetState(){
  digitalWrite(DONE_PIN, HIGH);
  longBeep();
  digitalWrite(DONE_PIN, LOW);
  timerState = -1;
}

void loop(){
  int reading = digitalRead(BUTTON_PIN);

  if(reading != lastButtonState){
    lastDebounceTime = millis();
  }  

  if((millis() - lastDebounceTime) > debounceDelay){
    if (reading != buttonState){
      buttonState = reading; 

      if (timerState <= 1){
        timerState++;
        shortBeep();
      }
      else{
        resetState();
      }
    }
  }

  if(timerState == -1){
    waitingState();
  }

  if(timerState == 0){
    startTime = millis();
  }

  if(timerState == 1){
    wipState();

    //Reset timer when clock runs out
    if((millis() - startTime) > TIMEOUT){
      resetState();
    }
  }

  lastButtonState = reading; 
}

