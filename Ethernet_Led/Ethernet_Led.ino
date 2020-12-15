/*
Led Ethernet 

baseado en :
http://geekytheory.com/arduino-ethernet-shield-relay/

Modificado por xrosas
*/

#include <SPI.h>
#include <Ethernet.h>


byte mac[] = {0x00, 0x22, 0xF7, 0x19, 0x4B, 0x28 };
IPAddress ip(10,0,4,125);
EthernetServer server(80);


int PIN_LED=8;
String readString=String(30);
String state=String(3);

void setup() {

  Serial.begin(9600);

  }


  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println("  RobotiCastelao");
  Serial.print("IP servidor:  ");
  Serial.println(Ethernet.localIP());
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,HIGH);
  delay(250);               
  digitalWrite(PIN_LED, LOW);
  delay(250);               
  digitalWrite(PIN_LED,HIGH);
  delay(250);               
  digitalWrite(PIN_LED, LOW);
  state="OFF";
  Serial.println("Actualizacion 5 seg");
}

void loop() {
  // escoitamos o cliente
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      {
    boolean lineaenbranco=true;
    while(client.connected()) //Cliente conectado
    {
      if(client.available())
      {
        char c=client.read();
        if(readString.length()<30)//Leemos a petición HTTP caracter a caracter
        {
          readString.concat(c); //Almacenamos os caracteis na variable readString
        }
        if(c=='\n' && lineaenbranco)//Si a petición HTTP finalizou
        {
          int LED = readString.indexOf("LED=");
          if(readString.substring(LED,LED+5)=="LED=T")
          {
            digitalWrite(PIN_LED,HIGH);
            state="ON";
          } else if (readString.substring(LED,LED+5)=="LED=F")
          {
            digitalWrite(PIN_LED,LOW);
            state="OFF";
          }
            Serial.print("Estado = ");
            Serial.println(state);
          
          //Cabeceira hhtp
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
	  client.println("Refresh: 5");  // refresca a paxina cada 5 seg
          client.println();

          //Página Web en HTML
          client.println("<!DOCTYPE HTML>");             
          client.println("<html>");
          client.println("<head>");
          client.println("<title>WEB Led server </title>");
          client.println("</head>");
          client.println("<body width=100% height=100%>");
          client.println("<body bgcolor='#000033' text='#FF9900'>");
          client.println("<center>");
          client.println("<h1>LED ON/OFF</h1>");
          client.print("<br><br>");
          client.print("Estado do LED: ");
          client.print(state);
          client.print("<br><br><br><br>");
          client.println("<input type=submit value=ON style=width:200px;height:75px onClick=location.href='./?LED=T\'>");
          client.println("<input type=submit value=OFF style=width:200px;height:75px onClick=location.href='./?LED=F\'>");
          client.println("</center>");
          client.println("</body>");
          client.println("</html>");
          client.stop();//Pechamos a conexión co cliente
          readString="";
        }
      }
    }
  }
 }
  }
}
