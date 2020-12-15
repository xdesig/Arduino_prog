//Coche fasnatico

int pinArray[] = {2, 3, 4, 5, 6, 7, 8, 9} ; // pines de saida nunha matriz

// PINes
int conta = 0; // Contador

int espera = 30; // Temporizador

void setup(){
for (conta=0;conta<8;conta++) { // Configuramos todos los PINs de golpe

pinMode(pinArray[conta], OUTPUT);
                              }
            }


void loop() {
for (conta=0;conta<8;conta++) { // Ancende os LEDs creando unha estela visual

digitalWrite(pinArray[conta], HIGH); // Recorremos a matriz en sentido ascendente
delay(espera);
digitalWrite(pinArray[conta], LOW);
delay(espera);
                            }

for (conta=7;conta>0;conta--) { // recorremos a matriz en sentido descendente pero comenzamos no 7 porque o 8 xa está acceso
digitalWrite(pinArray[conta], HIGH);
delay(espera);
digitalWrite(pinArray[conta], LOW);
delay(espera);
                             }
}
