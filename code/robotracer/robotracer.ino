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


//Setup
void setup() {
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
  Serial.begin(9600);
}

//Main Loop
void loop() {
  motor_der("atrás", 100);
  delay(3000);
  motor_der("atrás", 255);
  delay(1000);
  motor_der("atrás",100);
  delay(1000);
  motor_der("adelante",100);
  delay(500);
  motor_der("adelante",255);
  delay(3000);
  motor_der("apagado",0);
  delay(2000);
  

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
