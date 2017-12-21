#define Faixas_Direita 8 // Numero de faixas direita que o robo tem que contar para parar. Incluso cruzamento
#define Kp 30 // Contante Kp, ajuste de acordo com seu robo
#define Kd 80 // Contante Kd, ajuste de acordo com seu robo
#define Ki 15 // Contante Ki, ajuste de acordo com seu robo
#define initial_motor_speed 120 // Velocidade inicial do motor, quanto mais baixo mais facil o controle, porém é lento. Mais alto é mais rapido, porem mais rapido
#define tempo_calib 5000 // Tempo de calibração dos sensores

float error = 0, P = 0, I = 0, D = 0, PID_value = 0; // Iniciando o PID
float previous_error = 0, previous_I = 0;
int a[6] = {0, 0, 0, 0, 0,0}; // Leiura analogica dos sensores
int media[6] = {0, 0, 0, 0, 0,0}; // Media da leitura dos sensores
int sensor[6] = {0, 0, 0, 0, 0,0}; // Leitura "digital" dos sensores
int alto[6] = {0, 0, 0, 0, 0,0}; 
int baixo[6] = {1000, 1000, 1000, 1000, 1000,1000};
int past_sensor5=0; // variavel auxiliar para contar faixa direita
int faixas=0; // contador de faixas direita
int contador=0; // variavel auxiliar para condição de parada
unsigned long tempo_atual=0; // Tempo para calibracao e parada
unsigned long tempo_final=0; // Tempo para calibracao e parada

void read_sensor_values(void); // função de leitura dos sensores
void calculate_pid(void); // função para calcular o PID
void motor_control(void); // função para aplicar o PID nos motores
void calcular_media(void); // função para calcular a leitura media dos sensores
void conta_faixas(void); // função para contar faixas da direita

void setup()
{
  pinMode (A5, INPUT); // Entrada analogica do sensor0
  pinMode (A4, INPUT); // Entrada analogica do sensor1
  pinMode (A3, INPUT); // Entrada analogica do sensor2
  pinMode (A2, INPUT); // Entrada analogica do sensor3
  pinMode (A1, INPUT); // Entrada analogica do sensor4
  pinMode (A0, INPUT); // Entrada analogica do sensor da direita

  pinMode(5, OUTPUT); //PWM Motor Direito
  pinMode(6, OUTPUT); //PWM Motor Esquerdo
  
  Serial.begin(9600); //Enable Serial Communications
}

void loop() // Todas as funções rodam dentro do loop
{
  tempo_atual=millis();
  calcular_media();
  read_sensor_values();
  calculate_pid();
  conta_faixas();
  motor_control();
  Serial.println(tempo_atual-tempo_final);
}

void calcular_media()
{
  a[0] = analogRead(A5);
  a[1] = analogRead(A4);
  a[2] = analogRead(A3);
  a[3] = analogRead(A2);
  a[4] = analogRead(A1);
  a[5] = analogRead(A0); //sensor direita


  if (a[0] > alto[0])
      alto[0] = a[0];
    if(a[0] < baixo[0])
      baixo[0] = a[0];
    media[0] = (alto[0]+baixo[0])/2;
    
    if (a[1] > alto[1])
      alto[1] = a[1];
    if(a[1] < baixo[1])
      baixo[1] = a[1];
    media[1] = (alto[1]+baixo[1])/2;
  
   if (a[2] > alto[2])
      alto[2] = a[2];
    if(a[2] < baixo[2])
      baixo[2] = a[2];
    media[2] = (alto[2]+baixo[2])/2;
    
    if (a[3] > alto[3])
      alto[3] = a[3];
    if(a[3] < baixo[3])
      baixo[3] = a[3];
    media[3] = (alto[3]+baixo[3])/2;
    
    if (a[4] > alto[4])
      alto[4] = a[4];
    if(a[4] < baixo[4])
      baixo[4] = a[4];
    media[4] = (alto[4]+baixo[4])/2;

      if (a[5] > alto[5])
      alto[5] = a[5];
    if(a[5] < baixo[5])
      baixo[5] = a[5];
    media[5] = (alto[5]+baixo[5])/2;
}

void read_sensor_values()
{
    sensor[0] = (a[0]<media[0])? 1 : 0;
    sensor[1] = (a[1]<media[1])? 1 : 0;
    sensor[2] = (a[2]<media[2])? 1 : 0;
    sensor[3] = (a[3]<media[3])? 1 : 0;
    sensor[4] = (a[4]<media[4])? 1 : 0;
    sensor[5] = (a[5]<media[5])? 1 : 0;
   

// Se quiser verificar a leitura dos sensores
/*Serial.print(sensor[0]);
Serial.print("\t");
Serial.print(sensor[1]);
Serial.print("\t");
Serial.print(sensor[2]);
Serial.print("\t");
Serial.print(sensor[3]);
Serial.print("\t");
Serial.print(sensor[4]);
Serial.print("\t");
Serial.println(sensor[5]);*/


  
  if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))
    error = 4;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1))
    error = 3;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0))
    error = 2;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0))
    error = 1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 0;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    error = -1;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = -2;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = -3;
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = -4;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)){
    if(error<=-4) error = -5;
    else if(error>=4) error = 5;
  }
}

void calculate_pid()
{
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
}

void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 220);
  right_motor_speed = constrain(right_motor_speed, 0, 220);
  
  if (faixas<Faixas_Direita && tempo_atual>=tempo_calib)
  {
    analogWrite(6, left_motor_speed); //Left Motor Speed
    analogWrite(5, right_motor_speed); //Right Motor Speed
  } 
  else if (tempo_atual>=tempo_calib)
  {
    if (faixas== Faixas_Direita && contador==0){
      tempo_final=tempo_atual+500;
      contador++;
    }
    if (tempo_atual>=tempo_final && contador>0)
    {
      Serial.println("PARADO");
      analogWrite(6, 0); //Left Motor Speed
      analogWrite(5, 0); //Right Motor Speed
    }
  }
}

void conta_faixas()
{
  if (past_sensor5==0 && sensor[5]==1 && tempo_atual>=tempo_calib){
    faixas=faixas+1;
  }
  past_sensor5=sensor[5];
  Serial.println(faixas);
}
