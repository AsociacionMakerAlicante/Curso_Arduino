#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Wire.h>

#define ANCHO_PANTALLA 128 // OLED display width, in pixels
#define ALTO_PANTALLA 32   // OLED display height, in pixels
#define PIN_TERMOMETRO PIN_A0
#define PIN_PULSADOR DD2

// Declaracion para SSD1306 display conectados por I2C (SDA, SCL pins)
// Los pines I2C están definidos en la biblioteca Wire.
// Para arduino UNO, Nano:       A4(SDA), A5(SCL)
#define RESET_PANTALLA -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define DIRECCION_PANTALLA 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define RESOLUCION_LECTURA 1023
#define TENSION_REFERENCIA 1.5f
#define TENSION_A_0C 0.5f

void mostrarPantalla();
float leeTemperatura();

/* 
  Parámetros de la declaración
  ----------------------------
  1 - Nº pixel ancho pantalla
  2 - Nº pixel alto pantalla
  3 - Referencia a la biblioteca Wire
  4 - Pin "reset" pantalla
*/
Adafruit_SSD1306 pantalla(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, RESET_PANTALLA);

// Clase para el control del reloj.
RTC_DS1307 reloj;

// Variable de donde se almacenrá la fecha y hora
DateTime tiempo;
float grados;
char cadenaFormateada[9];

/*
  Variable que va a controlar cuando tenemos que refrescar los datos de la pantalla.
  Al cambiar su valor dentro de una interrupción tiene que declararse con el prefijo
  "volatile". Si no se hace así podria fallar en determinadas circunstancias.
*/ 
volatile bool presentarDatos;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200); // Incluir el platformio.ini "monitor_speed = 115200"

   // Fijamos la tensión de referencia interna para el ADC en 1,5V
   analogReference(INTERNAL1V5);


  // Inicializamos el reloj.
  reloj.begin();

  // Ponemos la fecha y hora del momento de la compilación
  reloj.adjust(DateTime(__DATE__, __TIME__));
  
  Serial.println(F("Reloj puesto en hora."));
  
  // Activamos la pantalla.
  pantalla.begin(SSD1306_SWITCHCAPVCC, DIRECCION_PANTALLA);
  Serial.println(F("Pantalla de inicio."));
  
  // Mostrar pantalla de inicio de Adafruit.
  pantalla.display();
  delay(2000);
  
  Serial.println(F("Borramos pantalla."));
  pantalla.clearDisplay();
  pantalla.display();
  
  // Configuramos la pantalla.
  pantalla.setTextSize(2);              // Grande 2:1 pixel escala
  pantalla.setTextColor(SSD1306_WHITE); // Definir color blanco
  pantalla.setCursor(0, 0);             // Situamos el cursor en la parte superior izquierda de la pantalla.
  pantalla.cp437(true);                 // Habilitamos la página de códigos 437
  
  Serial.println(F("Pantalla configurada correctamente."));

  // Leemos la fecha y hora.
  tiempo = reloj.now();

  // Pantalla bienvenida.
  // Enviamos el texto al buffer de la pantalla.
  pantalla.println(F("   Curso"));
  pantalla.println(F("  Arduino"));

  // Mostramos lo que hay en el buffer de la pantalla.
  pantalla.display();

  // Configuramos la interrupción
  pinMode(PIN_PULSADOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PULSADOR), mostrarPantalla, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(presentarDatos){
    // Ponemos a false la variable para que en el siguiente bucle no entre.
    presentarDatos = false;

    // Leemos la temperatura.
    grados = leeTemperatura();

    // Leemos la fecha y la hora.
    tiempo = reloj.now();

    // Borramos la pantalla
    pantalla.clearDisplay();

    // Formateamos el texto y lo enviamos a pantalla.
    // Guardamos en un char array la hora formateada como HH:MM:SS
    pantalla.setCursor(0, 0);
    sprintf(cadenaFormateada, "%02u:%02u:%02u", tiempo.hour(), tiempo.minute(), tiempo.second());
    pantalla.println(cadenaFormateada);
    
    pantalla.setCursor(0,16);
    sprintf(cadenaFormateada, "%.2f", grados);
    pantalla.print(cadenaFormateada);
    pantalla.print("C");
    pantalla.display();
  }
}

void mostrarPantalla(){
  presentarDatos = true;
}

float leeTemperatura(){
  static int lecturaADC;
  static float tension;
  static float temperatura;
  // Leemos el valor que nos devuelve el pin analógico.
  lecturaADC = analogRead(PIN_TERMOMETRO);
  Serial.print("Lectura ADC = "); Serial.println(lecturaADC);

  // Convertimos la lectura en voltios.
  tension = TENSION_REFERENCIA * lecturaADC / RESOLUCION_LECTURA;
  Serial.print("Tensión calculada = ");
  Serial.println(tension);

  // Ahora convertimos la tensión obtenida en temperatura.
  temperatura = (tension - TENSION_A_0C) * 100.0f;
  Serial.print("Temperatura calculada = ");
  Serial.println(temperatura);

  return temperatura;
} 