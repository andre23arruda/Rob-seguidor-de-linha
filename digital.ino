#define Faixas_Direita 1000000000000 // numero de faixas na direita que devem ser lidas para o robo parar
#define Kp 30
#define Kd 80
#define Ki 15
#define velocidade_inicial 180
#define tempo_calibracao 5000

float erro = 0, P = 0, I = 0, D = 0, PID = 0, p_erro = 0, p_I = 0;
int a[6] = {0, 0, 0, 0, 0,0};
int media[6] = {0, 0, 0, 0, 0,0};
int sensor[6] = {0, 0, 0, 0, 0,0};
int alto[6] = {0, 0, 0, 0, 0,0};
int baixo[6] = {1000, 1000, 1000, 1000, 1000,1000};
int past_sensor5 = 0; // leitura do sensor da faixa direita
int faixas = 0; // contador de faixas direita
int aux = 0; // variavel auxiliar para a condição de parada
unsigned long tempo_atual = 0, tempo_final = 0;

void sensores(void);
void calcula_pid(void);
void controle_motor(void);
void conta_faixas(void);

void setup(){
  pinMode (A5, INPUT);
  pinMode (A4, INPUT);
  pinMode (A3, INPUT);
  pinMode (A2, INPUT);
  pinMode (A1, INPUT);
  pinMode (A0, INPUT);

  pinMode(5, OUTPUT); //PWM Motor Direito
  pinMode(6, OUTPUT); //PWM Motor Esquerdo

  Serial.begin(9600); 
}

void loop(){
  tempo_atual = millis();
  sensores();
  calcula_pid();
  conta_faixas();
  controle_motor();
}

void sensores(){ // função que calcula a media dos valores de cada sensor
// Lendo sensores
  sensor[0] = digitalRead(A5);
  sensor[1] = digitalRead(A4);
  sensor[2] = digitalRead(A3);
  sensor[3] = digitalRead(A2);
  sensor[4] = digitalRead(A1);
  sensor[5] = digitalRead(A0); //sensor direita
  
  if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0))
    erro = 4;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    erro = 3;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 1))
    erro = 2;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))
    erro = 1;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1))
    erro = 0;
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1))
    erro = -1;
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))
    erro = -2;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))
    erro = -3;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))
    erro = -4;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1)){
    if(erro <= -4) erro = -5;
    else if(erro = 4) erro = 5;
  }
}

void calcula_pid(){ // função para calcular o PID
  P = erro;
  I = I + p_I;
  D = erro - p_erro;

  PID = (Kp * P) + (Ki * I) + (Kd * D);

  p_I = I;
  p_erro = erro;
}

void controle_motor(){ // função de controle de velocidade dos motores
  // Calculando a velocidade de acordo com o PID
  int motor_esquerdo = velocidade_inicial - PID;
  int motor_direito = velocidade_inicial + PID;

  // Nao pode ultrapassar o maximo e o minimo do PWM
  motor_esquerdo = constrain(motor_esquerdo, 0, 255);
  motor_direito = constrain(motor_direito, 0, 255);
 
  if (faixas<Faixas_Direita && tempo_atual>=tempo_calibracao){
    analogWrite(6, motor_esquerdo); // Velocidade do motor esquerdo
    analogWrite(5, motor_direito); // Velocidade do motor direito
  } 
  if (faixas == Faixas_Direita && aux == 0){
    tempo_final = tempo_atual + 500;
    aux++;
  }
  if (tempo_atual >= tempo_final && aux > 0){
    //Serial.println("PARADO");
    analogWrite(6, 0); // Velocidade do motor esquerdo
    analogWrite(5, 0); //Velocidade do motor direito
  }
  else { // Continua o controle do motor nesses 500 ms
    analogWrite(6, motor_esquerdo); 
    analogWrite(5, motor_direito); 
  }
}

void conta_faixas(){ // função de contar faixas da direita
  if (past_sensor5 == 0 && sensor[5] == 1 && tempo_atual >= tempo_calibracao){
    faixas++;
  }
  past_sensor5 = sensor[5];
}
