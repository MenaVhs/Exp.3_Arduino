/*
Título:
Autora proyecto: Isiris Guzmán
Autora código: Jimena García
Fecha: 18/12/2022

numCiclos = número de veces que se repite un ciclo
Ds = tiempo de la sesión en minutos
Ti = tiempo de inicio
Dt = duración del tono 
IEt = Intervalo entre tonos 
Dluz = duración de la luz
Hz = Frecuencia del buzzer
*/

//======================================================================
//                         DATOS MODIFICABLES 
//======================================================================
int numCiclos = 2;
//Ojo: para tiempos con punto decimal, ejemplo: 0.5 min = 30 segundos
float Ds = 0.5;          // minutos
unsigned long Ti = 2;    // segundos
unsigned long Dt = 1;    // segundos
unsigned long IEt = 5;   // segundos
unsigned long Dluz = 3;  // segundos
int Hz = 7000;           // ejemplo: 1000 = 1KHz. Verificar si físicamente da la frecuencia

// Pines
#define buzzerPin 8
#define ledPin 13


//======================================================================
//                      DATOS NO MODIFICABLES 
//======================================================================

// Tiempo anterior al actual
unsigned long tiempoAnterior = 0;

// Estado del buzzer
boolean buzzEstado = LOW;  //Activación del buzzer
boolean ledEstado = LOW;

//======================================================================
//                            FUNCIONES
//======================================================================
const unsigned long ConvertMilli(unsigned long sec) {
  return sec * 1000;
}

unsigned long ConvertMinToSec(float Ds) {
  return Ds * 60;
}


//======================================================================
//                              CÓDIGO
//======================================================================
void setup() {
  //Inicialización del puerto serial
  Serial.begin(9600);
  while (!Serial);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(buzzerPin, buzzEstado);  //Cambio del estado del buzzer

  // Conversiones
  Ds = ConvertMilli(ConvertMinToSec(Ds)); 
  Dt = ConvertMilli(Dt);
  Ti = ConvertMilli(Ti);
  IEt = ConvertMilli(IEt);
  Dluz = ConvertMilli(Dluz);
  
  
  // Señal de inicio de sesión
  digitalWrite(ledPin,!digitalRead(ledPin));
  delay(Dluz); // 3s
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long tiempoActual = millis() - Dluz - Ti;
  unsigned long tiempo = tiempoActual - tiempoAnterior;
  //Serial.println(tiempoActual);
  // Inicio de la sesión
  if((unsigned long)tiempoActual <= Ds){
    // Primer intervalo de tiempos, sólo se presenta al inicio de la sesión
    if(tiempoActual <= Ti){
      delayMicroseconds(Ti); 
      // Serial.println("Ti");
      // delay(1000);   
    } else{
      // Inicio de ciclos
      if(numCiclos > 0){
        // Tiempo del tono
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Me quedé en iniciar los ciclos, justo después de terminar Ti. Los ciclos no deben tomar en cuenta el valor de Ti, sólo al inicio.
        if(tiempo <= Dt ){  // Tiempo inicial = 2s 
          Serial.println("tiempo durante de Dt"); //BORRAR AL FINAL
          Serial.println(tiempo); //BORRAR AL FINAL
          tone(buzzerPin, Hz);
          buzzEstado = HIGH; //BORRAR AL FINAL
        }digitalWrite(buzzerPin, buzzEstado); //BORRAR AL FINAL
 

      }numCiclos -=1;
      // Serial.println(numCiclos);
      // delay(1000);  
      
    } 
  }
  else if(tiempoActual < 4294965295){
    // Señal de fin de sesión
    digitalWrite(ledPin,!digitalRead(ledPin));
    delay(Dluz); // 3s
    digitalWrite(ledPin, LOW);

    Serial.println("\t");
    Serial.println("Fin");
    while(1){
      // >>>>>>>>>>>>>> FIN  de  la sesión <<<<<<<<<<<<<<<<<
    }
  } 
} //  >>>>>>>>>>>>>> FIN  de  loop( ) <<<<<<<<<<<<<<<<<


