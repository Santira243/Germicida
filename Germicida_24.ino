/*

 PIN 2-> Entrada 1 Sensor movimiento
 PIN 3 -> Entrada 2 Sensor movimiento

 PIN 4-> SALIDA RELEE
 Pin 5 -> SALIDA LED
 Pin 12 -> SALIDA LED_RADIACION

 Pin 6 -> SALIDA BUZZER

 
 PIN 10 -> Entrada Boton Inicio CICLO
 
*/
#define MOV1 2
#define MOV2 3
//#define RELEE 4
#define LED 5
#define LED_RAD 12
#define BUZZER 6
#define BTINIT 10

#define TIEMPOOFF 15000  //al detectar a alguien
#define TIEMPO_PROC 600000 //tiempo total 20min -> 1200000

#define TIEMPO_FINAL 10000 //tiempo total 30min
unsigned int TIEMPOOFF_init = 15000; //al iniciar

// variables will change:
volatile int detec_mov1 = 0;         // variable for reading the sensor
volatile int detec_mov2 = 0;         // variable for reading the sensor
boolean parar = false;
boolean btninit = false;
boolean proceso = false;
boolean comienza = false;
boolean finalizo = false;

unsigned long tiempo;
unsigned long tiempo_2;
unsigned long tiempo_3;
unsigned long tiempo_4; //Comienza el ciclo 

unsigned long tiempo_final; //Comienza el ciclo 

unsigned long tiempo_proceso; //Comienza el ciclo 
unsigned long tiempo_perdido; //por culpa de movimientos

int btncount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(LED_RAD, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  //pinMode(RELEE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
 
  pinMode(BTINIT, INPUT_PULLUP);
  
  pinMode(MOV1, INPUT);
  //pinMode(MOV2, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(2), pin_ISR, RISING); 
  //attachInterrupt(digitalPinToInterrupt(3), pin_ISR, RISING); 
  
  btncount=0;
  tiempo_perdido = 0;
}

// the loop function runs over and over again forever
void loop() {
  tiempo = millis();
  btninit = digitalRead(BTINIT);
  
  if(!btninit)
  {
       delay(300); 
       btncount++;
  }

  if (btncount > 4)
  {
    tiempo_4 = millis();
    btncount=0;
    comienza = true;
    tiempo = millis();
  }
  
  if ((comienza) && (!proceso))
  { 
    iniciar();
  }


  if (finalizo)
  { 
    if ((tiempo - tiempo_final) < TIEMPO_FINAL)
     {
      beep_3();
     }  
    else
     { 
      finalizo = false; 
      analogWrite(BUZZER, 0); 
     }
  }

  tiempo = millis();
  
  if(proceso)
  {
        
        if (!parar)
            {
           // Serial.println("Esterilizando");  
            analogWrite(BUZZER, 0);        
            digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
            digitalWrite(LED_RAD, HIGH);   // turn the LED on (HIGH is the voltage level)
            //digitalWrite(RELEE, HIGH);   // turn the LED on (HIGH is the voltage level
            Serial.write('6');
            }     
        else
            {
            //Serial.println("Paro de esterilizar");
            beep_1();
            digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
           //digitalWrite(RELEE, LOW);    // turn the LED off by making the voltage LOW
            Serial.write('3');
            digitalWrite(LED_RAD, LOW);
             
            if((tiempo - tiempo_2) > TIEMPOOFF)
              {
                  tiempo_perdido += TIEMPOOFF;
                  parar = false;
                  analogWrite(BUZZER, 0);
               }
            }
         if((tiempo - tiempo_proceso) > (TIEMPO_PROC + tiempo_perdido))
         {
//          Serial.print("FINAL:");
//          Serial.println(tiempo - tiempo_proceso);
//          Serial.print(TIEMPO_PROC + tiempo_perdido);
          analogWrite(BUZZER, 0);
          digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
          //digitalWrite(RELEE, LOW);    // turn the LED off by making the voltage LOW
          Serial.write('3');
          digitalWrite(LED_RAD, LOW);
          comienza = false;
          proceso = false;
          tiempo_perdido = 0;
          tiempo_final = millis();
          finalizo = true;
         }
  }
}

void iniciar() 
{
 if((tiempo - tiempo_4) < floor(TIEMPOOFF_init/3))
 { 
  beep();
  digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
 }
 
 if(((tiempo - tiempo_4) > floor(TIEMPOOFF_init/3)) &&  ((tiempo - tiempo_4) < floor(2*TIEMPOOFF_init/3) ))
 {
    beep_1();
    digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
 }

 if((tiempo - tiempo_4) > floor(2*TIEMPOOFF_init/3) )
 { 
  beep_2();
  digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
 }
 
 if((tiempo - tiempo_4) > TIEMPOOFF_init)
  {
    tiempo_proceso = millis();
    digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
    proceso = true;
    comienza = false;
  }
}


void beep()
{
digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
if (abs(tiempo - tiempo_3) < 500)
    {
    analogWrite(BUZZER, 200);
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
if(abs(tiempo - tiempo_3) > 500)
    {
     analogWrite(BUZZER, 0);
     digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
    }

if(abs(tiempo - tiempo_3) > 1000)
    {
    tiempo_3 = millis();
    }
}

void beep_1()
{
 digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
 if (abs(tiempo - tiempo_3) < 300)
    {
    analogWrite(BUZZER, 200);
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
if(abs(tiempo - tiempo_3) > 300)
    {
     analogWrite(BUZZER, 0);
     digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
   }

if(abs(tiempo - tiempo_3) > 600)
    {
    tiempo_3 = millis();
    }
}

void beep_2()
{
 digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
 if (abs(tiempo - tiempo_3) < 100)
    {
    analogWrite(BUZZER, 200); 
    digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
    }
if(abs(tiempo - tiempo_3) > 100)
    {
     analogWrite(BUZZER, 0);
     digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    }

if(abs(tiempo - tiempo_3) > 200)
    {
    tiempo_3 = millis();
    }
}

void beep_3()
{
 digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level
 if (abs(tiempo - tiempo_3) < 200)
    {
    analogWrite(BUZZER, 200); 
    digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
    }
if(abs(tiempo - tiempo_3) > 500)
    {
     analogWrite(BUZZER, 0);
     digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    }

if(abs(tiempo - tiempo_3) > 700)
    {
    tiempo_3 = millis();
    }
}

void pin_ISR() 
{
  parar = true;
  tiempo_2 = millis();
  tiempo_3 = millis();
}

void serialEvent() {
  char inChar;
  if (Serial.available()) {
    // get the new byte:
    inChar = (char)Serial.read();
    // add it to the inputString:
    }
  if(inChar == 'A')
  {
   parar = true;
   tiempo_2 = millis();
   tiempo_3 = millis();
   }
     if(inChar == 'P')
  {
     if ((!comienza) && (!proceso))
      { 
        iniciar();
      }
   }

}
