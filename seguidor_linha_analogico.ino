#define Faixas_Direita 8 // numero de faixas na direita que devem ser lidas para o robo parar
#define Kp 30
#define Kd 80
#define Ki 15
#define velocidade_inicial 120
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
void calcular_media(void);
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
  calcular_media();
  sensores();
  calcula_pid();
  conta_faixas();
  controle_motor();
}

void calcular_media(){ // função que calcula a media dos valores de cada sensor
// Lendo sensores
  a[0] = analogRead(A5);
  a[1] = analogRead(A4);
  a[2] = analogRead(A3);
  a[3] = analogRead(A2);
  a[4] = analogRead(A1);
  a[5] = analogRead(A0); //sensor direita

// Calculo da média de cada sensor
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

void sensores(){ // função que "transforma" a leitura analogica em digital
    sensor[0] = (a[0]<media[0])? 1 : 0;
    sensor[1] = (a[1]<media[1])? 1 : 0;
    sensor[2] = (a[2]<media[2])? 1 : 0;
    sensor[3] = (a[3]<media[3])? 1 : 0;
    sensor[4] = (a[4]<media[4])? 1 : 0;
    sensor[5] = (a[5]<media[5])? 1 : 0;
   
// Se quiser imprimir na tela a leitura dos sensores
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
    erro = 4;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1))
    erro = 3;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0))
    erro = 2;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0))
    erro = 1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    erro = 0;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    erro = -1;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    erro = -2;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    erro = -3;
  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    erro = -4;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0)){
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
  motor_esquerdo = constrain(motor_esquerdo, 0, 220);
  motor_direito = constrain(motor_direito, 0, 220);
 
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
  // Serial.println(faixas);
}
