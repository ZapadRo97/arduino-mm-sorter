#include <Servo.h>
#include <StepperAK.h>

#define S0 7
#define S1 5
#define S2 4
#define S3 3
#define sensorOut 2

int frequencyR =0, frequencyG =0, frequencyB =0;

#define gearratio 64
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8,9,10,11);  

Servo myServo;

bool balancingStep = false;
int circleStep = 0;

int lastColor = 0;
int currentColor = 0;

void setup()
{
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  //stepper init
  myStepper.setSpeed(0.15*gearratio);
  
  myServo.attach(6);
  
  Serial.begin(9600);
}

void readColor()
{
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequencyR = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  //Serial.print("R= ");//printing name
  //Serial.print(frequencyR);//printing RED color frequency
  //Serial.print("  ");
  delay(100);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyG = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  //Serial.print("G= ");//printing name
  //Serial.print(frequencyG);//printing RED color frequency
  //Serial.print("  ");
  delay(100);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequencyB = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  //Serial.print("B= ");//printing name
  //Serial.print(frequencyB);//printing RED color frequency
  //Serial.println("  ");
  delay(100);
}

void loop()
{
  myServo.write(lastColor);
    
  int sumFreqR = 0;
  int sumFreqB = 0;
  int sumFreqG = 0;
  
  for(int i =0; i < 50; i++)
  {
    readColor();
    sumFreqR += frequencyR;
    sumFreqG += frequencyG;
    sumFreqB += frequencyB;
  }
  
  Serial.print("R= ");
  Serial.print(sumFreqR/50);
  Serial.print(" ");
  Serial.print("G= ");
  Serial.print(sumFreqG/50);
  Serial.print(" ");
  Serial.print("B= ");
  Serial.print(sumFreqB/50);
  Serial.print(" ");
  Serial.println(" ");
  
  int avgFreqR = sumFreqR/50;
  int avgFreqG = sumFreqG/50;
  int avgFreqB = sumFreqB/50;
  
  if(avgFreqR > 79 && avgFreqR < 110 && avgFreqG > 139 && avgFreqG < 166 && avgFreqB > 99 && avgFreqB < 121)
  {
    Serial.println("RED");
    lastColor = 90;
  }
  else if(avgFreqR > 124 && avgFreqR < 146 && avgFreqG > 114 && avgFreqG < 136 && avgFreqB > 69 && avgFreqB < 91)
  {
    Serial.println("BLUE");
  }
  else if(avgFreqR > 99 && avgFreqR < 121 && avgFreqG > 99 && avgFreqG < 121 && avgFreqB > 84 && avgFreqB < 111)
  {
    Serial.println("GREEN");
  }
  else if(avgFreqR < 81 && avgFreqG > 79 && avgFreqG < 101 && avgFreqB > 74 && avgFreqB < 101)
  {
    Serial.println("YELLOW");
    lastColor = 50;
  }
  else if(avgFreqR < 101 && avgFreqG > 109 && avgFreqG < 135 && avgFreqB > 84 && avgFreqB < 116)
  {
    Serial.println("ORANGE");
    lastColor = 20;
  }
  else if(avgFreqR > 114 && avgFreqR < 136 && avgFreqG > 124 && avgFreqG < 151 && avgFreqB > 84 && avgFreqB < 111)
  {
    Serial.println("BROWN");
  }

  //these should fix the no exact stepsPerRevolution/6
  //hint: it's not happening
  
  if(balancingStep)
  {
    if(circleStep == 6)
    {
      myStepper.step(stepsPerRevolution/6);
      circleStep = 0;
      Serial.println("FULL ROTATION");
    }
    else
      myStepper.step(stepsPerRevolution/6+1);
  }
  else
  {
    if(circleStep == 6)
    {
      myStepper.step(stepsPerRevolution/6-1);
      circleStep = 0;
      Serial.println("FULL ROTATION");  
    }
    else
      myStepper.step(stepsPerRevolution/6);
  }
    
  circleStep++;
  balancingStep = !balancingStep;
 
  delay(1000); 
}
