/* EEPROM factory defaults and parameters to be stored
 * 
 * PARAMETER               |TYPE          |SIZE  |STARTING LOCATION |FACTORY DEFAULT
 * one_turn_mandrel_steps  |unsigned int  |2     |0                 |1000
 * screw_pitch             |float         |4     |2                 |3.175
 * one_turn_screw_steps    |unsigned int  |2     |6                 |400
 * AccelerationDelay       |unsigned int  |2     |8                 |500
 * DecelerationDelay       |unsigned int  |2     |10                |500
 * av_carro_1              |unsigned int  |2     |12                |50
 * av_trasv_1              |unsigned int  |2     |14                |7
 * av_carro_2              |unsigned int  |2     |16                |30
 * av_trasv_2              |unsigned int  |2     |18                |5
 * av_carro_3              |unsigned int  |2     |20                |20
 * av_trasv_3              |unsigned int  |2     |22                |3
 * av_carro_4              |unsigned int  |2     |24                |10
 * av_trasv_4              |unsigned int  |2     |26                |2
 * CW                      |boolean       |1     |28                |HIGH
 * CCW                     |boolean       |1     |29                |LOW
 * offset_filetto          |unsigned int  |2     |30                |400
 * NORTON                  |byte          |1     |32                |1
 * MaxStepperSpeed         |unsigned long |4     |33                |500 
 * FREE                    |              |      |37                | 
*/
#define one_turn_mandrel_steps_default    1000
#define screw_pitch_default               3.175
#define one_turn_screw_steps_default      400
#define AccelerationDelay_default         800
#define DecelerationDelay_default         800
#define CW_default                        LOW
#define CCW_default                       HIGH
#define av_carro_1_default                54  //avanzamento carro in centesimi/giro per rapporto semiNorton 1
#define av_trasv_1_default                7   //avanzamento trasversale in centesimi/giro per rapporto semiNorton 1
#define av_carro_2_default                30  //avanzamento carro in centesimi/giro per rapporto semiNorton 2
#define av_trasv_2_default                5   //avanzamento trasversale in centesimi/giro per rapporto semiNorton 2
#define av_carro_3_default                20  //avanzamento carro in centesimi/giro per rapporto semiNorton 3
#define av_trasv_3_default                3   //avanzamento trasversale in centesimi/giro per rapporto semiNorton 3
#define av_carro_4_default                10  //avanzamento carro in centesimi/giro per rapporto semiNorton 4
#define av_trasv_4_default                2   //avanzamento trasversale in centesimi/giro per rapporto semiNorton 4
#define offset_filetto_default            400 //offset per inizio filettatura (in passi stepper vite)
#define norton_default                    0   //posizione scatola NORTON avanzamenti (av_carro_1 e av_trasv_1)
#define MaxStepperSpeed_default           500 //Max speed allowed to the stepper motor



void FactoryDefaultEEPROM()     //rester the FACTORY DEFAULT values in the EEPROM
{
unsigned int UI;
float F;
byte B;
boolean BOO;
String C;
long L;
unsigned long UL;

  //RESTORES FACTORY DEFAULT PARAMETERS IN EEPROM
  UI = one_turn_mandrel_steps_default;   //
  EEPROM.put(0, UI);                     //number of mandrel encoder steps per revolution
  F = screw_pitch_default;               //
  EEPROM.put(2, F);                      //lead screw pitch
  UI = one_turn_screw_steps_default;     //
  EEPROM.put(6, UI);                     //number of lead screw steps (stepper motor steps per revolution)
  UI = AccelerationDelay_default;        //
  EEPROM.put(8, UI);                     //stepper motor acceleration delay  
  UI = DecelerationDelay_default;        //
  EEPROM.put(10, UI);                    //stepper motor deceleration delay
  
  //CARRIAGE AND CROSS SLIDE FEEDS FOR EVERY NORTON GEARBOX RATIO (4 RATIOS IN TOTAL)
  UI = av_carro_1_default;
  EEPROM.put(12, UI);          
  UI = av_trasv_1_default;
  EEPROM.put(14, UI);
  UI = av_carro_2_default;
  EEPROM.put(16, UI);
  UI = av_trasv_2_default;
  EEPROM.put(18, UI);
  UI = av_carro_3_default;
  EEPROM.put(20, UI);
  UI = av_trasv_3_default;
  EEPROM.put(22, UI);
  UI = av_carro_4_default;
  EEPROM.put(24, UI);
  UI = av_trasv_4_default;
  EEPROM.put(26, UI);
  
  BOO = CW_default;         //
  EEPROM.put(28, BOO);      //
  BOO = CCW_default;        //STEPPER MOTOR DIRECTION
  EEPROM.put(29, BOO);      //

  UI = offset_filetto_default;          //
  EEPROM.put(30, UI);                   //offset for thread start
  B = norton_default;                   //
  EEPROM.put(32,B);                     //NORTON GEARBOX POSITION
  UL = MaxStepperSpeed_default;         //
  EEPROM.put(33,UL);                    //Max stepper speed
}



void WriteToEEPROM()  //Writes the current value to the EEPROM if they've been modified by the user
                      //Factory default setting can be reset with the FactoryDefaultEEPROM function
{
  EEPROM.put(0, one_turn_mandrel_steps);  //number of encoder steps per mandrel revolution (unsigned int)
  EEPROM.put(2, screw_pitch);             //leadscrew pitch value (float decimal)
  EEPROM.put(6, one_turn_screw_steps);    //number of STEPPER steps per leadscrew revolution (unsigned int)
  EEPROM.put(8, AccelerationDelay);       //delay value for the STEPPER acceleration in microseconds (unsigned int)
  EEPROM.put(10, DecelerationDelay);      //delay value for the STEPPER deceleration in microseconds (unsigned int)
  //this cycle takes the 4 values of the feed (both carriage and cross-slide) from the 2 arrays (av_carro and av_trasv)
  //and put them in the EEPROM
  byte address = 12;
  for (byte i = 0; i < 4; i++)
  {
    EEPROM.put(address, av_carro[i]);
    address += 2;
    EEPROM.put(address, av_trasv[i]);
    address += 2;
  }
  EEPROM.put(28, CW);                     //ClockWise direction of the leadscrew stepper (boolean)
  EEPROM.put(29, CCW);                    //CounterClockWise direction of the leadscrew stepper (boolean)
  EEPROM.put(30, thread_offset_steps);    //offset for thread start                 
  EEPROM.put(32,NORTON_gearbox);          //NORTON GEARBOX POSITION
  EEPROM.put(33,MaxStepperSpeed);         //Max stepper speed limit
  
  
}


void LoadFromEEPROM() //Loads the parameters from the EEPROM and put them in the proper variables
{
  EEPROM.get(0, one_turn_mandrel_steps);  //number of encoder steps per mandrel revolution (unsigned int)
  EEPROM.get(2, screw_pitch);             //leadscrew pitch value (float decimal)
  EEPROM.get(6, one_turn_screw_steps);    //number of STEPPER steps per leadscrew revolution (unsigned int)
  EEPROM.get(8, AccelerationDelay);       //delay value for the STEPPER acceleration in microseconds (unsigned int)
  EEPROM.get(10, DecelerationDelay);      //delay value for the STEPPER deceleration in microseconds (unsigned int)
  //this cycle takes the 4 values of the feed (both carriage and cross-slide) and put them in 2 arrays
  //one for the carriage (av_carro) and the other for the cross-slide(av_trasv)
  byte address = 12;
  for (byte i = 0; i < 4; i++)
  {
    EEPROM.get(address, av_carro[i]);
    address += 2;
    EEPROM.get(address, av_trasv[i]);
    address += 2;
  }
  EEPROM.get(28, CW);                     //ClockWise direction of the leadscrew stepper (boolean)
  EEPROM.get(29, CCW);                    //CounterClockWise direction of the leadscrew stepper (boolean)
  EEPROM.get(30, thread_offset_steps);    //offset for thread start
  EEPROM.get(32, NORTON_gearbox);          //NORTON GEARBOX POSITION
  EEPROM.get(33, MaxStepperSpeed);         //Max stepper speed limit
  
  
  single_step_pitch = screw_pitch/one_turn_screw_steps;   //calculates the linear movement (in mm) of the carriage for a single step of the stepper motor
}
