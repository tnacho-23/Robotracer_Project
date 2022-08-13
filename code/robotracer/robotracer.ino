//Librerías
#include <QTRSensors.h>

//Definición de componentes y sus pines
int Buzzer = 2;
int AIN1 = 3; //A: Motor Izquierdo
int AIN2 = 4;
int PWMA = 5; //Velocidad A
int PWMB =6; //Velocidad B
int BIN1 = 7; //B: Motor Derecho
int BIN2 = 8;
int StandBy = 9; //HIGH para QTR y Driver motor
int Button = 10; //Botón multifunción
int QTR1 = A0; // Sensor Frontal Extremo Izquierdo
int QTR2 = A1;
int QTR3 = A2;
int QTR4 = A3;
int QTR5 = A4;
int QTR6 = A5; //Sensor Frontal Extremo Derecho
int TCR1 = A6; //Sensor lateral Izquierdo
int TCR2 = A7; //Sensor lateral Derecho

QTRSensors qtr;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

//Setup
void setup() {
  Serial.begin(9600);

  //Sensors Config
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, SensorCount);
  qtr.setEmitterPin(StandBy);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

  //Pin Setup
  pinMode(Buzzer, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(StandBy,OUTPUT);
  pinMode(Button,INPUT);
  pinMode(QTR1,INPUT);
  pinMode(QTR2,INPUT);
  pinMode(QTR3,INPUT);
  pinMode(QTR4,INPUT);
  pinMode(QTR5,INPUT);
  pinMode(QTR6,INPUT);
  pinMode(TCR1,INPUT);
  pinMode(TCR2,INPUT);
  digitalWrite(StandBy,HIGH);
 

  //Motores Apagados
  motor_der("apagado",0);
  motor_izq("apagado",0);
}

//Main Loop
void loop() {
  uint16_t position = qtr.readLineBlack(sensorValues); //Se puede cambiar a línea blanca con readLineWhite()
  if(position < 2200){
    motor_der("adelante", 150);
    motor_izq("adelante", 50);
  }
  if(position > 2700){
    motor_der("adelante",50);
    motor_izq("adelante",150);
  }
  if(position>=2200 and position<= 2700){
    motor_der("adelante",255);
    motor_izq("adelante",255);
  }
}

//Funciones Auxiliares

void motor_izq(String movimiento, int velocidad){
  if(movimiento == "adelante"){
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,HIGH);
    analogWrite(PWMA,velocidad);
  }
  if(movimiento == "atrás"){
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA,velocidad);
  }
  if(movimiento == "apagado"){
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA,0);
  }
}

void motor_der(String movimiento, int velocidad){
  if(movimiento == "adelante"){
    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2,HIGH);
    analogWrite(PWMB,velocidad);
  }
  if(movimiento == "atrás"){
    digitalWrite(BIN1,HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB,velocidad);
  }
  if(movimiento == "apagado"){
    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB,0);
  }
}
