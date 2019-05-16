/*
 *
  * Modif:  Xabier Rosas (xrosas) roboticastelao
  * O programa vai descodificar os datos Wiegand dun RFID HID Reader http://bit.ly/29yz3lj
  * (ou, en teoría,   * Calquera outro dispositivo que xera Weigand de datos).
  * A interface Wiegand ten dúas liñas de datos, DATA0 e DATA1. Estas liñas son normalmente altas
  * Alta non 5V. Cando un 0 é enviado, DATA0 cae 5V. Cando un 1 é enviado, DATA1 cae 0V
  * Hai xeralmente uns poucos ms entre os pulsos.
  *
  * (DATA0) para Dixital Pin 2 (INT0).
  * (DATA1) para Dixital Pin 3 (INT1). Iso e todo!
  *
  * A operación é simple - cada unha das liñas de datos son conectadas ás liñas de interrupción de hardware. cando
  * baixa a cero, unha rutina de interrupción  algúns bits son invertidos. Tras algún tempo de
  * De non recibir bits, o Arduino pode decodificar os datos.
  * HID RFID Reader Wiegand Interface para Arduino Uno
  * Escrito por Daniel Smith, 2012/01/30
  * Www.pagemac.com
*/


//Includes

// Ethernet
#include <SPI.h>
#include <Ethernet.h>

  
//Wiegand Interface
#define MAX_BITS 100                 // maximo numero de bits
#define WEIGAND_WAIT_TIME  3000      // tempo para agardar por outro pulso Weigand.  

#define TARXETA_E 33611              // tarxeta para Engadir +
#define TARXETA_B 25111              // tarxeta para Borrar

//***
unsigned char databits[MAX_BITS];    // almacena todos os bits
unsigned char bitCount;              // numero de bits capturados
unsigned char flagDone;              // vai para abaixo cando os datos está a ser capturada
unsigned int weigand_counter;        // conta atrás ata que asumimos non hai máis bits

unsigned long facilityCode=0;        // facility (site)  code descodificado
unsigned long cardCode=0;            // card code descodificada
unsigned long cardOn=0;              // card code apertura
unsigned int evento=0;

unsigned int state;

const int ledv=9;                    // led Apertura Verde
const int leda=8;                    // led Espera Amarelo
const int buz=5;                     // Zumbador




// Ethernet 
  byte mac[] = { 
    0x00, 0x00, 0xBE, 0xEF, 0xFE, 0xED};

  IPAddress ip(10,0,5,125);
  IPAddress gateway(10,0,5,253);	
  IPAddress subnet(255, 255, 255, 0);
  EthernetServer server(80);

void setup()
{

  Serial.begin(9600);
  
  //iniciar a conexión Ethernet e o server:
  Ethernet.begin(mac, ip);
  server.begin();
 
  
  pinMode(ledv, OUTPUT);  // LED Apertura Verde
  pinMode(leda, OUTPUT);  // LED Espera e novo Amarelo

  pinMode(buz, OUTPUT);
  pinMode(2, INPUT);     // DATA0 (INT0)
  pinMode(3, INPUT);     // DATA1 (INT1)
  
  beep(200);
  digitalWrite(leda, HIGH);
  delay(400);
  digitalWrite(leda, LOW);
  beep(25);
  delay(100);
  beep(25);
  delay (250);
  digitalWrite(leda, LOW);
  digitalWrite(ledv, LOW);
  
  Serial.println("");
  Serial.print(" IP servidor:  "); Serial.print(Ethernet.localIP());Serial.println(" Actualizacion 5 seg"); //client.println("Refresh: 5; url = ./");  // refresca a paxina cada 5 seg.

  Serial.println(" RDIF Preparado");

 
  // une as funcions ISR (funcion de interrupción) ao flanco de baixada de INTO e INT1
  attachInterrupt(0, ISR_INT0, FALLING);  
  attachInterrupt(1, ISR_INT1, FALLING);
 

  weigand_counter = WEIGAND_WAIT_TIME;
}

// interrupción cando INTO pasa a nivel baixo (0 bit)
void ISR_INT0()
{
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
}

// interrupción cando INT1 pasa a nivel baixo (1 bit)
void ISR_INT1()
{
 //Serial.print("1");
  databits[bitCount] = 1;
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
}





void printBits()
{
      Serial.print("Cod ");
      Serial.print(facilityCode);
      Serial.print(", ");
      Serial.println(cardCode);
      verifica();      
}

void verifica()
{
    if (evento==1){ cardOn=cardCode; beep(16); delay (500); Serial.print(cardCode); Serial.println (" como Apertura"); evento=0; digitalWrite(leda, LOW);}
    else if (cardCode==TARXETA_E) engade();
    else if (cardCode==TARXETA_B) borra();
    else if (cardCode==cardOn) abre();
    else Serial.println (" Acceso denegado.");
}

void engade()
{
  beep (32);
  Serial.println (" Que tarxeta queres para Apertura? ");
  digitalWrite(leda, HIGH);
  delay (500);
    evento = 1;
}

void borra()
{
  digitalWrite(leda, HIGH);
  beep (400);
  beep (200);
  Serial.print (cardOn);
  Serial.println ("  BORRADA ");
  digitalWrite(leda, LOW);
  cardOn=0;
  evento=2;
 
}

void abre()
{
     Serial.println(" Abrindo.");
     evento=3;
     digitalWrite(ledv, HIGH);
     beep(100);
     delay(2000);             
     digitalWrite(ledv, LOW);
     beep(20);
     Serial.println(" Feito.");
     
}

void beep(unsigned char delayms)
{
  analogWrite(buz, 128);      
  delay(delayms);          // wait for a delayms ms
  analogWrite(buz, 32);      
  delay(delayms/2);          // 
  analogWrite(buz, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  


void loop()

{
  // Esta espera para asegurarse de que non hai máis pulsos de datos antes de procesamento de datos
  if (!flagDone) {
    if (--weigand_counter == 0)
      flagDone = 1;    
                 }
 
  // si temos xa os bits e o contador de weigand xa reamatou
  if (bitCount > 0 && flagDone) {
    unsigned char i;
    
    Serial.print("Formato ");
    Serial.print(bitCount,DEC);
    Serial.print(" bits. ");
    
    // vamos a decodificar os bits de maneira diferente dependendo da cantidade de bits que tenemos
    // www.pagemac.com/azure/data_formats.php para mais información
    if (bitCount == 35)
    {
      // Formato 35 bit HID Corporate 1000
      // codigo facility  = bits 2 ao 14
      for (i=2; i<14; i++)
      {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
      
      // codigo tarxeta = bits 15 ao 34
      for (i=14; i<34; i++)
      {
         cardCode <<=1;
         cardCode |= databits[i];
      }
      
      printBits();
    }
    else if (bitCount == 26)
    {
      // Formato standard 26 bit
      // facility code = bits 2 ao 9
      for (i=1; i<9; i++)
      {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
      
      // codigo tarxeta = bits 10 ao 23
      for (i=9; i<25; i++)
      {
         cardCode <<=1;
         cardCode |= databits[i];
      }
      
      printBits();  
    }
    else {
         Serial.println("Non o podo descodificar .");
        digitalWrite(leda, HIGH);
        beep (300);
        digitalWrite(leda, LOW);
        beep (300);
        digitalWrite(leda, HIGH);
        beep (300);
        digitalWrite(leda, LOW);
        beep (300);
    }

     // por a cero os datos para a seguinte tarxeta
     bitCount = 0;
     facilityCode = 0;
     cardCode = 0;
     for (i=0; i<MAX_BITS; i++)
     {
       databits[i] = 0;
     }
  }

 // Ethernet esperando Cliente e pax web
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("Cliente conectado");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      {
    boolean lineaenbranco=true;
    while(client.connected())//Cliente conectado
    {
      if(client.available())
      {
        char c=client.read();
        if(c=='\n' && lineaenbranco)//Si a petición HTTP finalizou
        {

          
          //Cabeceira hhtp
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
	  client.println("Refresh: 2; url = ./");  // refresca a paxina cada  seg
          client.println();

          //Página Web en HTML
          client.println("<!DOCTYPE HTML>");             
          client.println("<html>");
          client.println("<head>");
          client.println("<title>RFid </title>");
          client.println("</head>");
          client.println("<body width=100% height=100%>");
          client.println("<body bgcolor='#000033' text='#FF9900'>");
          client.println("<center>");
          client.print("<br><br>");
          client.print("Tarxeta de apertura: ");
          client.println(cardOn);
          client.print("<br><br>");  
          if (evento==1){client.println("Actualizando tarxeta de apertura: ");}
          else if (evento==2)client.println("Tarxeta borrada ");
          else if (evento==3)client.println("Abrindo "); delay(1000); evento=0;  
                 
          client.println("</center>");
          client.println("</body>");
          client.println("</html>");
          client.stop();//Pechamos a conexión co cliente
       }
      }
     }
    }
   }
  }
}
