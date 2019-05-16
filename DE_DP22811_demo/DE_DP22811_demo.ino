#define data 9      //Liña de datos para o visualizador DE-DP22811
#define clock 8     //liña de reloxo para o  DE-DP22811
#define dimm 10     //liña de atenuación (dimer) negada para o DE-DP22811


int segs[10] ={63, 6, 91, 79, 102, 109, 124, 07, 127, 103};  // codificación dos segmentos para os numeros 0 a 9

byte punto = B10000000;   // Codificación do segmento do punto 


void setup()
{
  pinMode(data , OUTPUT);      // definicion do modo da liña de datos
  pinMode(clock, OUTPUT);      // definicion do modo da liña de reloxo
  pinMode(dimm , OUTPUT);      // definicion do modo da liña de atenuación (negada)
}



void loop()
{

  for (int y = 254; y > 16; y-=16)  // bucle para incremantar o brillo do visualizador
  {
    analogWrite(dimm, y);          // PWM para atenuar o visualizador

    for (int x = 0; x <10; x++)  // bucle para enviar os dixitos o visualizador 1, 2,,,9
    {
      shiftOut(data, clock, MSBFIRST, segs[x]); // envia o dixito desprazando 8 bits enviando primeiro o bit máis significativo
      shiftOut(data, clock, MSBFIRST, segs[x]); // envia o segundo dixito desprazando 8 bits mais
      delay(200);
    }

    shiftOut(data, clock, MSBFIRST, punto); // acende o punto
    shiftOut(data, clock, MSBFIRST, punto);
    delay(200);
  }
}

