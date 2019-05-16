/*
Copyright (C) XDeSIG

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
USA.
See LICENSE.txt for details
*/

#include <Ultrasonic.h>
Ultrasonic ultrasonic(4, 5);
int distancia = 0;
int distancia_ = 0;
int Mdistancia = 0;
int o_chan = 0;
int led = 13;
int brillo = 250;
int brillo1 = 300 - brillo;
const int clock = 8;  // out to In clock PIN 10 (J1) DE-DP22811
const int data = 9;   // out to In data PIN 9 (J1) DE-DP22811
const int dimm = 10;  // out to In /dimm PIN 7 (J1) DE-DP22811
const int VIS1 = 11; // 1/2 dixito para representar nun visualizador de 7 segmentos ou 2 leds facendo o nº "1"


int segs[10] = {63, 6, 91, 79, 102, 109, 124, 07, 127, 103};  // // codificación dos segmentos para os n 0 a 9
int unidade = 0;
int decena = 0;
byte punto = B10000000; // Codificación do segmento do punto
byte nada = B00000000; // Codificación dixitos apagados


void setup()
{
  pinMode(VIS1, OUTPUT);  // VIS1
  pinMode(clock, OUTPUT);
  pinMode(data , OUTPUT);
  pinMode(dimm , OUTPUT);

  pinMode(led , OUTPUT);

  Serial.begin(9600); //Comentar para Leonardo
  Serial.println("Preparado");

  analogWrite(dimm, brillo);
  for (int x = 0; x < 10; x++)                         // bucle para enviar os dixitos o visualizador 1, 2,,,9
  {
    digitalWrite(dimm, HIGH);                       // apaga o display
    shiftOut(data, clock, MSBFIRST, segs[x]);       // envia o dixito desprazando 8 bits enviando primeiro o bit máis significativo
    shiftOut(data, clock, MSBFIRST, segs[x]);       // envia o segundo dixito desprazando 8 bits mais
    analogWrite(dimm, brillo);                      // acende o display
    analogWrite(VIS1, brillo1);
    delay(500);
  }
  delay(2000);
  digitalWrite(VIS1, LOW);

  axuste();



  if ( o_chan > 99) analogWrite(VIS1, brillo1);
  else digitalWrite(VIS1, LOW);

  distancia = o_chan;        //so para poder pintalo
  pinta();

  delay(1000);

  atenua();

}


void loop()
{
  distancia_ = distancia;

  if (distancia < 10 ) {
    digitalWrite(dimm, HIGH);
    pintapunto();
  }

  distancia = ultrasonic.Ranging(CM);

  //if (ultrasonic.Ranging(CM) > o_chan) axuste();

  distancia = o_chan - distancia ;

  Mdistancia = distancia;

  if (distancia < 100 ) digitalWrite(VIS1, LOW);
  if (distancia < 200) digitalWrite(led, LOW);

  delay(200);

  if (distancia < 0 || distancia > 499) loop;

  if (distancia != distancia_)   pinta() ;


}

void pinta()
{
  analogWrite(dimm, brillo);

  if (distancia > 99 && distancia < 199 )  {
    distancia -= 100;
    analogWrite(VIS1, brillo1);
  }
  else if (distancia > 199) {
    distancia -= 200;
    digitalWrite(led, HIGH);
  }

  decena = ("%f", distancia / 10);
  unidade = ("%d", distancia % 10);
  if (decena < 0 || decena > 9 ) return;
  if (unidade < 0 || unidade > 9 ) return;

  digitalWrite(dimm, HIGH);                      // apaga o display
  shiftOut(data, clock, MSBFIRST, segs[unidade]);
  shiftOut(data, clock, MSBFIRST, segs[decena]);
  analogWrite(dimm, brillo);                    // acende o display
  serie();
  delay (300);
}

void pintapunto()
{
  digitalWrite(VIS1, LOW);
  shiftOut(data, clock, MSBFIRST, punto);
  shiftOut(data, clock, MSBFIRST, nada);
  analogWrite(dimm, 250);
  delay(500);
}



void serie()
{
  Serial.print("Distancia ");
  Serial.print(Mdistancia);
  Serial.println(" cm");
  Serial.print("Distancia do sensor o Chan ");
  Serial.print(o_chan);
  Serial.println(" cm");
  Serial.print("Lectura do sensor ");
  Serial.print(ultrasonic.Ranging(CM));
  Serial.println(" cm");
}

void axuste()
{
  digitalWrite(led, HIGH);
  o_chan = ultrasonic.Ranging(CM);
  delay(100);
  if (o_chan > 400) axuste();
  Serial.print("Distancia o chan ");
  Serial.println(o_chan);
  digitalWrite(led, LOW);
}

void atenua()
{
  for (int y = 200; y < 256 ; y++)  // bucle para incrementar o brillo do visualizador
  {
    analogWrite(dimm, y);
    analogWrite(VIS1, 255 - y);
    delay(25);
  }

}
