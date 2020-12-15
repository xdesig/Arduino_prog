/*  Xabier Rosas
 * Pequeno robot con duas rodas conectadas a servos de xiro continuo 
 * Esquiva obxectos medianta a detecion por ultrasons.
 */

#include <Servo.h>

Servo myservoR;  // crear o obxeto servoR (dereito)
Servo myservoL;  // crear o obxeto servoL (esquerdo)

#define trigPin 8
#define echoPin 9

#define SR 7
#define SL 6
int distancia;      // Variable para a distancia
double duracion;    // Variable Da duración do eco.

void setup() {
  Serial.begin (9600);  
  
       
//Declaramos os pines do UltraSon
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  delay(2000);

  myservoR.attach(SR);  // conectamos o servo dereito
  myservoL.attach(SL);  // conectamos o servo esquerdo
}
void loop() {
// Medir distancia
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    duracion = pulseIn(echoPin,HIGH);
  
    
    distancia = (duracion/2)/27.6;

//**************************************

          if (distancia >=6 )  
              {
              adiante();
              delay(250);
              }  

          else
              {
              apaga_servo();
              delay(50);
              atras();
              delay(500);
              esquerda();
              //dereita
              delay(600);
              }  
                    
 }
//**************************************

void adiante() {
  myservoR.attach(SR);  // conectamos o servo dereito
  myservoL.attach(SL);  // conectamos o servo esquerdo
  myservoR.write(0); 
  myservoL.write(179); 
   
}

void atras() {
  myservoR.attach(SR);  // conectamos o servo dereito
  myservoL.attach(SL);  // conectamos o servo esquerdo
  myservoR.write(179); 
  myservoL.write(0); 
  
}

void dereita() {
  myservoR.attach(SR);  // conectamos o servo dereito
  myservoL.attach(SL);  // conectamos o servo esquerdo
  myservoR.write(0); 
  myservoL.write(0); 
  
}

void esquerda() {
  myservoR.attach(SR);  // conectamos o servo dereito
  myservoL.attach(SL);  // conectamos o servo esquerdo
  myservoR.write(179); 
  myservoL.write(179); 
  }

void apaga_servo(){
  myservoR.detach(); 
  myservoL.detach(); 
  delay (500);  
  }     

void serie(){
// comunicacion serie  
  Serial.print("Duracion ");
  Serial.print(duracion);
  Serial.print("\t ");
  Serial.print("Distancia ");
  Serial.print(distancia);
  Serial.println(" cm");    
}
