                                   /*
Led Ethernet con pulsador

por xrosas
*/

#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0x00, 0x22, 0xF7, 0x19, 0x4B, 0x28 };

IPAddress ip(10,0,5,125);

EthernetServer server(80);


int LED1=3;  //22;
int PUL1=12;  //23;


String readString=String(30);
int state;
int buttonState;             // a lectura actual do Pin de entrada
int lastButtonState = LOW;   // a lectura anterior do Pin de entrada
long lastDebounceTime = 0;  // a última vez que o Pino de saída foi alternado
long debounceDelay = 50;    // tempo para a supresion de rebotes

void setup() {
  pinMode(LED1, OUTPUT);      
  pinMode(PUL1, INPUT_PULLUP);    
 
 

  Serial.begin(115200);


  

// ini Ethernet e the server:
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.println("");
  Serial.print("IP servidor:  ");
  Serial.println(Ethernet.localIP());
  Serial.println("Actualizacion 5 seg");
  state=1;
}

void loop() {
  int LED = readString.indexOf("LED=");
  int reading = digitalRead(PUL1);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();     // resetea o temporizador anti rebotes
  } 

  if ((millis() - lastDebounceTime) > debounceDelay) {
  
    if (reading != buttonState) {     // si o estado do pul cambiou, lese o novo
      buttonState = reading;

                                  // so cambia o estado do led cando o pulsador pasa a "1"
      if (buttonState == HIGH) {
         state = !state;
          if (state==0){
             digitalWrite(LED1, LOW);
             readString.substring(LED,LED+5)="LED=F";
           }
              else {
             digitalWrite(LED1, HIGH);
             readString.substring(LED,LED+5)="LED=T";
            }
            Serial.print("Estado = ");
            Serial.println(state);

      }
    }
  }

lastButtonState = reading;
  
  // escoitando por peticións entrantes
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    // unha solicitude HTTP remata cunha liña en branco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      {
    boolean lineaenbranco=true;
    while(client.connected())//Cliente conectado
    {
      if(client.available())
      {
        char c=client.read();
        if(readString.length()<30)//Leemos a petición HTTP caracter a caracter
        {
          readString.concat(c); //Almacenamos os caracteis na variable readString
        }
        if(c=='\n' && lineaenbranco) //Si a petición HTTP finalizou
        {
          int LED = readString.indexOf("LED=");
          if(readString.substring(LED,LED+5)=="LED=T")
          {
            state=1;
          } else if (readString.substring(LED,LED+5)=="LED=F")
          {
            state=0;
          }
           if (state==0){
             digitalWrite(LED1, LOW);
           }
              else {
             digitalWrite(LED1, HIGH);
                  }
            Serial.print("Estado (internet) = ");
            Serial.println(state);
          
          //Cabeceira hhtp
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
	  client.println("Refresh: 5; url = ./");  // refresca a paxina cada 5 seg.

          client.println();

          //Página Web en HTML
          client.println("<!DOCTYPE HTML>");             
          client.println("<html>");
          client.println("<head>");
          client.println("<title>WEB Led server/ pulsador </title>");
          client.println("</head>");
          client.println("<body width=100% height=100%>");
          client.println("<body bgcolor='#000033' text='#FF9900'>");
          client.println("<center>");
          client.println("<h1>LED ON/OFF</h1>");
          client.print("<br><br>");
          client.print("Estado do LED: ");
          client.print(state);
          client.print("<br><br>");

          client.println("<input type=submit value=ON style=width:75px;height:75px onClick=location.href='./?LED=T\'>");
          client.println("<input type=submit value=OFF style=width:75px;height:75px onClick=location.href='./?LED=F\'>");

           client.println ("");
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
