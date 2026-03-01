
const char PH=20;
const char TR=21;

void setup() {
  // put your setup code here, to run once:
  DDRF=0xFF;
  pinMode(PH,OUTPUT);
  pinMode(TR,OUTPUT);
}


char segment[8]={0x3f,0x06,0x5B,0x4f,0x66,0x6D,0x7D,0x07};
void loop() 
{
   for(int c=0;c<8;c++) 
  {
    PORTF=segment[c];
    delay(2000);
  }

  for(int c=0;c<8;c++)
  {
    PORTF=segment[7-c];
    delay(2000);
  }
  

}


void program1()
{
  digitalWrite(PH,1);
  delay(500);
  digitalWrite(PH,0);
  delay(3000);
  for(int c=0;c<8;c++) 
  {
    PORTF=segment[c];
    delay(2000);
  }
  digitalWrite(TR,1);
  delay(500);
  digitalWrite(TR,0);
  delay(3000);
}

void program2()
{
  digitalWrite(PH,1);
  delay(500);
  digitalWrite(PH,0);
  delay(3000);
  for(int c=8;c>4;c--) 
  {
    PORTF=segment[c];
    delay(2000);
  }
  digitalWrite(TR,1);
  delay(500);
  digitalWrite(TR,0);
  delay(3000);
}

void program3()
{
  digitalWrite(PH,1);
  delay(500);
  digitalWrite(PH,0);
  delay(3000);
  for(int c=4;c<6;c++) 
  {
    PORTF=segment[c];
    delay(2000);
  }
  digitalWrite(TR,1);
  delay(500);
  digitalWrite(TR,0);
  delay(3000);
}
void program4()
{
  digitalWrite(PH,1);
  delay(500);
  digitalWrite(PH,0);
  delay(3000);
  for(int c=6;c<8;c++) 
  {
    PORTF=segment[c];
    delay(2000);
  }
  digitalWrite(TR,1);
  delay(500);
  digitalWrite(TR,0);
  delay(3000);
}

void program5()
{
  digitalWrite(PH,1);
  delay(500);
  digitalWrite(PH,0);
  delay(3000);
  for(int c=8;c<-1;c--) 
  {
    PORTF=segment[c];
    delay(2000);
  }
  digitalWrite(TR,1);
  delay(500);
  digitalWrite(TR,0);
  delay(3000);
  
}
