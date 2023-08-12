#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#define interruptPin PB1 //Pin we are going to use to wake up the Arduino
#define LED PB4
enum states {Standby, Light5, Light10, Light15, Candle};
volatile int mode = Standby;
volatile bool back = false;

void setup() {
  pinMode(interruptPin, INPUT_PULLUP); //Set pin d2 to input using the buildin pullup resistor
  pinMode(LED, OUTPUT);
  pinMode(PB0, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ButtonPressed, LOW);//attaching a interrupt to pin d2
}

void loop() {
  back = false;
  switch (mode) {
    case Standby: {
        disableLEDs();
        Going_To_Sleep();
        break;
      }
    case Light5: {
        digitalWrite(LED, HIGH);
        delay_s(300);
        decideReason();
        break;
      }
    case Light10: {
        digitalWrite(LED, HIGH);
        delay_s(600);
        decideReason();
        break;
      }
    case Light15: {
        digitalWrite(LED, HIGH);
        delay_s(900);
        decideReason();
        break;
      }
    case Candle: {
        while(!back){
          analogWrite(PB0, (uint8_t) (rand() % 150)+100);
          delay(50);
        }
        analogWrite(PB0, 0);
        decideReason();
        break;
      }
    default: {
        mode = Standby;
      }
  }
  
  disableLEDs();
  _delay_ms(100);
}

//more then a second
void delay_s(int s) {
  for (int i = 0; i < s && !back; i++) {
      _delay_ms(1000);
  }
}

void disableLEDs() {
  digitalWrite(LED,LOW);
  digitalWrite(PB0,LOW);
}

void decideReason(){
  disableLEDs();
  if(!back){
    mode= Standby;
  }
}

void Going_To_Sleep() {
  disableLEDs();
  sleep_enable();//Enabling sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
  _delay_ms(1000);
  sleep_cpu();//activating sleep mode
}

void ButtonPressed() {
  detachInterrupt(digitalPinToInterrupt(interruptPin)); //Removes the interrupt from pin;
  sleep_disable();//Disable sleep mode
  
  //Simple debouncing
  _delay_ms(25);
  if (digitalRead(interruptPin) == LOW) {
    while (digitalRead(interruptPin) == LOW) {
      ;
    }
    mode++;
  }
  attachInterrupt(digitalPinToInterrupt(interruptPin), ButtonPressed, LOW);
  back = true;
}
