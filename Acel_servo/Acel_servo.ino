/*Xabier Rosas
 * controlar un servo 0 a 179º mediante acelerometro analoxico
 * 
 * 
 */


#include <Servo.h> // Incluie a libreria para controlar o servo. https://www.arduino.cc/en/Reference/Servo

Servo servo1;      // crea o obxecto para o control do servo

#define Serv 4    // saída onde conectamos o servo.
#define Acel_x A2  // Entrada conectada o acelerómetro (X)

int i = 0;        // Declara e inicia a variable i, para o contador

int fil = 100;     //declara e inicia a variable para o filtro por media

int AX = 0;       // Variable para a entrada analóxica conectada o acelerómetro X

int Grad = 0;     // Variable para a saída conectada o servo.

void setup() {
  Serial.begin(9600);     // Inicia a comunicación serie a 9600 bits por segundo
  servo1.attach(4);       // Activa o servo e o fixa a saída 4
   
  pinMode (Acel_x, INPUT); // Pin Acel_x como entrada

}

void loop() {
  long MAX =0;
  for (i=0; i < fil; i++) {
    AX = analogRead(Acel_x);
    MAX = MAX+AX;
    }
  MAX = MAX/fil;
  Serial.print ("Acleromero");
    Serial.print("\t");
    Serial.print (MAX);
    Grad = map (MAX,180,512,0,127);
  Serial.print("\t\t");
  Serial.print ("Grados");
  Serial.print("\t");
  Serial.println (Grad);
  servo1.write(Grad);
  delay (100);
    

}
