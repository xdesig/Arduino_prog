/*  Xabier Rosas
 *  Vumetro con unha barra de 8 leds (a barra ten 10 só usamos 8)
 */
int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9} ; // pines de saida nunha matriz
int Pin_analox = A0;

boolean LED_pico = false; 
int Valor_pico = 0;

void setup()                 
{
  for (int i = 0; i < 8; i++)
  {
     pinMode(ledPins[i], OUTPUT); 
  }
 Serial.begin(9600);   
}

void loop()                   
{
   Valor_pico = 0;
    
  int sinal = analogRead(Pin_analox);
  
  int LED_maior = map(sinal, 0, 1023, 0, 8); 
  
    for (int i = 0; i < LED_maior; i++) //Acender os leds ata o valor máis alto
  {
      digitalWrite(ledPins[i], HIGH);    
     
    }

      for (int i = LED_maior+1; i <8; i++) //apagar ata o LED máis alto
      {
      digitalWrite(ledPins[i], LOW); 
      
  
      
      }
      //Serial.print("sinal ");
      //Serial.print ("\t");
      Serial.println(sinal);


      /*Serial.print("Led_maior ");
      Serial.print ("\t");
      Serial.println( LED_maior);
  */
  delay(100);
}
