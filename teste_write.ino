void writeString(String x)
{
  for (int i = 0; i<x.length(); i++)
    Serial.write(x[i]);
  Serial.println();
}

void setup(){
  Serial.begin(9600);
}

void loop(){
  //Serial.write(45); // send a byte with the value 45
   String x = "hello";
   writeString(x)
