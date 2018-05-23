
#include <DDS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int val1;
float val2;
#define sw 12
const int W_CLK = 2;
const int FQ_UD = 3; 
const int DATA = 4;
const int RESET = 5;
int statesignal=0;
int freq = 1;
int freqx10 = 4;
double freqn = 1000;
byte GA = 6;
byte GB = 7;
byte GSW = 8;
int aLastState, aState, bState, counter = 0;
float temp;
  // Instantiate the DDS...
  DDS dds(W_CLK, FQ_UD, DATA, RESET);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  lcd.begin();
  Serial.begin(9600);
  pinMode(sw,INPUT_PULLUP);
      pinMode(GA, INPUT_PULLUP);
  pinMode(GB, INPUT_PULLUP);
  pinMode(GSW, INPUT_PULLUP);
  dds.init();  
  
  dds.trim(125000000); 
  aLastState = digitalRead(GA);
  dds.setFrequency(1000);  
  analogWrite(A3,0);
  lcd.print("Sine");
  delay(750);
}
void(* resetFunc) (void) = 0;
void loop() {
  if(!digitalRead(sw))
  {
    Serial.println(digitalRead(sw));
    while(!digitalRead(sw));
    if(statesignal==0)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Square");
      analogWrite(A3,255);
      statesignal =1;  
      delay(750);
    }
    else if(statesignal==1)
    {
      analogWrite(A3,0);
      statesignal =0;  
      resetFunc();
    }
    
  }
  aState = digitalRead(GA);
  bState = digitalRead(GB);
  lcd.setCursor(0, 0);
  lcd.print("Freq:");

  if (aState != aLastState && aState == 0)
  {
    if (bState != aState)

    { 
      freq++;
      counter++;
    }
    else {
      freq--;
      counter--;
    }
    Serial.println(counter);
    freqcap();
    freqn = 1;
    for(int i = 1;i<freqx10;i++){
      freqn *= 10; 
    }
    freqn *= freq;
    dds.setFrequency(freqn);
    delay(200);
  }
  if(freqn<100000.0d)
  {
    
    lcd.print(freqn);
    lcd.setCursor(13,0);
    lcd.print(" Hz");
    }else
    if(freqn<1000000.0d)
  {
    lcd.print(freqn/1000);
    lcd.setCursor(13,0);
    lcd.print("KHz");
    }else
  {
    lcd.print(freqn/1000000.0d);
    lcd.setCursor(13,0);
    lcd.print("MHz");
    }

  lcd.setCursor(0,1);
  float temp;
  val1=analogRead(1);
  temp=val1/4.092;
  val1=(int)temp;//
  val2=((val1%100)/10.0);
  lcd.print("Voltage:");
  lcd.print(val2);
  lcd.print("V");
  //delay(350);
  lcd.setCursor(0,0);
  aLastState = aState;
}

void freqcap(){
      if(freq<1){
      freqx10--;
      freq = 9;
    }
    if(freq > 9){
      freqx10++;
      freq = 1;
    }
    if(freqx10 >= 8){
      if(freq > 6){
      freqx10 = 0;
      freq = 1;
      }
    }  
    if(freqx10<0){
      freqx10 = 8;
      freq = 6;
    }
}
