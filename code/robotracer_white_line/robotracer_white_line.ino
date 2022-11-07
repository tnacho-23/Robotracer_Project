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
int TCR1 = 11; //Sensor lateral Izquierdo
int TCR2 = 12; //Sensor lateral Derecho

//Variables sensores laterales
int geo = 0;
int lgeo = 0;
int end_line = 0; //variable que indica stop


//Variables Buzzer
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
float E = 0;
float E_ant = 0;
float E_2ant = 0;
double kp = 0.115;
double kd = 0.085;
double ki = 0.045;
double vel =120;




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
  pinMode(TCR1,INPUT);
  pinMode(TCR2,INPUT);
  digitalWrite(StandBy,HIGH);

  //Buzzer Start

  //Sensors Config
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A1, A2, A3, A4, A5, A6}, SensorCount);
  qtr.setEmitterPin(StandBy);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  tone(Buzzer, Do1, cuarto);
  delay(cuarto*pausa);
  digitalWrite(LED_BUILTIN, HIGH);
   
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
  tone(Buzzer, Do1, cuarto);
  delay(cuarto*pausa);
  tone(Buzzer, Do1, cuarto);
  delay(cuarto*pausa);
  
  //Motores Apagados
  motor_der("apagado",0);
  motor_izq("apagado",0);
}

//Main Loop
void loop(){
  int start = LOW;
  if (digitalRead(Btn) == HIGH){
    delay(1000);
    start = true;
    while(start){
      uint16_t pos = qtr.readLineWhite(sensorValues); //Se puede cambiar a línea blanca con readLineWhite()
      E = pos - 2500;
      motor_der("adelante",vel-PID(E, E_ant,E_2ant));
      motor_izq("adelante",vel+PID(E,E_ant,E_2ant));
      E_2ant = E_ant;
      E_ant = E;
      hito();
      if (end_line ==2){
        motor_der("apagado",0);
        motor_izq("apagado",0);
        start = false;
      }
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
  if(movimiento == "atras"){
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


//PID = kp*e + kd*(e-ea) + ki*(e-e2a)
int PID(int E,int E_ant,int E_2ant){
  return kp*E+kd*(E-E_ant)+ki*(E-E_2ant);

}

  

//Hitos laterales
//geo = 0: sin hitos
// geo = 1: hito solo a la izquierda
// geo = 2: hito solo a la derecha
// geo = 3: hito a la izquierda y derecha
// Los sensores entregan LOW sobre negro y HIGH sobre blanco
void hito(){
  int HIZ = digitalRead(TCR1);
  int HDE = digitalRead(TCR2);
  //Serial.print(HIZ);
  if (HIZ == LOW and HDE == LOW){ //NO HAY HITOS
     geo = 0;
  }
  else if (HIZ == HIGH and HDE == LOW){ //HITO IZQUIERDO
    geo = 1;
  }
  else if (HIZ == LOW and HDE == HIGH){ //HITO DERECHO
    geo = 2;
  }
  else if (HIZ == HIGH and HDE == HIGH){ //HITO AMBOS LADOS
    geo = 3;
  }
  if (geo != lgeo){
    if (geo == 1 and lgeo == 0){ //HITO IZQUIERDO
      Lhito();
    }
    else if (geo == 2 and lgeo == 0){ //HITO DERECHO
      Rhito();
    }
    else if (geo == 3 and lgeo == 0){ //HITO AMBOS LADOS
      Cruce();
    }
  }
  lgeo = geo;
}

void Lhito(){
  tone(Buzzer, Do1, cuarto);
}

void Rhito(){
  tone(Buzzer, Do1, cuarto);
  end_line = end_line + 1;
}

void Cruce(){
  tone(Buzzer, Do, cuarto);
  tone(Buzzer, Do, cuarto);
}
