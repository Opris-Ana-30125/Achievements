
#include <Servo.h>
Servo servoMotor;
int n=3;
int pingPin[] = {A0,A1,A2};
int pinLed=13;
int pinResistor=A3;
int pinLuminaEXTERIORFabrica=3;
double valueNight;
int Alarm=13;

struct UltraSonic{
  
 long duration;
 long inches;
 long cm;
  
};

struct UltraSonic machine[3];
  
void setup() {
 
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  pinMode(pinResistor, INPUT);
  pinMode(Alarm, OUTPUT);
  servoMotor.attach(2);
  afiseazaOptiuni();
}

void loop() {
 if (Serial.available()) {


    String com = Serial.readString();
    char comanda = com.charAt(0)&0xDF;   //se forteaza litere mari NOT 20H
    
    
    switch (comanda) {
     case 'A':
        afiseazaOptiuni();
        break;
 case 'N' :       //stingere  lumini EXTERIOR fabrica 
        {
         digitalWrite(pinLuminaEXTERIORFabrica, LOW);
         
         Serial.println("Stingere lumina din fabrica");
        }
        break;
       case 'D' :       //aprindere lumini EXTERIOR fabrica 
        {
         digitalWrite(pinLuminaEXTERIORFabrica, HIGH);
         
         Serial.println("aprindere lumina din fabrica");
        }
        break;
case 'T' :    //actionare ALARMA
        {
         
          Serial.print( "Este nevoie de administrator pentru a opri alarma\n");
          Serial.print("Motoarele care activeaza protectia pentru usi si geamuri au fost pornite\n");
  Serial.println(); 
  for(int i=0;i<n;i++){
 

  
               
  pinMode(pingPin[i], OUTPUT);
  digitalWrite(pingPin[i], LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin[i], HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin[i], LOW);


  pinMode(pingPin[i], INPUT);
  machine[i].duration = pulseIn(pingPin[i], HIGH);

  machine[i].inches = microsecondsToInches(machine[i].duration);
  machine[i].cm = microsecondsToCentimeters(machine[i].duration);

     Serial.print("Valoarea intregistrata senzorul de miscare ");
       Serial.print(i);
        Serial.print("  cu ultrasunet");
      
  Serial.print(":  ");  
  Serial.print(machine[i].inches);
  Serial.print("in, ");
  Serial.print(machine[i].cm);
  Serial.print("cm");
  Serial.println();
  
    
  valueNight=analogRead(pinResistor) * (5.0 / 1023.0);
     servoMotor.write(0);
  if(machine[i].cm<300 && machine[i].cm>200 && valueNight>3.4)
  { digitalWrite(pinLed,HIGH);
    servoMotor.write(40);
    delayMicroseconds(500);
    digitalWrite(pinLed,LOW);
    delay(100);
    tone(Alarm,200,100);
  }
    else
      digitalWrite(pinLed,LOW);
    
    

  
  }
        tone(Alarm,5000000,5000000000000);  }
break;
 case 'O' :       //actionare lumini EXTERIOR
        {
         digitalWrite(pinLuminaEXTERIORFabrica, HIGH);
          digitalWrite(pinLed, LOW);
           digitalWrite(Alarm, LOW);
           servoMotor.write(0);
         Serial.println("Administratorul va opri alarma si va aprinde lumina de afara .");
          noTone(Alarm); 
        }
      break;
    }}}
long microsecondsToInches(long microseconds) {

  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  
  return microseconds / 29 / 2;
}
void afiseazaOptiuni() {
  Serial.write("Comenzi disponibile:\n");
  Serial.write("A - afisare comenzi disponibile: \n");
  Serial.write("N - stingere lumini EXTERIOR fabrica  \n");
  Serial.write("T -testare sistem alarma( un motor va deplasa toate  usile de siguranta,altul grilajul ferestrelor si fotorezistorul va actiona alarma si lumina )\n");
  Serial.write("D - Aprindere lumini EXTERIOR fabrica \n");
  Serial.write("O - Oprire sistem de siguranta de catre administror\n");
  Serial.write("\n");
 
}

