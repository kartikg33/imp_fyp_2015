void setup()
{
  pinMode(A5, INPUT);

  Serial.begin(9600);  

}

void loop()
{
  int val = analogRead(A5);   
  //float volt = val* (5.0/1023.0);
  Serial.println(val);            
}


