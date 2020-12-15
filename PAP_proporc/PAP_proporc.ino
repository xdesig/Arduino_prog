/* Xabier Rosas
 *  
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
 * Cambio de xiro e cambio de velocidade
 * 
 */



#define M1  8     //Definimos as saidas que compoñen as bobinas. 
#define M2  9
#define M3  10
#define M4  11

#define Iana 0   //Definimos a entrada analoxica para comparar


//Esta é a variable paso. Como vemos, es unha matriz que inclue cada unha das
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

int Direcion = 0;    // Define o sentido del motor 1=adiante, 0=parado, -1=atrás

int Steps = 0;       // Define o paso actual de la secuencia

int Vanalox = 0;     // Define a variable de lectura analoxica

int Retardo = 0;     // Define unha variable para calcular o retardo entre pasos

// os catro valores clave do valor da resistencia do joystick:
int MIN_NEGATIVO = 0;     // Define o valor mínimo que pode coller a entrada analóxica
int OFF_NEGATIVO = 500;   // Define o valor para o apagado chegando do negativo
int OFF_POSITIVO = 520;   // Define o valor para o apagado chegando do positivo
int MAX_POSITIVO = 1023;  // Define o valor máximo que pode coller a entrada analóxica

int RETARDO_MIN = 60;     // Retardo mínimo en microsegundos entre pasos
int RETARDO_MAX = 1000;   // Retardo máximo en microsegundos entre pasos



void setup() 
{
     // Saídas para o motor
     pinMode(M1, OUTPUT); 
     pinMode(M2, OUTPUT);
     pinMode(M3, OUTPUT);
     pinMode(M4, OUTPUT);
     
     // Entrada do joystick
     pinMode(Iana, INPUT);
}



void loop()
{
    Vanalox = analogRead(Iana); // Lemos a entrada analoxica conctada o Joystick

    Direcion = 0; // en principio parado, pero avaliar segundo valor en Vanalox:
    if (Vanalox > OFF_POSITIVO)  { Direcion = 1; }  // sentido de xiro cara diante
    if (Vanalox < OFF_NEGATIVO)  { Direcion = -1; } // sentido de xiro cara atrás

    if (Direcion == 0) // Parar o motor:
    {
        apagar();
    }
    else // Se o valor non é 0 hai que mover nunha dirección:
    { 
        // Executar un paso
        stepper() ;     

        // Esperar para o seguinte paso
        retardar();
    }
}

     
// Avanzar un paso
void stepper()            
{  
    // Activar a fase do paso que toca
    digitalWrite( M1, Paso[Steps][ 0] );
    digitalWrite( M2, Paso[Steps][ 1] );
    digitalWrite( M3, Paso[Steps][ 2] );
    digitalWrite( M4, Paso[Steps][ 3] );

    // Avanzar á seguinte fase 
    calcularSeguintePaso();
}



// Retardar o tempo para o seguinte paso
void retardar()
{
    // Calcular o retardo proporcional á resistencia coa axuda da función map()
    
    if (Direcion = -1) // atrás
    {
        Retardo = map(Vanalox, MIN_NEGATIVO, OFF_NEGATIVO , RETARDO_MIN, RETARDO_MAX);
    }

    if (Direcion = 1) // adiante
    {
        Retardo = map(Vanalox, OFF_POSITIVO, MAX_POSITIVO, RETARDO_MIN, RETARDO_MAX);
    }

    // esperar os microsegundos de retardo
    delayMicroseconds(Retardo); 
}



// Establecer o seguinte paso en función da dirección
void calcularSeguintePaso() 
{  
    // Adiante ou atrás
    if(Direcion)
        { Steps++; }
    else
        { Steps--; }

    /* Axusta os límites ao tamaño do array dos pasos */
    Steps = (Steps +8)% 8;
}



// Apagar o motor
void apagar()            
{  
    digitalWrite( M1, 0 );
    digitalWrite( M2, 0 );
    digitalWrite( M3, 0 );
    digitalWrite( M4, 0 );
}
