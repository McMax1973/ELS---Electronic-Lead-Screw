const char FIL0[] PROGMEM = "imposta passo mm";
const char FIL1[] PROGMEM = "imposta passo TPI";
const char FIL2[] PROGMEM = "imposta passo MOD";
const char FIL3[] PROGMEM = "sviluppo";
const char FIL4[] PROGMEM = "fil. a misura";
const char FIL5[] PROGMEM = "vincolo meccanico";
const char FIL6[] PROGMEM = "destro  ";
const char FIL7[] PROGMEM = "sinistro";
const char FIL8[] PROGMEM = "VINCOLATO";
const char FIL9[] PROGMEM = "passo: ";
const char FIL10[] PROGMEM = "SEL: blocca";
const char FIL11[] PROGMEM = "SEL+RESET: ritorna ";
const char FIL12[] PROGMEM = "SEL:start  ESC:esci";
const char FIL13[] PROGMEM = "ESC:esci";
const char FIL14[] PROGMEM = "dist:";
const char FIL15[] PROGMEM = "imposta corsa";
const char FIL16[] PROGMEM = " ** FILETTATURA **  ";
const char FIL17[] PROGMEM = "aggiusta posizione";
const char FIL18[] PROGMEM = " < -------------- >";
const char* const MenuFilettatura[] PROGMEM = {FIL0, FIL1, FIL2, FIL3, FIL4, FIL5,
                                               FIL6, FIL7, FIL8, FIL9, FIL10, FIL11, FIL12, FIL13, FIL14, FIL15, FIL16, FIL17, FIL18};

void CreaSequenza(float passo)  // this function generates the array with the step configuration to build the screw with the
// reqeusted pitch "passo" passed in the parameter
{
  // local variable declaration
  int mandrel_steps_counter = 0;     // variable temporanea da eliminare che totalizza il numero di passi per verificare che vengano effetuati tuti i passi dell'encoder mandrino
  float real_absolute_ratio = 0;         //this variable is used to calculate the ratio between mandrel steps and screw steps
  unsigned long long_progressive_ratio = 0;      //this variable is used to store the progrerssive ratio in long integer format including the remainder of the previous step
  float real_progressive_ratio = 0.0;    //this variable is used to store the progrerssive ratio in float format including the remainder of the previous step
  float decimal_remainder = 0;           //this variable is used to store the decimal remainder of the calculated ratio
  byte actual_integer_steps = 0;         //this variable is used to store the interger part of the number of steps, separated from the decimal remainder
  
  unsigned int matched_mandrel_steps = one_turn_mandrel_steps; //this is the calculated mandrel steps in order to get a "clear" ratio to compansate system nonlinearity and inches to mm conversion
  unsigned int progressive_mandrel_steps;                      //this is used to calculate the matched mandrel steps
  float progressive_pitch;                              //this is used to calculate the reduced pitch according to the calculated matched mandrel steps
  unsigned long best_remainder = 99999;                             //this is the remainder to check when it reaches 0
  unsigned long int_ratio;
  unsigned long integer_remainder;
  float matched_pitch = passo;
  
  // end local variable declaration

  for (pointer = 0; pointer < MaxSteps; pointer++) //this cycle initializes the "sequenza" array to 0
    sequenza[pointer] = 0;
  
  for (progressive_mandrel_steps = 1; progressive_mandrel_steps < MaxSteps; progressive_mandrel_steps++)
  {  
    progressive_pitch = (passo/one_turn_mandrel_steps)*progressive_mandrel_steps;    //progressive pitch calculated on the current mandrel steps
    long_progressive_ratio = (progressive_pitch/single_step_pitch)*1000000;          //calculates the number of screw steps required to perform the threadmultiplied by 1000000 to avoid float
    integer_remainder = long_progressive_ratio % 1000000;
    actual_integer_steps = long_progressive_ratio/1000000;                           //extract the integer part of the calculated steps
    if (integer_remainder > 500000) integer_remainder = 1000000 - integer_remainder; //extract the decimal part of the calculated steps
    if (integer_remainder < 500) integer_remainder = 0;                              //round the last 2.5 digits of the calculated steps
    if (integer_remainder < best_remainder){                                         //if the current remainder il lower than the current best...
      best_remainder = integer_remainder;                                            //make the actual the current best
      matched_mandrel_steps = progressive_mandrel_steps;                             //save the current progressive steps
      matched_pitch = progressive_pitch;                                             //Matched_pitch is the pitch of the screw to be created reduced by the angular factor of the partial mandrel revolution
    }
    if (best_remainder == 0) break;                          //if the best remainder is already 0 we have found the best combination: exit the cycle immediately
  }

  numero_passi = round(matched_pitch / single_step_pitch);     //numero di passi della vite madre da eseguire per ottenere il passo ridotto (matched_pitch)
  real_absolute_ratio = float(matched_mandrel_steps) / numero_passi; //rapporto tra passi mandrino e passi vite calcolato con il passo angolare ridotto
  mandrel_steps_counter = 0;                                          //azzero il contatore passi mandrino

  /* //DEBUG
  
  Serial.println("profilo definito");
  Serial.print("one_turn_mandrel_steps ");
  Serial.println(one_turn_mandrel_steps);
  Serial.print("single_step_pitch ");
  Serial.println(single_step_pitch,8);
  Serial.print("passo ");
  Serial.println(passo, 4);
  Serial.println();
  
  Serial.print("best remainder ");
  Serial.println(best_remainder);
 
  Serial.print("matched_mandrel_steps ");
  Serial.println(matched_mandrel_steps);
 
  Serial.print("matched_pitch ");
  Serial.println(matched_pitch, 8);
  Serial.print("numero_passi ");
  Serial.println(numero_passi);
  Serial.print("real_absolute_ratio ");
  Serial.println(real_absolute_ratio, 8);
  Serial.println();
  Serial.println("***end definizione profilo*****");
  Serial.println();
  Serial.println();
   
  */ //END DEBUG
  
  for (pointer = 0; pointer < numero_passi; pointer++)                //ciclo che carica i valori dei passi nell'array che ha tante celle quanti sono i passi vite da eseguire
  {
    real_progressive_ratio = real_absolute_ratio + decimal_remainder;      //il rapporto progressivo è uguale al rapporto assoluto con aggiunto il resto decimale del rapporto precedente
    actual_integer_steps = int(real_progressive_ratio);                     //identifico quanti passi interi devo eseguire prendendo la sola parte intera del rapporto progressivo
    decimal_remainder = real_progressive_ratio - actual_integer_steps;      //estraggo la parte deimale dal rapporto progressivo
    mandrel_steps_counter = mandrel_steps_counter + actual_integer_steps;   //incremento il contatore passi mandrino aggiungendo i passi interi appena calcolati
    sequenza[pointer] = actual_integer_steps;                               //inserisco il valore dei passi interi appena calcolati nella cella corrente dell'array
  }
  pointer--;                                                                                 //decremento il puntatore e mi sposto sull'ultimo valore dell'array
  sequenza[pointer] = sequenza[pointer] + (matched_mandrel_steps - mandrel_steps_counter);  //aggiusto il valore dell'ultima cella in modo da avere il totale passi mandrino corretto

  /* //DEBUG
  
  Serial.println("seuqenza array");
  for (pointer = 0; pointer < numero_passi; pointer++)
  {
    Serial.print("cella ");
    Serial.print(pointer);
    Serial.print("  valore ==> ");
    Serial.println(sequenza[pointer]);
  }
  
  Serial.println();
  Serial.println("****end sequenza array****");
  Serial.println();
  Serial.println();
   
  */ //END DEBUG
 
} //END CreaSequenza



void InterruptEncoderFilettatura()   //this is the Interrupt Service Routine which manage the encoder inputs while threading
{
  static byte prev_encoder;          // this variable stores the value of the input port register of the previous encoder state
  byte port;                         // this variable stores the value of the input port register of the current encoder state

  port = PIND & B00001100;           //reads input pins 2 and 3 of the Arduino uno PORTD D and puts the binary image on the "port" byte variable
  port |= prev_encoder;              //makes the logic "OR" between the current "port" byte and the previous
  steps = steps + encoder[port];     //increment/decrement the encoder couter according to the previous and current encoder values
  absolute_encoder_steps = absolute_encoder_steps + encoder[port]; //increment the absolute encoder counter
 
  port >>= 2;                        //shift the port image by 2 bits on right
  prev_encoder = port;               //assign the current port image to the "prev_encoder" byte by keeping just the bits n. 2 and 3
  step_flag = true;
}                                    //END of the Interrupt Service Routine

char FilettaturaMenu()
{
  static char Current;
  char previous = 5;
  char next = 1;
  boolean stay = true;
  boolean innerstay = true;

  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print(">");
  lcd.setCursor(19, 2);
  lcd.print("<");
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
  lcd.setCursor(0, 0);
  lcd.print(buff);


  while (stay)
  {
    switch (Current) {
      case -1: Current = 5; break;
      case  6: Current = 0; break;
    }
    previous = Current - 1;
    next = Current + 1;
    if (previous == -1)
      previous = 5;
    if (next == 6)
      next = 0;

    strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[previous])));
    lcd.setCursor(1, 1);
    lcd.print("                  ");
    lcd.setCursor(1, 1);
    lcd.print(buff);

    strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[Current])));
    lcd.setCursor(1, 2);
    lcd.print("                  ");
    lcd.setCursor(1, 2);
    lcd.print(buff);

    strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[next])));
    lcd.setCursor(1, 3);
    lcd.print("                  ");
    lcd.setCursor(1, 3);
    lcd.print(buff);

    innerstay = true;
    while (innerstay)
    {
      if (digitalRead(SEL) == PRESSED) {
        stay = false;
        innerstay = false;
      }
      if (analogRead(ANALOG_Y) < LOW_1) {
        Current -= 1;
        innerstay = false;
      }
      if (analogRead(ANALOG_Y) > HIGH_1) {
        Current += 1;
        innerstay = false;
      }
      if (digitalRead(ESC) == PRESSED) {
        Current = 6;
        stay = false;
        innerstay = false;
      }
    }
    delay (300);
  }

  return Current;
}

void ImpostaPassoMetrico() //this set the required thread pitch in mm and creates the valiue array for the threading
{
  float MaxPasso = ((float)(((int)((single_step_pitch * one_turn_mandrel_steps)*20))))/20;
  boolean stay = true;

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[0])));
  lcd.setCursor(1, 1);
  lcd.print(buff);
  lcd.setCursor(1, 3);
  lcd.print("(max) "); lcd.print(MaxPasso, 2); lcd.print(" mm");
  lcd.setCursor(6, 2);
  lcd.print("mm");
  delay(200);
  while (stay)
  {
    if (digitalRead(SEL) == PRESSED) stay = false;
    if (digitalRead(ESC) == PRESSED) {delay(250); return;}
    if ((analogRead(ANALOG_Y) < LOW_TOL) && (analogRead(ANALOG_Y) > LOW_3)) thread_pitch -= 0.05;
    if (analogRead(ANALOG_Y) <= LOW_3) thread_pitch -= 0.1;
    if ((analogRead(ANALOG_Y) > HIGH_TOL) && (analogRead(ANALOG_Y) < HIGH_3)) thread_pitch += 0.05;
    if (analogRead(ANALOG_Y) >= HIGH_3) thread_pitch += 0.1;
    if (thread_pitch > MaxPasso) thread_pitch = MaxPasso;
    if (thread_pitch < 0.2) thread_pitch = 0.2;
    lcd.setCursor(1, 2);
    lcd.print(thread_pitch, 2);
    delay(150);
  }
  thread_type = 0;        //0 = metric thread; 1 = TPI(inch) thread; 2 = Module (mod) thread
  CreaSequenza(thread_pitch);
}

void ImpostaPassoPollici() //this set the required thread pitch in inches and creates the valiue array for the threading
{
  float MaxPasso = single_step_pitch * one_turn_mandrel_steps;
  float thread_pitch_mm;
  boolean stay = true;
  byte MaxTPI = 1+(1/(MaxPasso/25.4));

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[1])));
  lcd.setCursor(1, 1);
  lcd.print(buff);
  lcd.setCursor(1, 3);
  lcd.print("(max) "); lcd.print(MaxTPI); lcd.print(" TPI");
  lcd.setCursor(6, 2);
  lcd.print("TPI");
  delay(200);
  while (stay)
  {
    if (digitalRead(SEL) == PRESSED) stay = false;
    if (digitalRead(ESC) == PRESSED) {delay(250); return;}
    if (analogRead(ANALOG_Y) < LOW_3) TPI -= 1;
    if (analogRead(ANALOG_Y) > HIGH_3) TPI += 1;
    if (TPI < MaxTPI) TPI = MaxTPI;
    if (TPI > 127) TPI = 127;
    lcd.setCursor(1, 2);
    lcd.print(TPI);
    lcd.print(" ");
    delay(150);
  }
  thread_type = 1;        //0 = metric thread; 1 = TPI(inch) thread; 2 = Module (mod) thread
  thread_pitch_mm = 25.4/(float)TPI;
  CreaSequenza(thread_pitch_mm);
}

void ImpostaPassoModulo() //this set the required module pitch and creates the value array for the threading
{
  float MaxPasso = single_step_pitch * one_turn_mandrel_steps;
  float MaxModulo = ((float)(((int)((MaxPasso/PiGreco)*20))))/20;
  float thread_pitch_mm;
  boolean stay = true;

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[0])));
  lcd.setCursor(1, 1);
  lcd.print(buff);
  lcd.setCursor(1, 3);
  lcd.print("(max) "); lcd.print(MaxModulo, 2); lcd.print(" MOD");
  lcd.setCursor(6, 2);
  lcd.print("MOD");
  delay(200);
  while (stay)
  {
    if (digitalRead(SEL) == PRESSED) stay = false;
    if (digitalRead(ESC) == PRESSED) {delay(250); return;}
    if ((analogRead(ANALOG_Y) < LOW_TOL) && (analogRead(ANALOG_Y) > LOW_3)) mod_pitch -= 0.05;
    if (analogRead(ANALOG_Y) <= LOW_3) mod_pitch -= 0.1;
    if ((analogRead(ANALOG_Y) > HIGH_TOL) && (analogRead(ANALOG_Y) < HIGH_3)) mod_pitch += 0.05;
    if (analogRead(ANALOG_Y) >= HIGH_3) mod_pitch += 0.1;
    if (mod_pitch > MaxModulo) mod_pitch = MaxModulo;
    if (mod_pitch < 0.1) mod_pitch = 0.1;
    lcd.setCursor(1, 2);
    lcd.print(mod_pitch, 2);
    delay(150);
  }
  thread_type = 2;            //0 = metric thread; 1 = TPI(inch) thread; 2 = Module (mod) thread
  thread_pitch_mm = mod_pitch*PiGreco;
  CreaSequenza(thread_pitch_mm);
}




void Sviluppo()         //set the threading direction (left-hand or right-hand)
{
  boolean stay = true;
  boolean innerStay = true;

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[3])));
  lcd.setCursor(1, 1);
  lcd.print(buff);
  while (stay)
  {
    if (sviluppo_filetto) strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[6]))); else strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[7])));
    lcd.setCursor(1, 2);
    lcd.print(buff);
    innerStay = true;
    while (innerStay)
    {
      if (digitalRead(SEL) == PRESSED) {
        innerStay = false;
        stay = false;
      }
      if ((analogRead(ANALOG_Y) < LOW_1) | (analogRead(ANALOG_Y) > HIGH_1)) {
        innerStay = false;
        sviluppo_filetto = !(sviluppo_filetto);
        delay(50);
      }
    }
    delay(200);
  }
}


void FilettaturaToPosition() //threading to a specific distance (in mm with 2 decimals)
{
  boolean resta = true;
  boolean pass_done = false;
  boolean stay = true;
  boolean esegui = true;
  unsigned long ThreadSteps = 0;
  unsigned int joy;
  unsigned long stepperSpeed;

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16]))); //FILETTATURA
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[17]))); //aggiusta posizione
  lcd.setCursor(1, 1);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[18]))); //< -------------- >
  lcd.setCursor(0, 2);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[12]))); //SEL:start ESC:esci
  lcd.setCursor(0, 3);
  lcd.print(buff);
  
  SetPWM();
  while ((stay) && (esegui))
  {
    joy = analogRead(ANALOG_X);
    if (joy >= HIGH_TOL) { stepperSpeed = 50; Direction = CCW;}
    if (joy <= LOW_TOL) { stepperSpeed = 50; Direction = CW;}
    if ((joy >= LOW_TOL) && (joy <=HIGH_TOL)) stepperSpeed = 0;
    if (digitalRead(DIR) != Direction) {if (Speed == 0) digitalWrite(DIR, Direction); delayMicroseconds(10);}
    if (digitalRead(SEL) == PRESSED) stay = false;
    if (digitalRead(ESC) == PRESSED){ esegui = false; stay = false; delay(200);}
    if (digitalRead(DIR) == Direction) StepperRunToSpeed(stepperSpeed); else StepperRunToSpeed(0);
    delay (10);
  }
  StepperRunToSpeed(0);   // be sure the stepper motor is stopped
  delay(50);
  
  ClearPWM();
  if (esegui == false) return;
  stay = true;
  
  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16]))); //FILETTATURA
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[15]))); //IMPOSTA CORSA
  lcd.setCursor(1, 1);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[12]))); //SEL:start ESC:esci
  lcd.setCursor(0, 3);
  lcd.print(buff);
  
  delay(200);
  while ((stay) && (esegui))
  {
    if (digitalRead(SEL) == PRESSED) stay = false;
    if (digitalRead(ESC) == PRESSED){ esegui = false; stay = false;}
    if ((analogRead(ANALOG_Y) < LOW_TOL) && (analogRead(ANALOG_Y) > LOW_3)) distance -= 0.1;
    if (analogRead(ANALOG_Y) <= LOW_3) distance -= 1.0;
    if ((analogRead(ANALOG_Y) > HIGH_TOL) && (analogRead(ANALOG_Y) < HIGH_3)) distance += 0.1;
    if (analogRead(ANALOG_Y) >= HIGH_3) distance += 1.0;
    if (distance < 0) distance = 0.0;
    lcd.setCursor(1, 2);
    if (distance > 0) {lcd.print(distance, 2); lcd.print(" mm");} else lcd.print ("OFF  ");
    delay(150);
  }
  if (distance == 0) esegui = false;
  if (esegui){ 
    stay = true;  
    ClearPWM();
    SetOneShot();
    lcd.clear();
    strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
    lcd.setCursor(0, 0);
    lcd.print(buff);
    strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[9])));
    lcd.setCursor(1, 1);
    lcd.print(buff); 
    switch (thread_type) {
    case 0: lcd.print(thread_pitch, 2); lcd.print(" mm"); break;
    case 1: lcd.print(TPI); lcd.print(" TPI"); break;
    case 2: lcd.print(mod_pitch, 2); lcd.print(" MOD"); break;
    }
    lcd.setCursor(18, 1);
    if (sviluppo_filetto) lcd.print("DX"); else lcd.print("SX");
    strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[13])));
    lcd.setCursor(11, 3);
    lcd.print(buff);
    ThreadSteps = round(distance / single_step_pitch); //calculates the number of steps for the screw depending on the thread lenght
    if (sviluppo_filetto == false) {
      CW = !(CW);  //if the threading is LEFT-HAND, temporarily invert the screw rotation
      CCW = !(CCW);
    }
    attachInterrupt(0, InterruptEncoderFilettatura, CHANGE);
    attachInterrupt(1, InterruptEncoderFilettatura, CHANGE);
    step_flag = false;
    steps = 0;
    pointer = 0;
    absolute_steps = 0;
    StepperMoveToPosition(thread_offset_steps);
    absolute_steps = ThreadSteps+thread_offset_steps;
    absolute_encoder_steps = one_turn_mandrel_steps/2;
    while (stay)
    {
      lcd.setCursor(0, 3);
      lcd.print("         ");
      pass_done = false;
      
      
      while (((absolute_encoder_steps % one_turn_mandrel_steps) != 0) && (stay))                 //wait here until the mandrel catch the sync with the screw
      { 
        if (digitalRead(ESC) == PRESSED) stay = false;
      } 
      
      steps = 0;   
      while (pass_done == false)
      {
        while (step_flag == false)
        {
          if (digitalRead(ESC) == PRESSED) {
            step_flag = true;
            stay = false;
            pass_done = true;
          }
        }
        step_flag = false;
        if (steps == -1)
        {
          digitalWrite (DIR, CCW);
          absolute_steps++;
          pointer--;
          if (pointer == -1) pointer = numero_passi - 1;
          steps = sequenza[pointer] - 1;
          FireStep();
        }
        if (steps == sequenza[pointer])
        {
          digitalWrite (DIR, CW);
          absolute_steps--;
          pointer++;
          if (pointer == numero_passi) pointer = 0;
          steps = 0;
          FireStep();
        }
        if (digitalRead(ESC) == PRESSED) {
            stay = false;
            pass_done = true;
          }
        if (absolute_steps == 0) { 
          pass_done = true;
          strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[11])));
          lcd.setCursor(0, 3);
          lcd.print(buff);
          while ((digitalRead(SEL)!= PRESSED) || (digitalRead(RESET) != PRESSED)) delayMicroseconds(1);  // waits here at the end of the pass to press
          StepperMoveToPosition(ThreadSteps + thread_offset_steps);                                      // SEL+RESET to return the carriage back
                                                                                                         // this allow to move the cross-slide back before coming back
          strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[12])));
          lcd.setCursor(0, 3);
          lcd.print(buff);
          while ((digitalRead(SEL)!= PRESSED) && (digitalRead(ESC) != PRESSED)) delayMicroseconds(1);    //waits here to restart - time to reposition the cross-slide
          if (digitalRead(ESC) == PRESSED) stay = false;      
          
        }
      }
    }
    detachInterrupt(0);
    detachInterrupt(1);
    ClearPWM();
    if (sviluppo_filetto == false) {
      CW = !(CW);  //restore the original CW and CCW directions
      CCW = !(CCW);
    }
  }
  delay(300); 
}

void Filettatura()
{
  boolean resta = true;
  byte Opzione;

  switch (thread_type) {                 //creates the sequence array for threading using the current set thread type
    case 0: CreaSequenza(thread_pitch); break;
    case 1: CreaSequenza(25.4/(float)TPI); break;
    case 2: CreaSequenza(mod_pitch*PiGreco); break;
  }
  
  
  while (resta)
  {
    Opzione = FilettaturaMenu();
    switch (Opzione) {
      case 0: ImpostaPassoMetrico(); break;
      case 1: ImpostaPassoPollici(); break;
      case 2: ImpostaPassoModulo(); break;
      case 3: Sviluppo(); break;
      case 4: FilettaturaToPosition(); break;
      case 5: VincoloMeccanico(); break;
      case 6: resta = false; break;
    }
  }
}


void VincoloMeccanico() //this function simply links the screw to the mandrel with a pseudo-mechanical link, emulating the
                        //same situation you have on a lathe with gears or a NORTON gearbox
{
  boolean stay = true;      //used to stay or go out from while cycles

  ClearPWM();
  SetOneShot();

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[16])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[8])));
  lcd.setCursor(0, 1);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[9])));
  lcd.setCursor(1, 2);
  lcd.print(buff); 
  switch (thread_type) {
    case 0: lcd.print(thread_pitch, 2); lcd.print(" mm"); break;
    case 1: lcd.print(TPI); lcd.print(" TPI"); break;
    case 2: lcd.print(mod_pitch, 2); lcd.print(" MOD"); break;
  }
  strcpy_P(buff, (char*)pgm_read_word(&(MenuFilettatura[13])));
  lcd.setCursor(0, 3);
  lcd.print(buff);
  lcd.setCursor(18, 2);
  if (sviluppo_filetto) lcd.print("DX"); else lcd.print("SX");

  if (sviluppo_filetto == false) {
    CW = !(CW);  //if the threading is LEFT-HAND, temporarily invert the screw rotation
    CCW = !(CCW);
  }
  steps = 0;
  pointer = 0;
  step_flag = false;
  attachInterrupt(0, InterruptEncoderFilettatura, CHANGE);
  attachInterrupt(1, InterruptEncoderFilettatura, CHANGE);

  while (stay)
  {
    while (step_flag == false)
    {
      if (digitalRead(ESC) == PRESSED) {
        step_flag = true;
        stay = false;
      }
    }
    step_flag = false;
    if (steps == -1)
    {
      digitalWrite (DIR, CCW);
      absolute_steps++;
      pointer--;
      if (pointer == -1)  pointer = numero_passi - 1;
      steps = sequenza[pointer] - 1;
      FireStep();
    }
    if (steps == sequenza[pointer])
    {
      digitalWrite (DIR, CW);
      absolute_steps--;
      pointer++;
      if (pointer == numero_passi) pointer = 0;
      steps = 0;
      FireStep();
    }
  }
  detachInterrupt(0);
  detachInterrupt(1);
  if (sviluppo_filetto == false) {
    CW = !(CW);  //restore the original CW and CCW directions only if we have made a left-hand screw
    CCW = !(CCW);
  }
  delay(300);
}
