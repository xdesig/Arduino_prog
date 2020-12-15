/*  Xabier Rosas
 * Medir a distancia mediante ultrasons ping
 * representando a medida na pantalla oled SSD1306_128X64
 * 
 */

#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); 

#define trigPin 16
#define echoPin 10
#define led 15


// rutina para visualizar os textos na OLED
void draw(void) {

  u8g.setFont(u8g_font_unifont);// tipo de fonte
  
  u8g.setPrintPos(5, 62);       // posicion do cursor
    u8g.print("RobotiCastelao");  // cadea a visualizar
  
  u8g.setPrintPos(0, 27);
    u8g.print("Distancia. ");

    u8g.setPrintPos(105, 45);
    u8g.print("cm");
  
  
}



void setup() {
  Serial.begin (9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);

}

void loop()
{
  double distancia;
  long duracion; 
  

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duracion = pulseIn(echoPin,HIGH);
  
// uSeg * 340m/s /2

    distancia = (duracion/2)/27.6;
  

 
  delay(200);
  

 u8g.firstPage();  
  do {
    draw();
    u8g.setPrintPos(50, 45);
    u8g.print(distancia);
     u8g.setPrintPos(10, 10);
    if (distancia >= 300 ){ u8g.print("> 300 cm"); }
  } while( u8g.nextPage() );

  
  digitalWrite(led, HIGH);
  Serial.print("Duracion ");
  Serial.print(duracion);
  Serial.print("\t ");
  Serial.print("Distancia ");
  Serial.print(distancia);
  Serial.println(" cm");
  digitalWrite(led, LOW);
}
