//Ludvig Ström 2018
//Firmware for the Dudeilator, an Arduino based machine that mixes White Russians
//If the machine is started with the button pressed it goes into calibration mode. 
//press the button once to start pump, once again to stop the pump. 

//one turn = 12800, 
//speed = 40uS/Step.
//one ml = 4266,7 steps

#define STEP_PIN 2
#define SLEEP_PIN_ONE 3
#define SLEEP_PIN_TWO 4
#define SLEEP_PIN_THREE 5
#define BUTTON_PIN 6

const int ONE_REV = 42667;
int buttonState = 0;  

void setup() { 
 
  pinMode(STEP_PIN, OUTPUT); 
  
  pinMode(SLEEP_PIN_ONE, OUTPUT);
  digitalWrite(SLEEP_PIN_ONE, LOW);
  
  pinMode(SLEEP_PIN_TWO, OUTPUT);
  digitalWrite(SLEEP_PIN_TWO, LOW);
  
  pinMode(SLEEP_PIN_THREE, OUTPUT);
  digitalWrite(SLEEP_PIN_THREE, LOW);

  pinMode(BUTTON_PIN, INPUT);

  //Check if button state is high, if so go into calibration mode
  buttonState = digitalRead(BUTTON_PIN);

  if(buttonState == HIGH){
    calibrate(SLEEP_PIN_ONE);
    calibrate(SLEEP_PIN_TWO);
    calibrate(SLEEP_PIN_THREE);
  }
  delay(3000);
  buttonState = LOW;
  //end of calibration mode
} 

void loop(){  
  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    makeAWhiteRussian();
  } 
}

void makeAWhiteRussian(){
    rotateAccel(4,2,6,25); //4 vodka, 2 kahula, 6 milk
}

long centilitreToSteps(int cl){
  long steps;
  steps = cl * 42667; //42667 pulses/centiLitre
  return  steps;
}

void rotateAccel(int centiLitreOne, int centiLitreTwo, int centiLitreThree, int accelLimit){ 
  
  long stepsOne;
  stepsOne = centilitreToSteps(centiLitreOne);
  
  long stepsTwo; 
  stepsTwo = centilitreToSteps(centiLitreTwo);
  
  long stepsThree; stepsThree = centilitreToSteps(centiLitreThree);

  long accelTime = 5000;
  float speedRightNow = 250; //start delay time
  
  //calculate how much to decrese delay each time
  float speedDecreseValue = (float)speedRightNow - accelLimit;
  speedDecreseValue = speedDecreseValue / accelTime;

  digitalWrite(SLEEP_PIN_ONE, HIGH);
  digitalWrite(SLEEP_PIN_TWO, HIGH);
  digitalWrite(SLEEP_PIN_THREE, HIGH);

  while(stepsOne > 0 || stepsTwo > 0 || stepsThree > 0){
    if(accelTime > 0 && speedRightNow > accelLimit){ //accelerate if suitible
      accelTime--;
      speedRightNow -= speedDecreseValue;
    }
  
    if(accelTime > 0 && speedRightNow > accelLimit){ //accelerate if suitible
      accelTime--;
      speedRightNow -= speedDecreseValue;
    }

    if(stepsOne > 0){
      stepsOne--;
    }
    
    if(stepsTwo > 0){
      stepsTwo--;
    }
    if(stepsThree > 0){
      stepsThree--;
    }

    if(stepsOne <= 0){
      digitalWrite(SLEEP_PIN_ONE, LOW);
    }
    
    if(stepsTwo <= 0){
      digitalWrite(SLEEP_PIN_TWO, LOW);
    }
    if(stepsThree <= 0){
      digitalWrite(SLEEP_PIN_THREE, LOW);
    }
    
     toggleStepPin(speedRightNow);
  }
}

void toggleStepPin(float delayTime){
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(delayTime); 
    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(delayTime); 
}

void calibrate(int sleepPin){
  
  buttonState = LOW;
  bool calibrating;

  //wait until button is pressed
  while(buttonState == LOW){
    buttonState = digitalRead(BUTTON_PIN);
  }
  
  buttonState = LOW;

  calibrating = true;

  delay(500);

  digitalWrite(sleepPin, HIGH);
  while(calibrating){

    toggleStepPin(75);

    //exit if button is pressed
    buttonState = digitalRead(BUTTON_PIN);
    if(buttonState == HIGH){
      calibrating = false;
    }
  }
  digitalWrite(sleepPin, LOW);
  delay(500);
}


