/* Xabier Rosas
 *  Ventilación conctado a saída rele
 *  sensor de temperatura NTC   
 *  Activamos o ventilador en funcion da temperatura actual e a elixida.   
 *  evitamos sobre-disparos usando un valor de histeresis. 
 *  O pulsar recollemos o valor de temperatura actual como valor consigna.
*/

#include <math.h>
#define sensorPin A6    // Entrada NTC
#define rele 13         // Saida para o Relé
#define Tecla 2         // Entrada do Pulsador

int Ventila = 25;      // Temperatura de ventilación
int histeresis = 0.5;  // valor de histeresis para evitar rearmes 


// sub rutina Steinhart-Hart para ler a NTC

double Thermistor(int RawADC) {
double Temp;
Temp = log(10000.0*((1024.0/RawADC-1)));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
Temp = Temp - 273.15; // Convert Kelvin to Celcius
//Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
return Temp;
}

void setup() {
Serial.begin(9600);
pinMode(rele,OUTPUT);
pinMode(Tecla,INPUT_PULLUP);
}
void loop() {

int readVal=analogRead(sensorPin);  //ler o sensor
double temp = Thermistor(readVal);  //pasamos o valor a subrutina e devolta o valor de temperatura

  if (temp >= Ventila + histeresis  ){ digitalWrite (rele, HIGH); }  //se superamos a temperatura de comparación acendemos a ventilación
  if (temp < Ventila - histeresis  ){ digitalWrite (rele, LOW); }    // en caso contrario apagamos a ventilación

  if (digitalRead(Tecla) == LOW){Ventila = temp;}  // si se pulsa a tecla pasamos a temperatura actual o valor de comparación
 
Serial.print("Temp= ");
Serial.print(temp);
Serial.print(" C");
if (temp >= Ventila ){ Serial.print("\t Ventilando por encima de: "); }
else { Serial.print("\t non ventilando por baixo de: "); }
Serial.print(Ventila);
Serial.println(" C +-0.5C");

delay(500);
}
