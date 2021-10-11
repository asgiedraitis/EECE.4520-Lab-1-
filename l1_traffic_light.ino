/*
 * Alden Giedraitis
 * EECE.4520
 * Lab 1
 * Description: LED traffic controller with speaker feedback. 
 */

//Defined LED pins

int r = 2; //red LED pin
int y = 4; //yellow LED pin
int g = 7; //green LED pin

//Defined buzzer and button values

int buzz = 6; 

int button = 8;

int bState = 0; //Button state

//Timer variables

unsigned long startT; //Beginning time stamp

unsigned long currT; //Current time stamp

const unsigned long rg_delay = 20000; //Delay for red and green lights in ms

const unsigned long yb_delay = 3000; //Delay for yellow light and buzzer in ms


void setup() {

  Serial.begin(9600);

  pinMode(r,OUTPUT);
  pinMode(y,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(buzz,OUTPUT);
  pinMode(button,INPUT); //Button used for feedback

  startT = millis(); //Get the initial time stamp 
}


void changePhase(int curr,int next,const unsigned long del){

  currT = millis(); //Grab current time to begin count

  //Keep the current light on until the delay has elapsed
  
  while( (currT - startT) <= del){
      Serial.print("Switching in:");

      Serial.print(del - (currT-startT));

      Serial.println();
    
    digitalWrite(curr, HIGH);
    
    currT = millis(); //Get current timestamp

    //Make the speaker sound 3 seconds before next light

    //If desired delay - elapsed time <3, sound the buzzer 
    if ( del - (currT - startT) < yb_delay){
      
      tone(buzz,1000); //1 kHz sound
      delay(500); //1 second
      noTone(buzz);
      delay(500);
      
    }
    
  }

  //Turn off current light, turn on next light
  digitalWrite(curr,LOW);
  digitalWrite(next,HIGH);

  //Reset beginning time

  startT = millis();
  
}

void checkPowerOn(){
  //If button isn't pressed yet, blink the red LED on and off, 1 second between states
  while (bState == LOW){

    Serial.println("No power!");
    bState = digitalRead(button);
    digitalWrite(r,HIGH);
    delay(1000);
    digitalWrite(r,LOW);
    delay(1000);
  }
  Serial.println("Powered on!");
}

void loop() {

 

  //POWERON:
  //==========================================================

  //Red flashes 1 second on, 1 second off,wait for button press

  checkPowerOn();

  //==========================================================



  //RED LIGHT PHASE:
  //==========================================================

  //Red light on for 20 seconds

  startT = millis();

  changePhase(r,g,rg_delay);

  //==========================================================




  //GREEN LIGHT PHASE:
  //==========================================================

  //Green light on for 20 seconds

  changePhase(g,y,rg_delay);

  //==========================================================



  //YELLOW LIGHT PHASE
  //==========================================================

  //Yellow light on for 3 seconds

  changePhase(y,r,yb_delay);

  //==========================================================
  

}
