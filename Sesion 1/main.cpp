/*
Códigos de ejemplo:
Ejemplo 1 y 2:
  Dos formas distintas de hacer parpadear el led interno.

Ejemplo 3:
  Lee el estado del pulsador y lo pasa al led, de forma que
  el led solo está encendido mientras el pulsador se presiona
  en el momento que se suelta el led se apaga.

Ejemplo 4:
  El pulsador se utiliza para cambiar el estado del led.
  Cada vez que se pulsa cambia de estado. De esta forma
  al pulsarlo la primera vez el led se enciende y permanece
  encendido aunque se suelte el pulsador hasta la siguiente 
  pulsación.
  Con la nueva pulsación el led se apaga y permanece apagado
  hasta la siguiente pulsación.

Los pulsadores tienen "rebotes" si no se monta alguna
protección. Esto significa que aunque pulsemos solo una vez
el micro registrará n pulsaciones. Esto hace que el ejemnplo
4 no funcione correctamente si no se monta un circuito
antirebotes.
Para poder entender lo que pasa montar primero el pulsador solo
y probar. Después montar el circuito antirebotes y volver a probar
*/
#include <Arduino.h>
#define EJEMPLO4          // Selecciona el ejemplo a compilar

#define PINPULSADOR DD4   // El pulsador se conecta a D4
#define PINLED      DD2   // El led se conecta a D2

void cambiaEstado();

bool pulsado;       // Lectura del estado del pulsador (True = pulsado/ False = Sin pulsar)
bool activo;        // Último estado del pulsador 
bool encendido;     // Estado del led.

void setup()
{
  // Configuración hardaware.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  pinMode(PINPULSADOR, INPUT_PULLUP);
  pinMode(PINLED, OUTPUT);
}

void loop()
{
#if defined(EJEMPLO1)
  // Tiempo encendido = Tiempo apagado
  // Escribe en el pin el valor contrario al que tiene.
  // Si tiene 1 escribe 0 y si tiene 0 escribe 1
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  // Envia información a pantalla
  Serial.println(F("Cambiando estado led"));
  delay(250);
#endif

#if defined(EJEMPLO2)
  // Tiempo encendido != Tiempo apagado
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(F("Led encendido."));
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("Led apagado"));
  delay(750);
#endif

#if defined(EJEMPLO3)
  // Guardamos en "pulsado el estado del pulsador"
  pulsado = digitalRead(PINPULSADOR);

  // En función del estado hacemos cosas distintas.
  if (pulsado)
  {
    // Si Pulsado = 1 (True) ponemos a (HIGH) el pin
    digitalWrite(PINLED, LOW);
  }
  else
  {
    // Si Pulsado = 0 (False) ponemos a (LOW) el pin
    digitalWrite(PINLED, HIGH);
  }
#endif

#if defined(EJEMPLO4)
  // Leemos el estado del pulsador.
  pulsado = digitalRead(PINPULSADOR);

  // El pulsador estaba en reposo comprobamos si lo han pulsado.
  if (!activo)
  {
    if (pulsado)
    {
      // Si estaba en repososo y lo han pulsado, lo marcamos como pulsado
      // y cambiamos el estado del led.
      activo = false;
      cambiaEstado();
    }
  }

  // El pulsador estaba pulsado. Comprobamos si lo han soltado.
  if (activo)
  {
    if (!pulsado)
    {
      // Si estaba en pulsado y lo han soltado, lo marcamos como inactivo (sin pulsar)
      activo = true;
    }
  }

#endif
}

// Cambia de estado la variable encendido.
void cambiaEstado()
{
  // Guardamos en encendido el valor contrario al que tiene.
  // Si es true lo pasamos a false y si es false lo pasamos a
  encendido != encendido;
  digitalWrite(PINLED, encendido);
}
