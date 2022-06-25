// by McMax
// ELS - Electronic Lead Screw
// Firmware V1.8 - 20/06/2022
// Encoder PULL_UP configured by default

#include <LiquidCrystal.h>    //include la libreria di controllo del display LCD
#include <EEPROM.h>           //include la libreria per il controllo della EEPROM
//#include <YetAnotherPcInt.h>  //include la libreria per l'utilizzo di interrupt su ogni pin (PcInt)
#include <avr/pgmspace.h>     //include la libreria per l'utilizzo della flash come storage (per le stringhe)

#define firmware_version "   firmware v1.8"

// Pin assignement
#define ANALOG_X A0    //ingresso analogico joystick asse X - Sparkfun joystick X
#define ANALOG_Y A1    //ingresso analogico joystick asse Y - Sparkfun joystick Y
#define STEP     9     //uscita segnale "STEP" per motore stepper 
#define DIR      8     //uscita segnale "DIR" per motore stepper
//#define ENCZ     1     //ingresso encoder Z (index) - PCINT17, portD
#define ENCA     2     //ingresso ancoder A - INTERRUPT 0 
#define ENCB     3     //ingresso ancoder B - INTERRUPT 1 
#define ENABLE   7     //uscita segnale "ENABLE" per motore stepper
#define ESC      4     //bottone ESC - Sparkfun D4
#define SEL      5     //bottone SELECT - Sparkfun D5
#define RESET    6     //bottone RESET - Sparkfun D6
#define LCD_RS   A2    //LCD RS pin
#define LCD_RW   A3    //LCD RW pin
#define LCD_EN   A4    //LCD enable pin
#define LCD_D4   10    //LCD D4 pin
#define LCD_D5   11    //LCD D5 pin
#define LCD_D6   12    //LCD D6 pin
#define LCD_D7   13    //LCD D7 pin
// END pin assignment 

#define MaxSteps 800   //dimension of the step array - this can be adjusted to increase the maximum pitch according to the available RAM
                       //this number MUST be lower than the encoder steps per turn

#define PiGreco 3.141592

//Analog Joystick reading tolerance definition
#define LOW_TOL   450   //minimum tolerance for joystick movement LOW (2.2V on joystic pot cursor)
#define HIGH_TOL  573   //minimum tolerance for joystick movement HIGH (2.8V on joystic pot cursor)
#define LOW_1     348   //first LOW pos tolerance for joystick movement (1.7V on joystic pot cursor)
#define LOW_2     246   //second LOW pos tolerance for joystick movement (1.2V on joystic pot cursor)
#define LOW_3     143   //third LOW pos tolerance for joystick movement (0.7V on joystic pot cursor)
#define HIGH_1    675   //first HIGH pos tolerance for joystick movement (3.3V on joystic pot cursor)
#define HIGH_2    777   //second HIGH pos tolerance for joystick movement (3.8V on joystic pot cursor)
#define HIGH_3    880   //third HIGH pos tolerance for joystick movement (4.3V on joystic pot cursor)
//END Analog Joystick reading tolerance definition



// initialize the LCD library with the references of the interface pins
LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7); //inizializza LCD

#define PRESSED LOW               //costante per determinare la pressione dei pulsanti

char buff[21];                    //buffer per memorizzare le stringhe lette dalla flash e visualizzate sul display

boolean mm_min = false;            //variabile che determina la modalita' di avanzamento: true = mm/min; false = cent/giro 

unsigned int av_carro[4];         //array da 4 valori che memorizza di avanzamenti dei rapporti norton per il carro caricandoli dalla EEPROM
unsigned int av_trasv[4];         //array da 4 valori che memorizza di avanzamenti dei rapporti norton per il trasversale caricandoli dalla EEPROM
byte NORTON_gearbox;              //valore che memorizza la posizione corrente della scatola norton (usato come indice per gli array qui sopra)

// variabili relative ad encoder e motore stepper
int one_turn_mandrel_steps;        //numero di step/giro dell'encoder mandrino (x4) (valore caricato da EEPROM)
float screw_pitch;                          //passo della vite madre in mm (valore caricato da EEPROM)

unsigned int one_turn_screw_steps;          //numero di step/giro del motore stepper sulla vite (valore caricato da EEPROM)
float single_step_pitch;                    //valore in mm di movimento del carro ad ogni step della vite madre
                                            //calcolato al termine della funzione LoadFromEEPROM()
float SingleStepFeed;                       //valore in millimetri di movimento del carro ad ogni step della barra
                                            //calcolato nelle funzioni di avanzamento dove richiesto ed in base alla posizione del cambio norton
float distance = 0.00;                      //distanza in mm usata nella funzione FilettaturaToPosition()                                            
// fine variabili relative ad encoder e motore stepper                                            

// mixed variables
long OldSpeedTimer;
long OldPos = 0;
long NewPos = 0;
//end mixed variables

//variabili usate per il calcolo della progressione di filettatura
byte sequenza [MaxSteps];              //array usato per i calcolo della sequenza passi encoder/stepper. la lunghezza di questo array limita il massimo passo eseguibile in filettatura
                                       //max_passo = passo_vite*(800/passi_stepper_giro)
int numero_passi = 0;                  //variabile che memorizza il numero dei passi stepper per ogni giro mandrino (usata in filettatura e avanzamento)
int pointer = 0;                       //puntatore per array "sequenza"
byte thread_type = 0;                  //0 = metric thread; 1 = TPI(inch) thread; 2 = Module (mod) thread
float thread_pitch = 1.00;             //passo impostato per la filettatura in mm (standard 1.00mm)
byte TPI = 20;                         //passo impostato per la filettatura in pollici (standard 20 TPI)
float mod_pitch = 1.00;                //passo impostato per la filettatura a modulo (standard 1.00 mod) 
//fine variabili usate per il calcolo della progressione di filettatura

//variabili relative alla lettura dell'encoder
int steps = 0;                             //passi encoder relativi - variabile usata come appoggio in filettatura e avanzamento
volatile long absolute_encoder_steps = 0;  //passi encoder assoluti
boolean step_flag;                         //Flag per detarminare se il passo encoder è avvenuto (usata nella rountine di interrupt di lettura dell'nencoder) 
int passi_sequenza = 0;
char encoder[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};  //arrray da 16 valori usato per "muovere" i passi encoder nella rountine interrupt di filettatura
boolean sviluppo_filetto = true;      //Flag che detrmina il verso id filettatura: TRUE = DESTRO; FALSE = SINISTRO
//fine variabili relative alla lettura dell'encoder                                     


// Stepper motor variables
boolean CW;                    //TRUE se la rotazione standard della vite e' in senso orario (orario trascina il carro verso il mandrino). variabile letta da EEPROM
boolean CCW;                   //TRUE se la rotazione standard della vite e' in senso anti-orario (anti-orario trascina il carro verso il madnrino) variabile letta da EEPROM
boolean Direction = CW;        //variabile usata per determinare il senso di rotazione momentaneo del motore stepper e incrementare o decrementare i passi
boolean CarroTrasv = true;     //TRUE = avanzamneto sul carro; FALSE = avanzamento sul trasversale
int passi_stepper = 0;
unsigned long Speed;           //memorizza la velocità corrente del motore stepper in giri al minuto - se = 0 lo stepper e' fermo
unsigned long MaxStepperSpeed; //massima velocità di rotazione ammessa per il motore stepper. Variabile letta da EEPROM

volatile unsigned int TOP = 65535;        //valore usato per scrivere il registro ICR1 che determina la frequenza del PWM che regola la velocità di rotazione del motore stepper 
volatile long absolute_steps = 0;         //passi stepper assoluti
unsigned int AccelerationDelay;           //ritardo accelerazione del motore stepper - varibile letta da EEPROM
unsigned int DecelerationDelay;           //ritardo decelerazione del motore stepper - varibile letta da EEPROM
int thread_offset_steps;                  //offset per operazione di filettatura. Indica quanti passi prima dell'inizio del filetto si deve posizionare la vite
int feed = 1;                             //valore corrente avanzamneto usato nella funzione di avanzamneto
// End Stepper motor variables


//******************************************** E N D     D E C L A R A T I O N  **************************************//

void StopTimer1() //pulisce i registri del Timer1 per fermare il PWM e quindi il motore stepper

{
  TCCR1B &= B11111000;  //imposta il prescaler del Timer a 0 - il Timer1 si ferma in questo punto
  TIMSK1 &= B11111101;  //ferma l'interrupt associato al Timer1 
  TCNT1 = 0;            //resetta il valore del Timer1
}

void StartTimer1() //imposta i registri del Timer 1 e fa partire il conteggio - riavvia il motore dopo che  stato fermato
{
  TCNT1 = 0;            //resetta il valore del Timer1
  TIMSK1 |= B0000010;   //imposta l'interrupt associato al Timer1 
  TCCR1B |= B0000011;   //imposta il precaler del Timer1 a clk/64 - il Timer1 parte in questo punto 
}


ISR (TIMER1_COMPA_vect)    // rountine di interrupt del Timer1 - usata per incrementare i passi stepper e tenere traccia della posizone
{
  ICR1 = TOP;
  if (Direction == CW)
    absolute_steps++;
  else
    absolute_steps--;    
}


void StepperON()          //accende il motore stepper
{
  digitalWrite(ENABLE, HIGH);
  delay(10);
  digitalWrite(ENABLE, LOW);
}

void StepperOFF()        //spegne il motore stepper
{
  digitalWrite(ENABLE, HIGH);
}

void ClearPWM()  //Clear the PWM and stops Timer1
{
  TCCR1B = 0;                        // halt Timer by setting clock bits (Prescaler) to 0. This will stop the timer until we get set up
  TCCR1A = 0;                        // Reset the register to avoid misconfiguration issue
  TCNT1 = 0;                         // start counting at 0 (BOTTOM)
  TIMSK1 = 0;                        // Reset the interrupt mask register for Timer1                
   
}

void SetPWM()    //Sets the PWM and starts Timer1
{
  ICR1 = 65535;                        //(65535)set TOP at maximum value in order to get minumun frequency on the motor with prescaler clk/64
  OCR1A = 3;                           // Set OCR1A register to 3 which means that the CLEAR of the OC1A pin is done after 3 timer1 ticks.
                                       // means 12us with clk/64 prescaler
  TCCR1A = _BV(COM1A1) | _BV(WGM11);   //OC1A (pin 9) set for PWM with SET at BOTTOM and CLEAR at MATCH (non-inverted mode)
  TCCR1B = _BV(WGM13) | _BV(WGM12);    //WGM set for Fast PWM with ICR1 used as TOP value

}

void SetOneShot()
{
  ICR1 = 0;                                                 // Set ICR1 register to 0. ICR1 is the TOP of the timer counting which means the timer is stuck to 0
  OCR1A = 65534;                                            // Set OCR1A to 65534 in order to fire a pulse of 2 cycle clock (8us)
  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM11);          //OC1A (pin 9) set for PWM with CLEAR at BOTTOM and SET at MATCH (inverted mode)
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10); //WGM set for Fast PWM with ICR1 used as TOP value. Prescaler set to clk/64 (4uSec cycle)
                                                            // Timer1 start HERE  
}

void FireStep()
{
  TCNT1 = 65533;                // Force the Timer1 value to 65533 in order to hit the MATCH of OCR1A after one clock cycle
}

void StepperMoveToPosition(long absolute_position) //runs the stepper to a specific position without accelerating (this is used for precision positioning only
                                                          //please make sure to call SetOneShot() before using this function
                                                        
{
  if (absolute_steps < absolute_position) digitalWrite(DIR, CCW); else digitalWrite(DIR, CW);
  while (absolute_steps != absolute_position)
  {
    FireStep();
    if (digitalRead(DIR) == CW) absolute_steps--; else absolute_steps++;
    delayMicroseconds(1500);
  }
}

void StepperRunToSpeed(unsigned long Target)    //Runs the stepper to a specific Target speed (in rpm)
                                                //AccelRate is the wait time in usec for acceleration
                                                //Make sure to call SetPWM() before using this function
{
  unsigned long CurrentSpeed;
  
  if (Target > MaxStepperSpeed) Target = MaxStepperSpeed;  
  if (Target == 0)
  {
    for (CurrentSpeed = Speed; CurrentSpeed > 0; CurrentSpeed--)
    {
      TOP = ((1.0/((CurrentSpeed*one_turn_screw_steps)/60))*1000000)/4;
      delayMicroseconds(DecelerationDelay);
    }
    StopTimer1();
  }
  else        //IF target != 0
  {
    if (Speed == 0) StartTimer1();
    if (Target > Speed)
    { 
      if (Speed == 0) Speed = 1; 
      for (CurrentSpeed = Speed; CurrentSpeed <= Target; CurrentSpeed++)
      {
        TOP = ((1.0/((CurrentSpeed*one_turn_screw_steps)/60))*1000000)/4;
        delayMicroseconds(AccelerationDelay);
      }
    }
    else //Taget is < than Speed
    {   
      for (CurrentSpeed = Speed; CurrentSpeed >= Target; CurrentSpeed--)
      {
        TOP = ((1.0/((CurrentSpeed*one_turn_screw_steps)/60))*1000000)/4;
        delayMicroseconds(DecelerationDelay);
      }
    }
  }
  
  Speed = Target;
}                              



void setup() 
{
  /* // DEBUG
  Serial.begin(38400);
  Serial.println("ELS v1.8");
  Serial.println("debug session");
  */ //END DEBUG
  lcd.begin(20, 4);
  lcd.print("Electronic LeadScrew");
  lcd.setCursor(9,1);
  lcd.print("by");
  lcd.setCursor(7,2);
  lcd.print("McMax");
  lcd.setCursor(0,3);
  lcd.print(firmware_version);
  delay(2500);
  //pinMode(ENCZ, INPUT_PULLUP);         //Encoder Z input (index) - software INTERRUPT
  pinMode(ENCA, INPUT_PULLUP);         //Econder A input - INTERRUPT
  pinMode(ENCB, INPUT_PULLUP);         //Encoder B input - INTERRUPT
  pinMode(ESC, INPUT_PULLUP);   //ESC BUTTON (D4 of the sparkfun Joystick shield)
  pinMode(SEL, INPUT_PULLUP);   //SELECTION BUTTON (D5 of the sparkfun Joystick shield)
  pinMode(RESET, INPUT_PULLUP); //RESET BUTTON (D6 of the sparkfun Joystick shield)
  pinMode(STEP, OUTPUT);        //STEP OUTPUT to stepper driver
  pinMode(DIR, OUTPUT);         //DIRection OUTPUT to stepper driver
  pinMode(ENABLE, OUTPUT);      //ENABLE OUTPUT to stepper driver
  LoadFromEEPROM();             //Load the settings stored in the EEPROM and place it in the variables 
  
  absolute_steps = 0;           //initialize the abolute number of leadscrew stepper steps
  absolute_encoder_steps = 0;   //initialize the abolute number of mandrel encoder steps
  Speed = 0;                    //initialize the current stepper speed to 0
}

void loop() 
{
  char scelta;
  while (true)
  {
    scelta = Principale();
    switch (scelta) {
      case 1: lcd.clear(); Impostazioni(); break;
      case 2: lcd.clear(); Avanzamento(); break;
      case 3: lcd.clear(); Filettatura(); break;
      case 4: lcd.clear(); MovimentoLibero(); break;
      case 5: lcd.clear(); MandrelSpeed(); break;
      case 6: lcd.clear(); PosizioneAngolare(); break;
    }  
  }
}
