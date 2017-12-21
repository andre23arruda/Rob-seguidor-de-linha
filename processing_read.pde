import processing.serial.*;

Serial myPort;  // The serial port:

int i;
char z;

void setup() {
  // List all the available serial ports:
  printArray(Serial.list());
  // O//pen the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[4], 9600);

}

void draw() {
  while (myPort.available() > 0) {
    String x = "";
    for (i = 0; i < 4; i++) {
      z = myPort.readChar();
      if (z == '0')
        print("huehue");
      x += z;
    }
    println(x);
  }
}
