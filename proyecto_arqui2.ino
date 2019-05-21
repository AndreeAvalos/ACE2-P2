#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <ArduinoJson.h>

int tiempo=0;
int distancia=4;//distancia del ultrasonico a humano
const int Trigger = 2;   //Pin digital 2 para el Trigger del sensor
const int Echo = 3;   //Pin digital 3 para el Echo del sensor
/*Pines de los colores indicadores*/
const int rojo=4;
const int amarillo=5;
const int verde=6;
const int bomba=7;

int tiempoCancion=60;//Este el tiempo en el que deberia de baÃ±arse
int duracionBano=0;
int tiempoAmarillo=0;
int miliGastados=0;
boolean inicio=false;
boolean fin=false;
SoftwareSerial mySerial(10,11); //RX,TX
String frase;
/*Parte para conectar con WIFI*/
char ssid[] = "HUAWEI Mate 10 Pro";//Tech_D0016808
char pass[] = "ambrosio1809";//XGEVMENN
int status = WL_IDLE_STATUS;
char server[] = "proyecto2-arqui2.azurewebsites.net";
WiFiEspClient clienteServidor;

void setup() {
  Serial.begin(115200);//iniciailzamos la comunicaciÃ³n
  mySerial.begin(9600);
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(bomba, OUTPUT);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  pinMode(A0,OUTPUT);
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  while (status != WL_CONNECTED)
    {
        Serial.print("Conectando con la red WIFI: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
    }
  Serial.print("IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());
  Serial.println("Esta conectado a la red");
}

void loop() {
  //distanciaUltrasonico(10);
  //Serial.print("estatus wifi corriendo: ");
    //  Serial.println(status);
  serialEvent();
  Indicadores();
  //enviarDatos();
}
void serialEvent(){
  while (mySerial.available()){
    if(mySerial.available()>0){
      char letra= mySerial.read();
      int ascii = letra;
      //Serial.println(ascii);
      //Serial.print("ascii: ");
      //Serial.println(ascii);
      if(convertirAscii(ascii)){
        frase+=letra;
      }
      //frase+=letra;
      //Serial.println(letra);
    }
  }
  if(frase.length()==3){
    Serial.print("longitud de datos: ");
    Serial.print(frase.length());
    Serial.print(" datos recibidos ");
    tiempoCancion=frase.toInt();
    Serial.println(tiempoCancion);
    duracionBano=0;
    //inicio=true;
    frase="";
  }
  frase="";
}
void enviarDatos(){
  //delay(1000);
  if(status != WL_CONNECTED){
    Serial.println("No esta conectado al WIFI");
  }
  
  String yourdata="{\"Id_ducha\": 0, \"Tiempo\":"+ String(duracionBano/60)+", \"Fecha\": \"\", \"Genero\": \"Rock\", \"Ahorro\":"+String(miliGastados)+" }";
  //String yourdata2="{\"Id_ducha\": 4, \"Tiempo\": 1, \"Fecha\": \"\", \"Genero\": \"Rock\", \"Ahorro\":"+String(miliGastados)+" }";
  if(clienteServidor.connect(server,80)){
    Serial.println("Entro al servidor");
    Serial.println(yourdata);
    int value = 2.5;  // an arbitrary value for testing
    String content = "{\"JSON_key\": " + String(value) + "}";
    clienteServidor.println("POST /api/medicion HTTP/1.1");
    clienteServidor.println("Host: proyecto2-arqui2.azurewebsites.net");
    clienteServidor.println("Accept: */*");
    clienteServidor.println("Content-Length: " + String(yourdata.length()));
    clienteServidor.println("Content-Type: application/json");
    clienteServidor.println();
    clienteServidor.println(yourdata);
    Serial.println("Done!");
    Serial.println("Disconnecting from server...");
    clienteServidor.stop();
  }else{
    Serial.println("Fallo la conexion");
    Serial.println("Desconectando.");
    clienteServidor.stop();
  }
  delay(8000);
}

void Indicadores(){
  /*if(!inicio){
    tiempoCancion=tiempoEnSegundos;
    inicio=true;
  }*/
  if(tiempoCancion>0){
    if(tiempoCancion<=40){
      digitalWrite(verde, LOW);
      digitalWrite(rojo, LOW);
      digitalWrite(amarillo, HIGH);
      Serial.print("Estado amarillo: ");
      Serial.print(tiempoCancion);
      Serial.println();
      if(distanciaUltrasonico(10)){
        digitalWrite(bomba, HIGH);
        miliGastados+=10;
        //analogWrite(A0,250);
      }else{
        digitalWrite(bomba, LOW);
      }
    }else if(tiempoCancion>40){
      fin=false;
      digitalWrite(amarillo, LOW);
      digitalWrite(verde, HIGH);
      digitalWrite(rojo, LOW);
      Serial.print("Estado verde: ");
      Serial.print(tiempoCancion);
      Serial.println();
      if(distanciaUltrasonico(10)){
        digitalWrite(bomba, HIGH);
        miliGastados+=10;
        //analogWrite(A0,250);
      }else{
        digitalWrite(bomba, LOW);
      }
    }
    tiempoCancion--;
    duracionBano++;
  }else{
    digitalWrite(amarillo, LOW);
    digitalWrite(verde, LOW);
    digitalWrite(rojo, HIGH); 
    Serial.print("Estado finalizado: ");
    Serial.print(tiempoCancion);
    Serial.println();
    
    if(!fin){
      Serial.print("estatus wifi final: ");
      Serial.println(status);
      digitalWrite(bomba, LOW);
      delay(5000);//esto para evitar que el ruido contamine los datos
      enviarDatos();
      fin =true;
      duracionBano=0;
      miliGastados=0;
    }
  }
  delay(1000);
}

boolean convertirAscii(int ascii){
  switch(ascii){
    
    case 48:
      return true;
      
    case 49:
      return true;
    case 50:
      return true;
    case 51:
      return true;
    case 52:
      return true;
    case 53:
      return true;
    case 54:
      return true;
    case 55:
      return true;
    case 56:
      return true;
    case 57:
      return true;
    default:
      return false;
  }
}

boolean distanciaUltrasonico(long distancia){
  long t; //tiempo que demora en llegar el eco
  long d; //distancia en centimetros
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;
  if(d<=distancia){
    Serial.print("Distancia: ");
    Serial.print(d);
    Serial.print(" cm");
    Serial.println();
    return true;
   // digitalWrite(bomba, HIGH);
  }else{
    Serial.print("Distancia: ");
    Serial.print(d);
    Serial.print(" cm");
    Serial.println();
    return false;
    //digitalWrite(bomba, LOW);
  }
  //delay(100);          //Hacemos una pausa de 100ms
}


