/* Xabier Rosas
 Fade PWM
  Este exemplo convirte a entrada analoxica "potpin" nun valor dixital 0 - 1023
  Escala este valor de 10 bits nun de 8 bits (0-255)
  Este ultimo valor o utilizaremos para regular a iluminacion do led conectado no pin "led"
  
 This example code is in the public domain.
 */

int led = 6;                          // Pin para conectar o led (con capacidade PWM)
int potpin = A2;                      // Pin de entrada analoxica
int valor;                            // declaración da variable 

                                      // rutina de axuste despois de RESET
void setup()  { 

  pinMode(led, OUTPUT);               // declaramos pin 9 como saida


  Serial.begin(9600);                 // iniciamos a as comunicacións serire a 9600 bps.
  
} 


void loop()  {                         

  valor = analogRead(potpin);          // recollemos o valor da entrada analoxica na variable valor 
  Serial.print("sensor = " );          // enviamos via serie o texto "sensor = "
  Serial.print(valor*0.005);           // enviamos via serie o o valor equivalente en vots ( escalon = 5V / 1024 = 0.0048828125)
  Serial.print("V" );                  // enviamos via serie o texto "V"

  valor = map(valor, 0, 1023, 0, 255); // Mapeamos un entero de 10bits "valor" a outro de 8 bits
  analogWrite(led, valor);             // Pasamos ese valor a saida "led" en PWM 
  Serial.print("\t saida LED = ");     // enviamos via serie un tabulador (\t) mais o texto " saida LED =  "
  Serial.println(valor);               // enviamos via serie o valor PWM
  delay(1);                            // pomos un ataraso para evitar interferencias no CAD    
}
