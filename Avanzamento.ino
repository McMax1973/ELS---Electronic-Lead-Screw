
const char AVA0[] PROGMEM = " ** AVANZAMENTO **  ";
const char AVA1[] PROGMEM = "Long./Trasv.";
const char AVA2[] PROGMEM = "Avanzam. libero";
const char AVA3[] PROGMEM = "Avanzam. vincolato";
const char AVA4[] PROGMEM = "Imposta 'NORTON'";
const char AVA5[] PROGMEM = "Modo avanzamento";
const char AVA6[] PROGMEM = "***";      //Empty space for additional functions
const char AVA7[] PROGMEM = "longitudinale";
const char AVA8[] PROGMEM = "trasversale  ";
const char AVA9[] PROGMEM = "SEL: OK";
const char AVA10[] PROGMEM = "ESC:fine";
const char AVA11[] PROGMEM = "SEL:start ";
const char AVA12[] PROGMEM = "SEL:stop ";
const char AVA13[] PROGMEM = " <=== ";
const char AVA14[] PROGMEM = " ===> ";
const char AVA15[] PROGMEM = "Posizione";
const char AVA16[] PROGMEM = "Pos";
const char AVA17[] PROGMEM = "mm";
const char AVA18[] PROGMEM = "==";
const char AVA19[] PROGMEM = "mm/min";
const char AVA20[] PROGMEM = "RPM";
const char AVA21[] PROGMEM = "SEL:cambia";
const char AVA22[] PROGMEM = "cent/g";

const char* const MenuAvanzamento[] PROGMEM = {AVA0, AVA1, AVA2, AVA3, AVA4, AVA5, AVA6,
                                               AVA7, AVA8, AVA9, AVA10, AVA11, AVA12, AVA13,
                                               AVA14, AVA15, AVA16, AVA17, AVA18, AVA19, AVA20, 
                                               AVA21, AVA22};



void ImpostaModalitaAvanzamento()
{
  boolean stay = true;
  
  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[0])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[5])));
  lcd.setCursor(2, 1);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[9])));
  lcd.setCursor(0, 3);
  lcd.print(buff);
  
  
  while (stay)
  {
    if ((analogRead(ANALOG_Y) < LOW_1) || (analogRead(ANALOG_Y) > HIGH_1)) {mm_min = !(mm_min); delay (400);}
    if (mm_min) strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[19])));
    else strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[22])));
    lcd.setCursor(1, 2);
    lcd.print(buff);
    if (digitalRead(SEL) == PRESSED) {stay = false; delay (100);}     
  }
  
  delay(300);
  
}


char AvanzamentoMenu() 
{
  static char Current;
  char previous = 4;
  char next = 2;
  boolean stay = true;
  boolean innerstay = true;

  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print(">");
  lcd.setCursor(19, 2);
  lcd.print("<");
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[0])));
  lcd.setCursor(0, 0);
  lcd.print(buff);


  while (stay)
  {
    switch (Current) {
      case 0: Current = 6; break;
      case 7: Current = 1; break;
    }
    previous = Current - 1;
    next = Current + 1;
    if (previous == 0)
      previous = 6;
    if (next == 7)
      next = 1;

    strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[previous])));
    lcd.setCursor(1, 1);
    lcd.print("                  ");
    lcd.setCursor(1, 1);
    lcd.print(buff);

    strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[Current])));
    lcd.setCursor(1, 2);
    lcd.print("                  ");
    lcd.setCursor(1, 2);
    lcd.print(buff);

    strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[next])));
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
        Current = 7;
        stay = false;
        innerstay = false;
      }
    }
    delay (300);
  }

  return Current;
}




void ImpostaNORTON()
{
  boolean stay = true;
  boolean innerStay = true;

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[0])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[4])));
  lcd.setCursor(1, 1);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[9])));
  lcd.setCursor(0, 3);
  lcd.print(buff);

  while (stay)
  {
    lcd.setCursor(1, 2);
    lcd.print(NORTON_gearbox + 1);
    innerStay = true;
    while (innerStay)
    {
      if (digitalRead(SEL) == PRESSED) {
        innerStay = false;
        stay = false;
      }
      if (analogRead(ANALOG_Y) < LOW_1) {
        innerStay = false;
        if (NORTON_gearbox > 0) NORTON_gearbox --;
        delay(400);
      }
      if (analogRead(ANALOG_Y) > HIGH_1) {
        innerStay = false;
        if (NORTON_gearbox < 3) NORTON_gearbox ++;
        delay(400);
      }
    }
  }
  EEPROM.put(32, NORTON_gearbox);       // Saves the new NORTON GEARBOX POSITION in the EEPROM
  delay (300);
}


void LongTrasv()
{
  boolean stay = true;
  boolean innerStay = true;

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[0])));
  lcd.setCursor(0, 0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[1])));
  lcd.setCursor(1, 1);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[9])));
  lcd.setCursor(0, 3);
  lcd.print(buff);

  while (stay)
  {
    if (CarroTrasv) strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[7]))); else strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[8])));
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
        CarroTrasv = !(CarroTrasv);
        delay(400);
      }
    }
  }
  delay (300);
}

void LCD_print_float(float value, byte col, byte lin)        //print a floating point value between -999.99 and 999.99 with always 3 integer digits (000 mask)
{
  float assoluto = abs(value);

  lcd.setCursor(col, lin);
  if (value >= 0) lcd.print(" ");    // value is 0 or positive, print a space
  else lcd.print("-");               // value is negative, print the minus before the zeros
  if (assoluto < 10) {
    lcd.print("00");  //value has 1 digit + 2 decimals
    lcd.print(assoluto, 2);
  }
  if ((assoluto >= 10) && (assoluto < 100)) {
    lcd.print("0");  //value has 2 digits + 2 decimals
    lcd.print(assoluto, 2);
  }
  if ((assoluto >= 100) && (assoluto < 1000)) lcd.print(assoluto, 2);              //value has 3 digits + 2 decimals
  lcd.print(" ");
  lcd.setCursor(col, lin);
}

void LCD_print_float_justified(float value, byte col, byte lin)    //print a floating point value between -999.99 and 999.99 right justified
{
  if (value >= 0) col++;                       //value is positive so move the LCD column 1 position on right
  if (abs(value) < 10) col += 2;                    //value has 1 digit + 2 decimals
  if (abs((value) >= 10) && (abs(value) < 100)) col += 1; //value has 2 digits + 2 decimals

  lcd.setCursor(col, lin);
  lcd.print(value, 2);
  lcd.setCursor(col, lin);
}

void LCD_print_unsignedint(unsigned int value) //print an unsigned int number on the current position of the LCD with a "0000" mask.
{
  if (value < 10) {
    lcd.print("000");
    lcd.print(value);
  }
  if ((value >= 10) && (value < 100)) {
    lcd.print("00");
    lcd.print(value);
  }
  if ((value >= 100) && (value < 1000)) {
    lcd.print("0");
    lcd.print(value);
  }
  if ((value >= 1000) && (value < 10000)) lcd.print(value);
}

float Change_float(float current_value, byte col, byte lin)
{
  boolean stay = true;
  boolean exit = false;
  float prev_value = current_value;
  float original_value = current_value;

  LCD_print_float(current_value, col, lin);

  while (stay)
  {
    if (digitalRead(ESC) == PRESSED) {
      stay = false;
      exit = true;
    }
    if (digitalRead(SEL) == PRESSED) stay = false;
    if (digitalRead(RESET) == PRESSED) current_value = 0.00;
    if ((analogRead(ANALOG_Y) <= LOW_TOL) && (analogRead(ANALOG_Y) > LOW_1)) {
      current_value -= 0.01;
      delay (300);
    }
    if ((analogRead(ANALOG_Y) <= LOW_1) && (analogRead(ANALOG_Y) > LOW_2)) {
      current_value -= 0.01;
      delay (100);
    }
    if ((analogRead(ANALOG_Y) <= LOW_2) && (analogRead(ANALOG_Y) > LOW_3)) {
      current_value -= 0.1;
      delay (100);
    }
    if (analogRead(ANALOG_Y) <= LOW_3)  {
      current_value -= 1;
      delay (100);
    }
    

    if ((analogRead(ANALOG_Y) >= HIGH_TOL) && (analogRead(ANALOG_Y) < HIGH_1)) {
      current_value += 0.01;
      delay(300);
    }
    if ((analogRead(ANALOG_Y) >= HIGH_1) && (analogRead(ANALOG_Y) < HIGH_2)) {
      current_value += 0.01;
      delay(100);
    }
    if ((analogRead(ANALOG_Y) >= HIGH_2) && (analogRead(ANALOG_Y) < HIGH_3)) {
      current_value += 0.1;
      delay (100);
    }
    if (analogRead(ANALOG_Y) >= HIGH_3)  {
      current_value += 1;
      delay (100);
    }

    if (current_value != prev_value) {
      LCD_print_float(current_value, col, lin);
      prev_value = current_value;
    }

  }
  delay(200);
  if (exit) {
    LCD_print_float(original_value, col, lin);
    return original_value;
  }
  else {
    LCD_print_float(current_value, col, lin);
    return current_value;
  }
}

void AvanzamentoLibero()
{
  boolean stay = true;
  boolean InnerStay = true;
  boolean RUN = false;
  boolean Update = true;

  unsigned int NORTON_feed;        //current FEED value with 1 turn of screw/Feedbar (cent/turn)
  unsigned int MaxFeed = 0;        //Max Feed speed in mm/min
  unsigned int MandrelSpeed = 0;   //current speed of the mandrel in RPM; used to calculate teh feedspeed while using cutting velocity in cent/turn 
  float Current_pos = 0.00;        //Current position of the carriage/cross-slide
  unsigned long StepperSpeed = 0;  //Current speed of the stepper motor (RPM)
  int FeedSpeed = 0;               //feed speed in mm/min


  if (CarroTrasv) NORTON_feed = av_carro[NORTON_gearbox]; //assign the value of the Feed given from the NORTON position and the carriage or cross-slide selected option (valid only for lathes with cross-slide feed gear)
  else            NORTON_feed = av_trasv[NORTON_gearbox];
  
  SingleStepFeed = (((float)NORTON_feed / 100) / (float)one_turn_screw_steps);      //calculates the feed given by a single step of the stepper motor
  Current_pos = (float)absolute_steps * SingleStepFeed;  //calculate the current position 
    
  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[2]))); lcd.setCursor(2, 0); lcd.print(buff);   //avanzam. libero
  
  if (mm_min) strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[19])));   // mm/min
  else strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[22])));          //cent/g
  lcd.setCursor(4, 2); lcd.print(buff);
  
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[20]))); lcd.setCursor(12, 2); lcd.print(buff); // RPM
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[11]))); lcd.setCursor(0, 3); lcd.print(buff);  // SEL:start
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[10]))); lcd.setCursor(12, 3); lcd.print(buff); // ESC:fine
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[13]))); lcd.setCursor(0, 1); lcd.print(buff);  // print <===
          
  
  LCD_print_float(Current_pos, 6, 1);  // print the current position
    
  lcd.setCursor(16, 2); LCD_print_unsignedint(GetSpeed(100));
  
  SetPWM();
  StopTimer1();

  FeedSpeed = 0;

  digitalWrite(DIR, CW);
  Direction = CW;

  while (stay)                 // MAIN CYCLE
  {
    while ((RUN == false) && (InnerStay))       // CYCLE WHEN THE FEED IS NOT RUNNING
    {
      delay (200);
      if (digitalRead(ESC) == PRESSED) {stay = false; InnerStay = false; delay(50);}
      if (digitalRead(RESET) == PRESSED) {absolute_steps = 0; Current_pos = absolute_steps * SingleStepFeed; LCD_print_float(Current_pos, 6, 1);}
      if (digitalRead(SEL) == PRESSED) {Update = true; RUN = true; strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[12]))); lcd.setCursor(0, 3); lcd.print(buff); delay(50);} // SEL:stop
      MandrelSpeed = GetSpeed(200);
      lcd.setCursor(16, 2); 
      LCD_print_unsignedint(MandrelSpeed);
      
      if (mm_min) MaxFeed = (NORTON_feed * MaxStepperSpeed) / 100;     //calculate the max feed speed starting from the max stepper speed (mm/min)
      else MaxFeed = (unsigned int)((float)NORTON_feed * (float)MaxStepperSpeed) / MandrelSpeed;  //calculate the max feed based on mandrel speed and max stepper motore speede (cent/g)
      if (FeedSpeed > MaxFeed){ FeedSpeed = MaxFeed; lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" "); Update = true;} //if the current FeedSpeed is higher than the max, limit and print it
      
      if (analogRead(ANALOG_Y) < LOW_TOL) {
        FeedSpeed --;
        if (FeedSpeed < 0) FeedSpeed = 0;
        lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" ");
        Update = true;
        delay(30);
      }
      if (analogRead(ANALOG_Y) > HIGH_TOL) {
        FeedSpeed ++;
        if (FeedSpeed > MaxFeed) FeedSpeed = MaxFeed;
        lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" ");
        Update = true;
        delay(30);
      }
      if ((analogRead(ANALOG_X) < LOW_1)) {
          digitalWrite(DIR, CW);
          Direction = CW;
          lcd.setCursor(14, 1);
          lcd.print("     ");
          strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[13])));   // print <===
          lcd.setCursor(0, 1);
          lcd.print(buff);
          Update = true;
      }
      if ((analogRead(ANALOG_X) > HIGH_1)) {
          digitalWrite(DIR, CCW);
          Direction = CCW;
          lcd.setCursor(0, 1);
          lcd.print("     ");
          strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[14])));   // print ===>
          lcd.setCursor(14, 1);
          lcd.print(buff);
          Update = true;
       }
    }
    while (RUN)               //CYCLE WHEN THE FEED IS RUNNING
    {
      if (digitalRead(ESC) == PRESSED) {StopTimer1(); Speed = 0; stay = false; InnerStay = false; RUN = false; delay(50);}
      if (digitalRead(RESET) == PRESSED)  absolute_steps = 0;
      if ((digitalRead(SEL) == PRESSED) && (analogRead(ANALOG_X) > LOW_TOL) && (analogRead(ANALOG_X) < HIGH_TOL)){  //IF SEL is pressed and the joy is not "X" moved
        StopTimer1(); Speed = 0;                                        // Stop timer 1 and reset stepper speed
        RUN = false;                                                    // clear the flag to come out from the RUN cycle 
        strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[11])));   // print SEL:start
        lcd.setCursor(0,3);
        lcd.print(buff);
        delay(150);
      }
      if (analogRead(ANALOG_Y) < LOW_TOL) {
        FeedSpeed --;
        if (FeedSpeed < 0) FeedSpeed = 0;
        lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" ");
        Update = true;
        delay(50);
      }
      if (analogRead(ANALOG_Y) > HIGH_TOL) {
        FeedSpeed ++;
        if (FeedSpeed > MaxFeed) FeedSpeed = MaxFeed;
        lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" ");
        Update = true;
        delay(50);
      }
      if ((analogRead(ANALOG_X) < LOW_1)) {
        if (Direction == CCW) {
          StepperRunToSpeed(0);
          digitalWrite(DIR, CW);
          Direction = CW;
          lcd.setCursor(14, 1);
          lcd.print("     ");
          strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[13])));   // print <===
          lcd.setCursor(0, 1);
          lcd.print(buff);
          Update = true;
        }
        else{
          if ((analogRead(ANALOG_X) < LOW_3) && (digitalRead(SEL) == PRESSED)) StepperRunToSpeed(MaxStepperSpeed); //move direction and press SEL to go fast
        }
      }
      if ((analogRead(ANALOG_X) > HIGH_1)) {
        if (Direction == CW) {
          StepperRunToSpeed(0);
          digitalWrite(DIR, CCW);
          Direction = CCW;
          lcd.setCursor(0, 1);
          lcd.print("     ");
          strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[14])));   // print ===>
          lcd.setCursor(14, 1);
          lcd.print(buff);
          Update = true;
        }
        else{
          if ((analogRead(ANALOG_X) > HIGH_3) && (digitalRead(SEL) == PRESSED)) StepperRunToSpeed(MaxStepperSpeed); //move direction and press SEL to go fast
        }
      }
      if (Update){
        if (mm_min) StepperSpeed = (unsigned long)((float)FeedSpeed / ((float)NORTON_feed / 100));
        else StepperSpeed = (unsigned long)(MandrelSpeed/((float)NORTON_feed / (float)FeedSpeed));
        StepperRunToSpeed(StepperSpeed);
        Update = false;
      }
      Current_pos = absolute_steps * SingleStepFeed;
      LCD_print_float(Current_pos, 6, 1);
    }
  }
  ClearPWM();
  Speed = 0;
  delay(300);
}


void AvanzamentoVincolato()
{
  boolean stay = true;
  boolean innerstay = true;
  boolean RUN = false;
  boolean Update = true;
  boolean left = true;             //used to check if the left direction is free (became flase when the Current_pos hits the left limit
  boolean right = true;            //used to check if the right direction is free (became flase when the Current_pos hits the right limit
  
  byte CursorPos = 0;              //0: SX value change; 1:DX Value change: 2:START
  float SX_pos = 0.00;             //left limit position
  float DX_pos = 0.00;             //right limit position
  float Current_pos = 0.00;        //Current position of the carriage/cross-slide
  int FeedSpeed = 0;               //feed speed in mm/min
  unsigned int MaxFeed = 0;        //Max Feed speed in mm/min
  unsigned int MandrelSpeed = 0;   //current speed of the mandrel in RPM; used to calculate the feedspeed while using cutting velocity in cent/turn
  unsigned long StepperSpeed;
  unsigned int NORTON_feed;        //current FEED value with 1 turn of screw/Feedbar (cent/turn)


  if (CarroTrasv) NORTON_feed = av_carro[NORTON_gearbox]; //assign the value of the Feed given from the NORTON position and the carriage or cross-slide selected option (valid only for lathes with cross-slide feed gear)
  else            NORTON_feed = av_trasv[NORTON_gearbox];

  SingleStepFeed = (((float)NORTON_feed / 100) / (float)one_turn_screw_steps);      //calculates the feed given by a single step of the stepper motor


  Current_pos = (float)absolute_steps * SingleStepFeed;
  SX_pos = Current_pos;
  DX_pos = Current_pos - 1.50;

  
  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[16]))); lcd.setCursor(4, 0); lcd.print(buff); // Pos
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[18]))); lcd.setCursor(9, 1); lcd.print(buff); // ==
  
  if (mm_min) strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[19])));   // mm/min
  else strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[22])));          //cent/g
  lcd.setCursor(4, 2); lcd.print(buff);
    
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[20]))); lcd.setCursor(12, 2); lcd.print(buff); // RPM
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[21]))); lcd.setCursor(0, 3);  lcd.print(buff); // SEL:cambia
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[10]))); lcd.setCursor(12, 3); lcd.print(buff); // ESC:fine
  strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[17]))); lcd.setCursor(18, 0); lcd.print(buff); // mm

  LCD_print_float(SX_pos, 0, 1);
  LCD_print_float(DX_pos, 13, 1);
  LCD_print_float(Current_pos, 8, 0);

  digitalWrite(DIR, CW);      //set standard feed in CW direction: right to left
  lcd.setCursor(8, 1);
  lcd.print("<");              //print the current feed direction
  Direction = CW;
  
  lcd.setCursor(0,2);
  lcd.print("0");

  SetPWM();
  StopTimer1();

  FeedSpeed = 0;

  while (stay)                                      // MAIN CYCLE
  {
    while ((RUN == false) && (innerstay))           // CYCLE WHEN THE FEED IS NOT RUNNING
    {
      if (digitalRead(ESC) == PRESSED) {innerstay = false;stay = false;delay(200);}
      if (digitalRead(RESET) == PRESSED){absolute_steps = 0; Current_pos = absolute_steps * SingleStepFeed; LCD_print_float(Current_pos, 8, 0);}
      if (analogRead(ANALOG_X) < LOW_3){ 
        if(CursorPos > 0) CursorPos--; 
        if (CursorPos == 2) strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[11]))); else strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[21])));
        lcd.noBlink();
        lcd.setCursor(0, 3); 
        lcd.print(buff);
        delay(200);
      }
      if (analogRead(ANALOG_X) > HIGH_3){ 
        if(CursorPos < 2) CursorPos++; 
        if (CursorPos == 2) strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[11]))); else strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[21])));
        lcd.noBlink();
        lcd.setCursor(0, 3); 
        lcd.print(buff);
        delay(200);
      }
      switch (CursorPos) {
        case 0: lcd.blink(); lcd.setCursor(0,1); break;
        case 1: lcd.blink(); lcd.setCursor(13,1); break;
        case 2: lcd.setCursor(16, 2); MandrelSpeed = GetSpeed(100); LCD_print_unsignedint(MandrelSpeed); break;
      }
      
      if (mm_min) MaxFeed = (NORTON_feed * MaxStepperSpeed) / 100;     //calculate the max feed speed starting from the max stepper speed (mm/min)
      else MaxFeed = (unsigned int)((float)NORTON_feed * (float)MaxStepperSpeed) / MandrelSpeed;  //calculate the max feed based on mandrel speed and max stepper motore speede (cent/g)
      if (FeedSpeed > MaxFeed){ FeedSpeed = MaxFeed; lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" "); Update = true;} //if the current FeedSpeed is higher than the max, limit and print it
      
      if (digitalRead(SEL) == PRESSED) {
        delay(200);
        switch (CursorPos) {
        case 0: lcd.noBlink(); lcd.cursor(); SX_pos = Change_float(SX_pos, 0,1); lcd.noCursor(); lcd.blink(); break; 
        case 1: lcd.noBlink(); lcd.cursor(); DX_pos = Change_float(DX_pos,13,1); lcd.noCursor(); lcd.blink(); break;
        case 2: RUN = true;        // set the flag to come in the RUN cycle 
                lcd.noBlink();
                strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[12])));  // print SEL:stop
                lcd.setCursor(0,3);
                lcd.print(buff);
                Update = true;
                delay(300);
                break;
        }
      }
    }
    while ((RUN) && (innerstay))                    //CYCLE WHEN THE FEED IS RUNNING
    {
      if (digitalRead(ESC) == PRESSED) {innerstay = false; stay = false; delay(300);}
      if (digitalRead(RESET) == PRESSED)  absolute_steps = 0;
      if ((digitalRead(SEL) == PRESSED) && (analogRead(ANALOG_X) > LOW_TOL) && (analogRead(ANALOG_X) < HIGH_TOL)){  //IF SEL is pressed and the joy is not "X" moved
        StopTimer1(); Speed = 0;                                        // Stop timer 1 and reset stepper speed
        RUN = false;                                                    // clear the flag to come out from the RUN cycle 
        strcpy_P(buff, (char*)pgm_read_word(&(MenuAvanzamento[11])));   // print SEL:start
        lcd.setCursor(0,3);
        lcd.print(buff);
        CursorPos = 2;                    //positioning the cursor on the START selection
        delay(300);
      }
      if (analogRead(ANALOG_Y) < LOW_TOL) {
        FeedSpeed --;
        if (FeedSpeed < 0) FeedSpeed = 0;
        lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" ");
        if (((left) && (Direction == CW)) || ((right) && (Direction == CCW))) Update = true;
        delay(30);
      }
      if (analogRead(ANALOG_Y) > HIGH_TOL) {
        FeedSpeed ++;
        if (FeedSpeed > MaxFeed) FeedSpeed = MaxFeed;
        lcd.setCursor(0, 2); lcd.print(FeedSpeed); lcd.print(" ");
        if (((left) && (Direction == CW)) || ((right) && (Direction == CCW))) Update = true;
        delay(30);
      }
      if ((analogRead(ANALOG_X) < LOW_1) && (left)) {
        if (Direction == CCW) {
          StepperRunToSpeed(0);
          digitalWrite(DIR, CW);
          Direction = CW;
          lcd.setCursor(11, 1);
          lcd.print(" ");
          lcd.setCursor(8, 1);
          lcd.print("<");
          Update = true;
        }
        else{
          if ((analogRead(ANALOG_X) < LOW_3) && (digitalRead(SEL) == PRESSED)) StepperRunToSpeed(MaxStepperSpeed); //move direction and press SEL to go fast
        }
      }
      if ((analogRead(ANALOG_X) > HIGH_1) && (right)) {
        if (Direction == CW) {
          StepperRunToSpeed(0);
          digitalWrite(DIR, CCW);
          Direction = CCW;
          lcd.setCursor(8, 1);
          lcd.print(" ");
          lcd.setCursor(11, 1);
          lcd.print(">");
          Update = true;
        }
        else{
          if ((analogRead(ANALOG_X) > HIGH_3) && (digitalRead(SEL) == PRESSED)) StepperRunToSpeed(MaxStepperSpeed); //move direction and press SEL to go fast
        }
      }
      if (Update){
        if (mm_min) StepperSpeed = (unsigned long)((float)FeedSpeed / ((float)NORTON_feed / 100));
        else StepperSpeed = (unsigned long)(MandrelSpeed/((float)NORTON_feed / (float)FeedSpeed));
        StepperRunToSpeed(StepperSpeed);
        Update = false;
      }
      Current_pos = absolute_steps * SingleStepFeed;
      if ((Current_pos >= SX_pos) && (Direction == CW)) {StopTimer1(); Speed = 0; left = false;} else left = true;
      if ((Current_pos <= DX_pos) && (Direction == CCW)) {StopTimer1(); Speed = 0; right = false;} else right = true;
      LCD_print_float(Current_pos, 8, 0);
    }
  }
  ClearPWM();
  Speed = 0;
  lcd.noBlink();
}


void Avanzamento()
{
  boolean resta = true;
  byte Opzione;

  while (resta)
  {
    Opzione = AvanzamentoMenu();
    switch (Opzione) {
      case 1: LongTrasv(); break;                  //long/trasv
      case 2: AvanzamentoLibero(); break;          //Avanzamento Libero
      case 3: AvanzamentoVincolato(); break;       //Avanzamnto Vincolato 
      case 4: ImpostaNORTON(); break;              //Imposta NORTON
      case 5: ImpostaModalitaAvanzamento(); break; //Imposta la modalita' di avanzamento
      case 6: break;                               //VUOTO 
      case 7: resta = false; break;
    }
  }
}


void InterruptEncoderAvanzamento()  //this is the Interrupt Service Routine which manage the encoder inputs while feeding (250 step/turn)
//activate this interrupt only with RISING on Encoder A channel (INT0)
{
  absolute_encoder_steps ++;
}


unsigned int GetSpeed (unsigned long frame)
{
  boolean stay = true;                    //boolean to determine if stay or not in the reading cycle
  unsigned int MandrelSpeed = 0;          //speed of the mandrel calculated
  unsigned long prev_millis = 0;          //used to store the millis alue for the speed count routine
  const float speed_multiplier = ((1000/(float)frame )/((float)one_turn_mandrel_steps/4))*60; //this is the multiplier used to calculate the speed
  // this is the multiplier used by to calculate the speed
  // one_turn_mandrel_steps is the total number of encoder steps per every single mandrel turn
  // the interrupt used in this routine is triggered only on one edge of one channel of the encoder, therefore it taked only 1 step every 4.
  // 1000/(float)interval: 1000msec (1sec) divided by the interval gives the ratio of the interval with respect to 1 second
  // (float)one_turn_mandrel_steps/4): the number of steps in one turn are divided by 4 (due to interrupt explained above)
  // (1000/(float)interval)/((float)one_turn_mandrel_steps/4): this therefore gives the number of turn per seconds, which are then multiplied by 60 to get the speed in RPM
    
  absolute_encoder_steps = 0;
  prev_millis = millis();
  attachInterrupt(0, InterruptEncoderAvanzamento, RISING);
  
  while (stay)
  {
    if (millis()-prev_millis >= frame)
    {
      detachInterrupt(0);
      MandrelSpeed = (unsigned int)((float)absolute_encoder_steps*speed_multiplier); //calculates the speed
      // CALCULATION OF SPEED
      // this routine calculates how many steps are done by the mandrel during "inteval" (in milliseconds)
      // the number of steps are multiplied by the "speed_multiplier" which is calculated above in the constant definition
      // the result is the exact speed in round per minute
      absolute_encoder_steps = 0;
      stay = false;
    }
  }
  detachInterrupt(0);
  return MandrelSpeed;
}

/*  OLD GETSPEED FUNCTION
unsigned int GetSpeed (unsigned long frame)       //return the speed with a given TimeSlot in milliseconds.
// The longer the timeslot, the longer the function will take to be executed
{
  float FloatSpeed;
  boolean stay = true;
  boolean EncoderMoved = true;
  unsigned int Speed_mandrino;
  steps = 0;
  unsigned long prev_time = millis();
  while (stay)
  {
    if ((digitalRead(ENCA) == HIGH) && (EncoderMoved == false)) {
      steps++;
      EncoderMoved = true;
    }
    if ((digitalRead(ENCA) == LOW) && (EncoderMoved == true)) EncoderMoved = false;
    if ((prev_time + frame) <= millis()) stay = false;
  }
  FloatSpeed = ((float(steps) / 250) * (1000 / float(frame))) * 60;
  Speed_mandrino = (unsigned int)round(FloatSpeed);
  return Speed_mandrino;
}
*/
