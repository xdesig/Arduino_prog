#include <ESP8266WiFi.h>
#include <Wire.h>  
#include "SSD1306Wire.h" //

#include "Adafruit_CCS811.h" 
Adafruit_CCS811 ccs;
//**********************************
const char *ssid = "............";  // Nome da conexión WiFi
const char *password = "....";      // Contrasinal da Conexión

const char* server = "api.thingspeak.com"; // enderezo de thingspeak.com
String apiKey = "................";        //Chave da Api de thingspeak.com

//Tempo de captura de temperatura para a grafica (segundos)
// 1 pixel cada Periodo(segundos)
float const Periodo = 112.5;      //112,5seg = 4 Horas por pantalla 


// ***** Valores de alarma *****
const int Co2Max = 3000;
const int TvocMax = 500;
const int TempMax = 35;
const int HumiMax = 85;


//****** Variables ********************
int Co2Ms[128];   //Matriz para gardar os valores de Co2
int TvocMs[128];  //Matriz para gardar os valores de Tvoc
int TempMs[128];  //Matriz para gardar os valores de temperatura
int HumiMs[128];  //Matriz para gardar os valores de Tvoc
int Co2;        // Valor actual de Co2
int Co2M;         // Valor mapeado a Pantalla
int Tvoc;         // Valor actual de Tvco
int TvocM;        // Valor m...
int Tempe;         // Valor actual de Temperatura
int TempM;        // Valor m...
int Humi;         //  Valor actualHumidade
int HumiM;        // Humidade mapeada

unsigned long TempoActC;  // Tempo para recoller os datos
unsigned long TempoAct;   // Tempo para pasar os datos as graficas
unsigned long TempoEnvi;  // tempo

float Tempo;

int menu = 0;

int i;
int x = 0;
int contconex = 0;

#define Key D0  // Tecla para cambiar a visualización
#define LED D4  // LED no NODEMCU 1.0



// Inicializa a pantalla
SSD1306Wire  display(0x3c, D2, D1);  //D2=SDK  D1=SCK  

void setup() {
  pinMode (Key, INPUT);
  pinMode (LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("XDeSIG");
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 16, "XDeSIG");
  display.display();
  
  //***** Conexión WIFI *****
  int cont = 10;        
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 40, "intentando conectar");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconex < 50) { //Tenta 50 veces conectar a WiFi
    ++contconex;
    cont = cont + 2;
    delay(500);
    Serial.print(".");    // Pinta un punto por conexión
    display.setPixel(cont, 62); // Pinta un punto por conexión
    display.display();
  }
  if (contconex < 50) {
    //para usar con ip fixa
    IPAddress ip(10, 0, 4, 150);
    IPAddress gateway(10, 0, 0, 254);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 40, "WiFi conectado");
    display.display();
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 40, " Error de conexion ");
    display.display();
  }
  delay(2000);
  
  //********* Comproba a conexión co sensor de CO2 ***************
  if (!ccs.begin()) {
    for (i = 0; i < 1000; i++) {
      Serial.println("Non se iniciou o sensor, comproba as conexións!");

      ErrCO2();           // Chama a funcion para indicar erro no sensor
      //AlCO2();          // Chama a funcion de Alarma
      display.display();
    }
    while (1);
  }
  Serial.println("Sensor CO2 iniciado");
  
  //***** Inicia as matrices con datos fora de pantalla *****
  for (int i = 127; i > 0; i--) {
    Co2Ms[i] = 64;
    TvocMs[i] = 64;
    TempMs[i] = 64;
    HumiMs[i] = 64;
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(64, 40, String(i));
    display.display();
    delay(200);
  }

}

void loop() {

  //***** Menú *****
  if (digitalRead (Key) == 0 ) {
    menu = menu + 1;
    delay(300);
    //beep();
    if (menu > 4) {
      menu = 0;
    }
  }
  switch (menu) {
    case 0: PDatos();
      break;
    case 1: GCo2(); //Grafica Co2
      break;
    case 2: GTvo(); //Grafica Tvoc
      break;
    case 3: GTem(); //Grafica Temperatura
      break;
    case 4: GHum(); //Grafica Temperatura
      break;
  }


  //**** Recolle os datos Co2, Tvoc, temp, e %Humi *****

  if (millis() - TempoActC >= 2000) {

    if (!ccs.readData()) {
      Co2 = ccs.geteCO2();
      Tvoc = ccs.getTVOC();
    }
    else {
      ErrCO2();
      //AlCO2();
    }
    TempoActC = millis();
  }

  //***** Pasa todolos datos  mapeados as suas matrices cada periodo *****
  if (millis() - TempoAct >= (Periodo * 1000)) {
    Tempo = millis() - TempoAct;
    Co2M = map(Co2, 400, 6000, 60, 10 );
    Co2Ms[x] = Co2M;

    TvocM = map(Tvoc, 0, 1100, 60, 10 );
    TvocMs[x] = TvocM;

    TempM = map(Tempe, 0, 50, 55, 10 );
    TempMs[x] = TempM;

    HumiM = map(Humi, 0, 99, 55, 10 );
    HumiMs[x] = HumiM;

    if (x < 127) {
      x = x + 1;
    }
    else {
      for (i = 0; i < 127; i++) {
        Co2Ms[i] = Co2Ms[i + 1];
        TvocMs[i] = TvocMs[i + 1];
        TempMs[i] = TempMs[i + 1];
        HumiMs[i] = HumiMs[i + 1];
      }
    }
    //debug();

    TempoAct = millis();
  }

  //**** Envia datos api.athingspeak.com cada 5 segundos *****
  if (millis() - TempoEnvi >=  5000) {
    TempoEnvi = millis();
    if (Co2 < 10000) {
      enviarDatos();
    }
  }



  //***** Borra o contido anterior e mostra os novos valores *****
  display.display();
  delay(1);
  display.clear();
}

//***** Presenta datos en pantalla*****
void PDatos() {

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "CO2 ppm");
  display.drawString(30, 45, "%h");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 0, "Tvoc");
  display.drawString(95, 45, "ºC");

  //*****
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 11, String(Co2));
  display.drawString(0, 39, String(Humi));
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(127, 11, String(Tvoc));
  display.drawString(127, 39, String(Tempe));

}


//******* Debuxa a grafica de Co2.  menu 1 *******
void GCo2() {
  // Pasa os os 128 valores almacenados a memoria do OLED
  for (i = 0; i < 127; i++) {
    display.setPixel( i, Co2Ms[i]);
  }
  Graf();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 0, "CO2");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(127, 0, String(Co2));
}

//******* Debuxa a grafica de Tvco.  menu 2 *******
void GTvo() {
  for (i = 0; i < 127; i++) {
    display.setPixel( i, TvocMs[i]);
  }
  Graf();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 0, "Tvoc");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(127, 0, String(Tvoc));

}

//******* Debuxa a grafica de temperatura.  menu 3 *******
void GTem() {
  // Pasa os os 128 valores almacenados a memoria do OLED
  for (i = 0; i < 127; i++) {
    display.setPixel( i, TempMs[i]);
  }
  Graf();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 0, "Temperatura");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(127, 0, String(Tempe));
}

//******* Debuxa a grafica de humidade.  menu 4 *******
void GHum() {
  // Pasa os os 128 valores almacenados a memoria do OLED
  for (i = 0; i < 127; i++) {
    display.setPixel( i, HumiMs[i]);
  }
  Graf();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 0, "Humidade");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(127, 0, String(Humi));
}



//***** Xestion de erro nos medidores
void ErrCO2() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 0, "ERRO - Sensor");
  display.drawString(63, 17, "Comproba as");
  display.drawString(63, 32, "conexións");
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 52, "V, G, SCL, SDA, W->G");
}

//***** debuxa puntos na pantalla *****
void Graf() {
  for (i = 0; i < 127; i = i + 16) {
    display.setPixel( i, 63);
  }
}

//*****
void debug() {
  Serial.print ("menu ");
  Serial.print (menu);
  Serial.print ("\t");
  Serial.print ("Tempo ");
  Serial.print (Tempo);
  Serial.print ("\t");
  Serial.print ("Co2 ");
  Serial.print (Co2);
  Serial.print ("\t");
  Serial.print ("Tvoc ");
  Serial.print (Tvoc);
  Serial.print ("Tempe ");
  Serial.print (Tempe);
  Serial.print ("Humi ");
  Serial.println (Humi);


}

//***** Función para enviar os datos a thingspeak.com *****

void enviarDatos()
{
  digitalWrite(LED, 0);
  WiFiClient client;

  if (client.connect(server, 80)) {
    Serial.println("Cliente connectado ");
    debug();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 12, "Envia");
    display.display();

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(Co2);
    postStr += "&field2=";
    postStr += String(Tvoc);
    postStr += "&field3=";
    postStr += String(Tempe);
    postStr += "&field4=";
    postStr += String(Humi);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    //delay(1000);
  }
  client.stop();
  digitalWrite(LED, 1);
}



// https://github.com/ThingPulse/esp8266-oled-ssd1306
 
