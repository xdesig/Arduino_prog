/* Xabier Rosas
 *  Ventilación conctado a saída rele
 *  sensor de temperatura NTC   
 *  Activamos o ventilador en funcion da temperatura actual e a elixida.   
 *  evitamos sobre-disparos usando un valor de histeresis. 
 *  O pulsar recollemos o valor de temperatura actual como valor consigna.
 *  Visualización en pantalla OLED SSD1306_128X64
*/

#include <math.h>
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); 

int sensorPin = A3; // Entrada NTC
int Led = 5;        // Saida para o Relé
int Tecla = 9;     // Entrada do Pulsador
int Ventila = 25;   // Tempreatura de ventilación
int histeresis = 0.5; // valor de histeresis para evitar rearmes 
double temp = 0;

// sub rutina Steinhart-Hart para ler a NTC

double Thermistor(int RawADC) {
double temp;
temp = log(10000.0*((1024.0/RawADC-1)));
temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
temp = temp - 273.15; // Convert Kelvin to Celcius
//Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
return temp;
}

void draw(void) {

  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(5, 62);
  u8g.print("Control ventila");
  
  u8g.setPrintPos(0, 27);
  u8g.print("Temp.Act. ");

  u8g.setPrintPos(0, 45);
  u8g.print("Temp.Vent ");
  
}

void setup() {
Serial.begin(9600);
pinMode(Led,OUTPUT);
pinMode(Tecla,INPUT_PULLUP);
}
void loop() {

int readVal=analogRead(sensorPin);  //ler o sensor
double temp = Thermistor(readVal);  //pasamos o valor a subrutina e devolta o valor de temperatura

  if (temp >= Ventila + histeresis  ){ digitalWrite (Led, HIGH); }  //se superamos a temperatura de comparación acendemos a ventilación
  if (temp < Ventila - histeresis  ){ digitalWrite (Led, LOW); }    // en caso contrario apagamos a ventilación

  if (digitalRead(Tecla) == LOW){ Ventila = temp;}  // si se pulsa a tecla pasamos a temperatura actual o vlor de comparación

  
u8g.firstPage();  
  do {
    draw();
    u8g.setPrintPos(85, 27);
    u8g.print(temp);
    u8g.setPrintPos(85, 45);
    u8g.print(Ventila);
    u8g.setPrintPos(10, 10);
    if (temp >= Ventila ){ u8g.print("Ventilando"); }
  } while( u8g.nextPage() );


Serial.print("Temp= ");
Serial.print(temp);
Serial.print(" C");
if (temp >= Ventila ){ Serial.print("\t Ventilando por encima de: "); }
else { Serial.print("\t non ventilando por baixo de: "); }
Serial.print(Ventila);
Serial.println(" C +-0.5C");



delay(500);
}
