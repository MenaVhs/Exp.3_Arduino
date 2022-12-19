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
int numCiclos = 20;
//Ojo: para tiempos con punto decimal, ejemplo: 0.5 min = 30 segundos
float Ds = 0.5;          // minutos
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
bool buzzEstado = LOW;  //Activación del buzzer
bool ledEstado = LOW;

// Banderas
bool flag1 = true;

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
    } else {
      Serial.print("Tiempo (seg): ");            //PRUEBA
      Serial.print(float(tiempoActual / 1000));  //PRUEBA
      unsigned long cicloActual = millis() - Dluz - Ti;
      unsigned long ciclo = cicloActual - cicloAnterior;
      // Inicio de ciclos
      if (numCiclos > 0) {

        // Serial.print("cicloActual: "); //PRUEBA
        // Serial.print(cicloActual); //PRUEBA
        // Serial.print(" ");
        // Serial.print(ciclo);

        if (flag1 == false)
          noTone(buzzerPin);

        // Presentación de Tono
        if (ciclo <= Dt && flag1 == true) {

          Serial.print("\tPresentación del tono (Dt)\t");
          tone(buzzerPin, Hz);                  // Activación del buzzer
          buzzEstado = HIGH;                    //PRUEBA
          digitalWrite(buzzerPin, buzzEstado);  //PRUEBA
        }

        // Intervalo entre tonos
        if (ciclo > Dt && ciclo < Dt + IEt) {
          noTone(buzzerPin);
          //Serial.print("\tIntervalo entre tonos (IEt)\t");  //PRUEBA
          buzzEstado = LOW;                     //PRUEBA
          digitalWrite(buzzerPin, buzzEstado);  //PRUEBA
        }
        // Reset de intervalo
        if (ciclo >= Dt + IEt && numCiclos != 0) {  // ciclo <= 3000
          tone(buzzerPin, Hz);                      // Activación del buzzer
          cicloAnterior = cicloActual;
          digitalWrite(buzzerPin, !digitalRead(buzzerPin));  //PRUEBA
          Serial.print("\tPresentación del tono (Dt)\t");

          flag1 = false;
          numCiclos -= 1;
        }
        if (numCiclos == 0) {
          Serial.print("No tono");
          noTone(buzzerPin);
        }
      }
      Serial.print("\t numCiclos: ");  //PRUEBA
      Serial.println(numCiclos);       //PRUEBA
      delay(1000);                     //PRUEBA
    }
  } else if (tiempoActual < 4294960000) {
    // Señal de fin de sesión
    noTone(buzzerPin);  // Asegurar que buzzer se apague.
    Serial.println("Fin \n");
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(Dluz);  // 3s
    digitalWrite(ledPin, !digitalRead(ledPin));
    digitalWrite(buzzerPin, LOW);  

    while (1) {
      // >>>>>>>>>>>>>> FIN  de  la sesión <<<<<<<<<<<<<<<<<
    }
  }
}  //  >>>>>>>>>>>>>> FIN  de  loop( ) <<<<<<<<<<<<<<<<<

