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
int Btn = 10; //Botón multifunción
int QTR1 = A0; // Sensor Frontal Extremo Izquierdo
int QTR2 = A1;
int QTR3 = A2;
int QTR4 = A3;
int QTR5 = A4;
int QTR6 = A5; //Sensor Frontal Extremo Derecho
int TCR1 = A6; //Sensor lateral Izquierdo
int TCR2 = A7; //Sensor lateral Derecho

//Variables Buzzer
int pBuzzer = 2;
int entero = 1000/2;
int medio = 1000/4;
int tercio = 1000/6;
int cuarto = 1000/8;
int octavo = 1000/16;
double pausa = 1.30;

int Do = 262;
int Re = 294;
int Mi = 330;
int Fa = 349;
int Sol = 392;
int La = 440;
int Si = 493;
int Do1 = 523;
int Re1 = 587;
int Mi1 = 659;
int Fa1 = 699;
int Sol1 = 784;

QTRSensors qtr;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];


//Constantes PID
float kp = 0.045;
float kd = 0.03;
float ki = 0.0065;
float vel =100;


//Setup
void setup() {
  Serial.begin(9600);

    //Pin Setup
  pinMode(Buzzer, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(StandBy,OUTPUT);
  pinMode(Btn,INPUT);
  pinMode(QTR1,INPUT);
  pinMode(QTR2,INPUT);
  pinMode(QTR3,INPUT);
  pinMode(QTR4,INPUT);
  pinMode(QTR5,INPUT);
  pinMode(QTR6,INPUT);
  pinMode(TCR1,INPUT);
  pinMode(TCR2,INPUT);
  digitalWrite(StandBy,HIGH);

  //Buzzer Start

  //Sensors Config
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, SensorCount);
  qtr.setEmitterPin(StandBy);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  tone(Buzzer, Sol, cuarto);
  delay(cuarto*pausa);
  digitalWrite(LED_BUILTIN, HIGH);
   
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
  tone(Buzzer, Sol, cuarto);
  delay(cuarto*pausa);
  tone(Buzzer, Sol, cuarto);
  delay(cuarto*pausa);
  
  //Motores Apagados
  motor_der("apagado",0);
  motor_izq("apagado",0);
}

//Main Loop
void loop(){
  //if(position < 2200){
    //motor_der("adelante", 150);
    //motor_izq("adelante", 50);
  //}
  //if(position > 2700){
    //motor_der("adelante",50);
    //motor_izq("adelante",150);
  //}
  //if(position>=2200 and position<= 2700){
    //motor_der("adelante",255);
    //motor_izq("adelante",255);
  //}
  float E = 0;
  float E_ant = 0;
  if (digitalRead(Btn) == HIGH){
    while(HIGH){
      uint16_t pos = qtr.readLineBlack(sensorValues); //Se puede cambiar a línea blanca con readLineWhite()
      float E = pos - 2500;
      motor_der("adelante",vel-PID(E, E_ant));
      motor_izq("adelante",vel+PID(E,E_ant));
      float E_ant = E;
    }   
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


//PID = kp*e + kd*(e-ea) + ki*(e-ea)
int PID(int E,int E_ant){
  return kp*E+kd*(E-E_ant)+ki*(E-E_ant);
}
