/* Xabier Rosas
Ventilacion e medida de temperatura e humindade con DHT11

*/

#include <math.h>

#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); 

#include <dht.h>
dht DHT;
#define DHT11_PIN 10

int LedVent = 5;   // Saida para o Relé
int Tecla = 9;     // Entrada do Pulsador
int Ventila = 25;  // Tempreatura de ventilación
int histeresis = 0.5; // valor de histeresis para evitar rearmes rápidos

// rutina para visualizar os textos na OLED
void draw(void) {

  u8g.setFont(u8g_font_unifont);// tipo de fonte
  
  u8g.setPrintPos(5, 62);       // posicion do cursor
    u8g.print("RobotiCastelao");  // cadea a visualizar
  
  u8g.setPrintPos(0, 27);
    u8g.print("Temp ");

  u8g.setPrintPos(65, 27);
    u8g.print("T.Ven ");

  u8g.setPrintPos(0, 45);
    u8g.print(" Humidade ");
  u8g.setPrintPos(105, 45);
    u8g.print("%");
}

void setup() {
Serial.begin(9600);             // inicializando as comunicacións

pinMode(LedVent,OUTPUT);          // modo dos pines
pinMode(Tecla,INPUT_PULLUP);      // entrada de pulsador con resistencia interna a +vcc
}

void loop() {
DHT.read11(DHT11_PIN); // lectura  do DTH11
int Temperatura = DHT.temperature;
int Humidade = DHT.humidity;

  if (Temperatura >= Ventila + histeresis  ){ digitalWrite (LedVent, HIGH); }  //se superamos a temperatura de comparación acendemos a ventilación
  if (Temperatura < Ventila - histeresis  ){ digitalWrite (LedVent, LOW); }                      // en caso contrario apagamos a ventilación

  if (digitalRead(Tecla) == LOW){ Ventila = Temperatura;}  // si se pulsa a tecla pasamos a temperatura actual o vlor de comparación

  
u8g.firstPage();  
  do {
    draw();
    u8g.setPrintPos(35, 27);
    u8g.print(Temperatura);
    u8g.setPrintPos(110, 27);
    u8g.print(Ventila);
    u8g.setPrintPos(85, 45);
    u8g.print(Humidade);
    u8g.setPrintPos(10, 10);
    if (Temperatura >= Ventila ){ u8g.print("Ventilando"); }
  } while( u8g.nextPage() );


Serial.print("Temp= ");
Serial.print(DHT.temperature);
Serial.print(" C");
if (Temperatura >= Ventila ){ Serial.print("\t Ventilando por encima de: "); }
else { Serial.print("\t non ventilando por baixo de: "); }
Serial.print(Ventila);
Serial.println(" C +-0.5C");
Serial.print(DHT.humidity, 1);
Serial.print(",\t");
Serial.println(DHT.temperature, 1);

delay(100);
}
