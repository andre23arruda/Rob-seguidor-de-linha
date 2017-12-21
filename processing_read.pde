import processing.serial.*; // importando serial

Serial myPort;  // porta serial

int i; // variavel auxiliar para loop de lei
char z; // variavel auxiliar que recebe char

void setup() 
{
  printArray(Serial.list()); // printando todas as portas disponiveis
  myPort = new Serial(this, Serial.list()[4], 9600); // acessando porta serial do arduino
}

void draw() 
{
  while (myPort.available() > 0) 
  {
    String x = ""; // iniciando string sem nada
    for (i = 0; i < 6; i++) // loop para percorrer a leitura de todos os sensores 
    {
      z = myPort.readChar(); // lendo char enviado pelo arduino
      if (z == '0') // aqui eh um teste porco para ver se da certo a comparaçao
        print("huehue");
      x += z; // montando string
    }
    println(x);
  }
}
