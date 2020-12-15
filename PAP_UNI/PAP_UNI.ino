/* Xabier Rosas
 * Control de un motor paso a paso 28 byj-48.
 *
 * Este motor unipolar está conectado mediante un xogo (4)de Pardarlington ULN
 * 2003
 *
 * Este programa vai excitando as fases do motor, primero unha, logo esa
 * misma e a seguinte, después a siguinte.
 *
 * Con este parámetro, consiguese un motor equilibrado, que non consuma
 * excesiva potencia e sen perder demasiado par.
 * 
 * Manexamos o motor con un potenciometro ou Joystick conectado a Iana
 * 
 */
 
 
 
#define M1  8     //Definimos as saidas que compoñen as bobinas.
#define M2  9
#define M3  10
#define M4  11
 
#define Iana 0   //Definimos a entrada analoxica para comparar
 
 
//Esta é a variable paso. Como vemos, é unha matriz que inclue cada unha das
//catro fases durante 8 ciclos, nos que imos alternando a activación de un o
//duas fases.
 
int Paso [ 8 ][ 4 ] =
   {  {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 1},
      {0, 0, 0, 1},
      {1, 0, 0, 1}
   };
 
//int steps_left=2048;  // Variable que define o número de voltas que vai a
//dar o motor. Con 4096 daría una vuelta completa.
 
boolean Direcion = true; //Define o sentido do motor.
 
int Steps = 0;       // Define o paso actual da secuencia
 
int Vanalox = 0;     // Define a variable de lectura analoxica
 
int RETARDO = 1000;     //Tempo en microsegundos que tarda o motor en dar un paso
 
void setup() {
     pinMode(M1, OUTPUT); //Declaramos como saidas as fases do motor
     pinMode(M2, OUTPUT);
     pinMode(M3, OUTPUT);
     pinMode(M4, OUTPUT);
     
     pinMode(Iana, INPUT);
     Serial.begin(9600);
         
}
 
void loop()
   {
    int Vanalox = analogRead(Iana); // Lemos a entrada analoxica conctada o Joystick
 
    if (Vanalox > 520)  {Direcion = true;}  // Definimos o sentido de xiro
    if (Vanalox < 500)  {Direcion = false;} // Cambiamos o sentido de xiro
 
    if (Vanalox > 520 or Vanalox < 500)      // Se o valor non é o de repouso do Joystick movemos o motor
    {
        stepper() ;     // Avanza un paso
 
        if (Vanalox > 1000 or Vanalox < 60) {RETARDO = 500;} // comprobamos se o potenciometro está no extremo para poder axustar a velocidade
       
        else {RETARDO = 1000;}  // se non esta cerca do extremo velocidade normal 
        //( poderiamos facer un retardo inverso a resistencia e vaiar a velocidade proporcioal o potenciometro)
 
        delayMicroseconds(RETARDO); // retardamos para o seguinte paso
    }

 
     
     apaga(); // apagamos o motor para consumir menos

     
     
   }
 
void stepper()            //Avanza un paso
   {  
      digitalWrite( M1, Paso[Steps][ 0] );
      digitalWrite( M2, Paso[Steps][ 1] );
      digitalWrite( M3, Paso[Steps][ 2] );
      digitalWrite( M4, Paso[Steps][ 3] );
      /*
       * En funcion da posición na que estemos da matriz activanse unha fase
       * ou outra.
       * Logo con setDirecion avanzamos unha posición ou a retrocedemos.
       */
      SetDirecion();
   }
 
void SetDirecion() // Establece o  sentido
   {  
      if(Direcion)
         {Steps++;}
      else
         {Steps--;}
 
   
      /* Axusta os límites ao tamaño do array */
      Steps = (Steps +8) %8;
      }
 
void apaga()            //Apaga o motor
   {  digitalWrite( M1, 0 );
      digitalWrite( M2, 0 );
      digitalWrite( M3, 0 );
      digitalWrite( M4, 0 );
   }
