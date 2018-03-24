#include <Servo.h>
#include <StepperAK.h>

#define S0 7
#define S1 5
#define S2 4
#define S3 3
#define sensorOut 2
#define SERVO_PIN 6

int frequencyR =0, frequencyG =0, frequencyB =0;

//measured values
//may depend on ambient light conditions
enum redValues {redMinR = 79, redMaxR = 110, redMinG = 139, redMaxG = 166, redMinB = 99, redMaxB = 121};
enum orangeValues {orangeMaxR = 101, orangeMinG = 109, orangeMaxG = 135, orangeMinB = 84, orangeMaxB = 116};
enum yellowValues {yellowMaxR = 81, yellowMinG = 79, yellowMaxG = 101, yellowMinB = 74, yellowMaxB = 101};

//just for refference
//these are not tested
enum blueValues {blueMinR = 124, blueMaxR = 146, blueMinG = 114, blueMaxG = 136, blueMinB = 69, blueMaxB = 91};
enum greenValues {greenMinR = 99, greenMaxR = 121, greenMinG = 99, greenMaxG = 121, greenMinB = 84, greenMaxB = 111};
enum brownValues {brownMinR = 114, brownMaxR = 136, brownMinG = 124, brownMaxG = 151, brownMinB = 84, brownMaxB = 111};

//brown, green and blue are just placeholders
enum servoValues {servoRed = 90, servoYellow = 50, servoOrange = 20, servoBrown = 0, servoGreen = 1, servoBlue = 2};

#define SMALL_CIRCLES 6
#define NUM_READS 50
#define gearratio 64
const int stepsPerRevolution = 2048;
const int stepperSpeed = 0.15;
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
  myStepper.setSpeed(stepperSpeed*gearratio);
  
  myServo.attach(SERVO_PIN);
  
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
  
  for(int i =0; i < NUM_READS; i++)
  {
    readColor();
    sumFreqR += frequencyR;
    sumFreqG += frequencyG;
    sumFreqB += frequencyB;
  }
  
  Serial.print("R= ");
  Serial.print(sumFreqR/NUM_READS);
  Serial.print(" ");
  Serial.print("G= ");
  Serial.print(sumFreqG/NUM_READS);
  Serial.print(" ");
  Serial.print("B= ");
  Serial.print(sumFreqB/NUM_READS);
  Serial.print(" ");
  Serial.println(" ");
  
  int avgFreqR = sumFreqR/NUM_READS;
  int avgFreqG = sumFreqG/NUM_READS;
  int avgFreqB = sumFreqB/NUM_READS;
  
  if(avgFreqR > redMinR && avgFreqR < redMaxR && avgFreqG > redMinG && avgFreqG < redMaxG && avgFreqB > redMinB && avgFreqB < redMaxB)
  {
    Serial.println("RED");
    lastColor = servoRed;
  }
  else if(avgFreqR > blueMinR && avgFreqR < blueMaxR && avgFreqG > blueMinG && avgFreqG < blueMaxG && avgFreqB > blueMinB && avgFreqB < blueMaxB)
  {
    Serial.println("BLUE");
    lastColor = servoBlue;
  }
  else if(avgFreqR > greenMinR && avgFreqR < greenMaxR && avgFreqG > greenMinG && avgFreqG < greenMaxG && avgFreqB > greenMinB && avgFreqB < greenMaxB)
  {
    Serial.println("GREEN");
    lastColor = servoGreen;
  }
  else if(avgFreqR < yellowMaxR && avgFreqG > yellowMinG && avgFreqG < yellowMaxG && avgFreqB > yellowMinB && avgFreqB < yellowMaxB)
  {
    Serial.println("YELLOW");
    lastColor = servoYellow;
  }
  else if(avgFreqR < orangeMaxR && avgFreqG > orangeMinG && avgFreqG < orangeMaxG && avgFreqB > orangeMinB && avgFreqB < orangeMaxB)
  {
    Serial.println("ORANGE");
    lastColor = servoOrange;
  }
  else if(avgFreqR > brownMinR && avgFreqR < brownMaxR && avgFreqG > brownMinG && avgFreqG < brownMaxG && avgFreqB > brownMinB && avgFreqB < brownMaxB)
  {
    Serial.println("BROWN");
    lastColor = servoBrown;
  }

  //these should fix the no exact stepsPerRevolution/6
  //hint: it's not happening
  
  if(balancingStep)
  {
    if(circleStep == SMALL_CIRCLES)
    {
      myStepper.step(stepsPerRevolution/SMALL_CIRCLES);
      circleStep = 0;
      Serial.println("FULL ROTATION");
    }
    else
      myStepper.step(stepsPerRevolution/SMALL_CIRCLES+1);
  }
  else
  {
    if(circleStep == SMALL_CIRCLES)
    {
      myStepper.step(stepsPerRevolution/SMALL_CIRCLES-1);
      circleStep = 0;
      Serial.println("FULL ROTATION");  
    }
    else
      myStepper.step(stepsPerRevolution/SMALL_CIRCLES);
  }
    
  circleStep++;
  balancingStep = !balancingStep;
 
  delay(1000); 
}
