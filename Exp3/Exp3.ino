/*
Título:
Autora proyecto: Isiris Guzmán
Autora código: Jimena García
Fecha: 19/12/2022

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
float Ds = 0.25;         // minutos
unsigned long Ti = 2;    // segundos
unsigned long Dt = 1;    // segundos
unsigned long IEt = 2;   // segundos
unsigned long Dluz = 2;  // segundos
int Hz = 7000;           // ejemplo: 1000 = 1KHz. Verificar si físicamente da la frecuencia

// Pines
#define buzzerPin 8
#define ledPin 12

//======================================================================
//                      DATOS NO MODIFICABLES
//======================================================================

// Tiempo anterior al actual
unsigned long tiempoAnterior = 0;
unsigned long cicloAnterior = 0;

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
  while (!Serial)
    ;

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
  digitalWrite(ledPin, !digitalRead(ledPin));
  delay(Dluz);  // 3s
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long tiempoActual = millis() - Dluz;
  unsigned long tiempo = tiempoActual - tiempoAnterior;
  // Inicio de la sesión
  if ((unsigned long)tiempoActual <= Ds) {
    // Primer intervalo de tiempos, sólo se presenta al inicio de la sesión
    if (tiempoActual <= Ti) {
      delayMicroseconds(Ti);
      //PRUEBA: Sí entra a los 2s
    } else {
      unsigned long cicloActual = millis() - Dluz - Ti;
      unsigned long ciclo = cicloActual - cicloAnterior;
      // Inicio de ciclos
      if (numCiclos > 0) {

        Serial.print("cicloActual: ");
        Serial.print(cicloActual);
        Serial.print(" ");
        Serial.print(ciclo);
        // Presentación de Tono ///////////////
        if (ciclo <= Dt) {
          Serial.print("\tPresentación del tono (Dt)\t");
          tone(buzzerPin, Hz);                  // Activación del buzzer
          buzzEstado = HIGH;                    //PRUEBA
          digitalWrite(buzzerPin, buzzEstado);  //PRUEBA
        }

        // Intervalo entre tonos
        if (ciclo > Dt && ciclo < Dt + IEt) {
          noTone(buzzerPin);
          Serial.print("\tIntervalo entre tonos (IEt)\t");  //PRUEBA
          buzzEstado = LOW;                                 //PRUEBA
          digitalWrite(buzzerPin, buzzEstado);              //PRUEBA
        }
        // Reset de intervalo
        if (ciclo >= Dt + IEt && numCiclos != 0) {  // ciclo <= 3000
          cicloAnterior = cicloActual;
          digitalWrite(buzzerPin, !digitalRead(buzzerPin));  //PRUEBA
          Serial.print("\tPresentación del tono...(Dt)\t");
          numCiclos -= 1;
          tone(buzzerPin, Hz);  // Activación del buzzer
        }
        if (numCiclos == 0) {
          noTone(buzzerPin);
        }
      }

      Serial.print("\t\t numCiclos: ");  //PRUEBA
      Serial.println(numCiclos);         //PRUEBA
      delay(1000);                       //PRUEBA
    }
  } else if (tiempoActual < 4294960000) {
    // Señal de fin de sesión
    Serial.println("Fin \n");
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(Dluz);  // 3s
    digitalWrite(ledPin, !digitalRead(ledPin));
    noTone(buzzerPin);             // Asegurar que buzzer se apague.
    digitalWrite(buzzerPin, LOW);  //PRUEBA

    while (1) {
      // >>>>>>>>>>>>>> FIN  de  la sesión <<<<<<<<<<<<<<<<<
    }
  }
}  //  >>>>>>>>>>>>>> FIN  de  loop( ) <<<<<<<<<<<<<<<<<
