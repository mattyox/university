#include <IRremote.h>
#define irPin 3
IRrecv irrecv(irPin);
decode_results results;
float lightValue = 255;
int redLed = 5;
int greenLed = 9;
int yellowLed = 11;
int state = 0;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
}
 
void loop() {
  if (irrecv.decode(&results)){
      switch (results.value) {
         case 0xE0E040BF:
            digitalWrite(redLed, LOW);
            digitalWrite(greenLed, LOW);
            digitalWrite(yellowLed, LOW);
            state = 0;
            break;
         case 0xE0E036C9:
            digitalWrite(greenLed, LOW);
            digitalWrite(yellowLed, LOW);
            lightValue = 255;
            analogWrite(redLed, lightValue);
            state = 1;
            break;
         case 0xE0E028D7:
            digitalWrite(redLed, LOW);
            digitalWrite(yellowLed, LOW);
            lightValue = 255;
            analogWrite(greenLed, lightValue);
            state = 2;
            break;
         case 0xE0E0A857:
            digitalWrite(redLed, LOW);
            digitalWrite(greenLed, LOW);
            lightValue = 255;
            analogWrite(yellowLed, lightValue);
            state = 3;
            break;
        case 0xE0E0E01F:
            lightValue = lightValue + 25.5;
            if (lightValue > 255) {
              lightValue = 255;
            }
            break;
         case 0xE0E0D02F:
            lightValue = lightValue - 25.5;
            if (lightValue < 0) {
              lightValue = 0;
            }
            break;
  }
  irrecv.resume();
  }
  switch (state) {
    case 1:
    analogWrite(redLed, lightValue);
    break;
    case 2:
    analogWrite(greenLed, lightValue);
    break;
    case 3:
    analogWrite(yellowLed, lightValue);
    break;
    }
}
