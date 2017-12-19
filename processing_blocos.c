int rect0X, rect0Y;      // posicao do quadrado 0
int rect1X, rect1Y;      // posicao do quadrado 1
int rect2X, rect2Y;      // posicao do quadrado 2
int rect3X, rect3Y;      // posicao do quadrado 3
int rect4X, rect4Y;      // posicao do quadrado 3
int rect5X, rect5Y;      // posicao do quadrado 3

int  rect1Color = color(0);
int  rect2Color = color(0);
int  rect3Color = color(0);
int  rect4Color = color(0);
int  rect5Color = color(0);
int  rect6Color = color(0);



int rectSize = 90;     // tamanho do quadrado
color baseColor;
color rectHighlight;
color currentColor;
boolean rectOver = false; // comeca como nao pressionado

int cont = -1;

void setup() {
  
  size(810, 500);
  
  baseColor = color(102);
  currentColor = baseColor;
  
  rect0X = 10;
  rect0Y = height/2-rectSize/2;
  
  rect1X = 150;
  rect1Y = height/2-rectSize/2;  
  
  rect2X = 290;
  rect2Y = height/2-rectSize/2;  

  rect3X = 430;
  rect3Y = height/2-rectSize/2;
  
  rect4X = 570;
  rect4Y = height/2-rectSize/2;
  
  rect5X = 710;
  rect5Y = height/2-rectSize/2;  
}

void draw() {
  background(currentColor);
  stroke(255);
  fill(rect1Color);
  rect(rect0X, rect0Y, rectSize, rectSize);
  fill(rect2Color);
  rect(rect1X, rect1Y, rectSize, rectSize);
  fill(rect3Color);  
  rect(rect2X, rect2Y, rectSize, rectSize);
  fill(rect4Color);  
  rect(rect3X, rect3Y, rectSize, rectSize);
  fill(rect5Color);  
  rect(rect4X, rect4Y, rectSize, rectSize);
  fill(rect6Color);  
  rect(rect5X, rect5Y, rectSize, rectSize);

}

void mousePressed() { // verificar se o mouse foi pressionado 
  cont++;
  if (cont%6 == 0)
    rect1Color = rect1Color + color(50);
  else if (cont%6 == 1)
    rect2Color = rect2Color + color(50);
  else if (cont%6 == 2)
    rect3Color = rect3Color + color(50);
  else if (cont%6 == 3)
    rect4Color = rect4Color + color(50);  
  else if (cont%6 == 4)
    rect5Color = rect5Color + color(50);
  else if (cont%6 == 5)
    rect6Color = rect6Color + color(50);
}
