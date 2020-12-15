/* Xabier Rosas
 *  Demo de PWM mediante teclado analóxico como o usado no Escornabot
 *  
 */

#define Teclas  A3



#define Led  5
#define Led2 7

int Brillo = 0;
int Tolerancia = 5;

void setup() {
    
    Serial.begin(9600);
    
    pinMode(Teclas,INPUT_PULLUP);
    }

void loop() {
 int Valor = analogRead(Teclas);  
 
 if ( Valor <= (28 + Tolerancia ) &&   Valor >= (28 - Tolerancia )) { Brillo = Brillo + 15; if (Brillo > 255) { Brillo = 255; digitalWrite (Led2, LOW); }}
 
 if ( Valor <= (40 + Tolerancia ) &&  Valor >= (40 - Tolerancia )) { Brillo = Brillo - 15; if (Brillo < 0) { Brillo = 0; digitalWrite (Led2, LOW); }}

  analogWrite(Led, Brillo);

  Serial.print("Valor teclado ");
  Serial.print(Valor);
  Serial.print("\t Brillo ");
  Serial.println(Brillo);
  
  digitalWrite (Led2, HIGH);
  delay(100);        
}
