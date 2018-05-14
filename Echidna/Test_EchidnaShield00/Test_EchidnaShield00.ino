//  *************************
//    FILE: Echidna_Test
//    X Rosas.
//  Test sensores / actuadores placa EchidnaShield 000
//  *************************

//*** Outputs ***
#define L_Red 13
#define L_Orange 12
#define L_Green 11

#define RGB_R 9
#define RGB_G 5
#define RGB_B 6

#define IO1 4
#define IO2 7
#define IO3 8

#define Buzzer 10

//*** Inputs ***
#define J_X A0
#define J_Y A1

#define Ace_X A2
#define Ace_Y A3

#define IN A4

#define LDR A5

#define SR 2
#define SL 3


int grados_1;
int grados_2;
int grados_3;
int Val_J_X;
int Val_J_Y;
int Val_Ace_X;
int Val_Ace_Y;

void setup() {

Serial.begin(9600);
  Serial.println(" TEST PROGRAM EchidnaShield, please sw Selecte in In sens mode! ");
  Serial.println ();

  
pinMode (L_Red, OUTPUT);
pinMode (L_Orange, OUTPUT);
pinMode (L_Green, OUTPUT);
pinMode (RGB_R, OUTPUT);
pinMode (RGB_G, OUTPUT);
pinMode (RGB_B, OUTPUT);
pinMode (IO1, OUTPUT);
pinMode (IO2, OUTPUT);
pinMode (IO3, OUTPUT);
pinMode (Buzzer, OUTPUT);


pinMode (J_X, INPUT);
pinMode (J_Y, INPUT);
pinMode (Ace_X, INPUT);
pinMode (Ace_Y, INPUT);
pinMode (IN, INPUT);
pinMode (LDR, INPUT);
pinMode (SR, INPUT);
pinMode (SL, INPUT);

}

void loop() {
Val_J_X = analogRead(J_X);
delay (10);
Val_J_Y = analogRead(J_Y);
delay (10);
Val_Ace_X = analogRead(Ace_X);
delay (10);
Val_Ace_Y = analogRead(Ace_Y);
delay (10);  

digitalWrite(RGB_B, HIGH);
delay (250);
digitalWrite(RGB_B, LOW);
digitalWrite(RGB_G, HIGH);
delay (250);
digitalWrite(RGB_G, LOW);
digitalWrite(RGB_R, HIGH);
delay (250);
digitalWrite(RGB_R, LOW);
delay(250);
digitalWrite(L_Red, HIGH); 
delay (250);
digitalWrite(L_Red, LOW); 
digitalWrite(L_Orange, HIGH);
delay (250);
digitalWrite(L_Orange, LOW);
digitalWrite(L_Green, HIGH);
delay (250);
digitalWrite(L_Green, LOW);
Serial.println("\t J_X A0 \t J_Y A1 \t Ace_X A2 \t Ace_Y A3 \t IN A4 \t I01 D4 \t I02 D7 \t I03 D8 \t LDR A5 \t SR D2 \t \t SL D3 ");
Serial.print("\t ");
Serial.print(Val_J_X);
  Serial.print("\t ");
  Serial.print("\t ");
    Serial.print(Val_J_Y);
      Serial.print("\t ");
      Serial.print("\t ");
        Serial.print(Val_Ace_X);
          Serial.print("\t ");
          Serial.print("\t ");
            Serial.print(Val_Ace_Y);delay (10);
              Serial.print("\t ");
              Serial.print("\t ");
                Serial.print(analogRead(IN));delay (10);
                  Serial.print("\t ");
                   Serial.print(digitalRead(IO1));
                    Serial.print("\t ");
                    Serial.print("\t ");

                     Serial.print(digitalRead(IO2));
                      Serial.print("\t ");
                      Serial.print("\t ");
                       Serial.print(digitalRead(IO3));delay (10);
                        Serial.print("\t ");
                        Serial.print("\t ");
                         Serial.print(analogRead(LDR));delay (10);
                          Serial.print("\t ");
                          Serial.print("\t ");
                           Serial.print(digitalRead(SR));
                            Serial.print("\t ");
                            Serial.print("\t ");
                            Serial.print(digitalRead(SL));
                              Serial.print("\t ");
                              Serial.println();

tone(Buzzer, 3500);
delay(250); 
noTone(Buzzer);

}
