#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

int sensorPin = A0;    // Pin de entrada del sensor
int sensorValue = 0;  // valor que capta el sensor
const char *port = "192.168.1.1"; // ip del default gateway
const char* url = "http://jsonplaceholder.typicode.com/users";
const char* id = "AndroidAP0D29"; // nombre de la red
const char* pass = "hola12345"; // password

int contador = 0; // verifica si tarda mucho en conectarse a la red

LiquidCrystal_I2C lcd(0x27,16,2); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 

void setup() {
  Serial.begin(115200);
  
  inicializarLCD();
  incializarWifi();

  lcd.clear();
  EscribirLcdFila1("Conexion");
  EscribirLcdFila2("Establecida");
  
  delay(3000);
  pinMode(sensorPin, INPUT);
}

void loop() {
    // toma el valor del sensor y lo asigna a sensorValue
    lcd.clear();
    sensorValue = analogRead(sensorPin);

    // lo escribe en el LCD
    EscribirLcdFila1("Lumenes: "+ String(sensorValue));
    delay(3000); // espera 3 segundos hasta que haga el post
    lcd.clear();

    // hace el post
    hacerPost(String(sensorValue));

    lcd.clear();
    EscribirLcdFila1("Espere 10");
    EscribirLcdFila2("Segundos...");
    delay(10000); // 10 segundos para hacer otro post
   
}

void EscribirLcdFila1(String texto){
  lcd.setCursor(0,0);
  lcd.print(texto);
  delay(100);
}

void EscribirLcdFila2(String texto){
  lcd.setCursor(0,1);
  lcd.print(texto);
  delay(100);
}

void inicializarLCD(){
  lcd.init();
  lcd.backlight();
}

void incializarWifi(){
  WiFi.mode(WIFI_OFF);
  WiFi.begin(id,pass);
  WiFi.mode(WIFI_STA);

  while((WiFi.status() != WL_CONNECTED)){
     delay(1000); // 1 segundo
     EscribirLcdFila1("Esperando");
     EscribirLcdFila2("Conexion");
     lcd.clear();
     contador++;
     if(contador >=10){
        break;
     }
  }
}

void hacerPost(String sensor){
  if(WiFi.status()== WL_CONNECTED){
    delay(1000);
    HTTPClient http;
    http.begin(port);

    http.begin(url);
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.POST(sensor);

    lcd.clear();
    EscribirLcdFila1("Respuesta: ");
    if(httpCode == 201){ EscribirLcdFila2("OK");}else{EscribirLcdFila2("BAD REQ");}
    
    delay(3000);
    http.end();
             
  }else{
    while(WiFi.status()!= WL_CONNECTED){
     lcd.clear();
     EscribirLcdFila1("Esperando");
     EscribirLcdFila2("Conexion");
     delay(100);
    }
  }
}
