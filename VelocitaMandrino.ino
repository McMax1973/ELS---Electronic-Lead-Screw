const char VM0[] PROGMEM = " VELOCITA' MANDRINO";
const char VM1[] PROGMEM = "giri al min:";
const char* const MenuVelocitaMandrino[] PROGMEM = {VM0, VM1};


void InterruptEncoderSpeed()  //this is the Interrupt Service Routine which manage the encoder inputs while feeding (250 step/turn)
                                    //activate this interrupt only with RISING on Encoder A channel (INT0)
{
  absolute_encoder_steps ++;
}


void MandrelSpeed()
{
  boolean stay = true;                    //boolean to determine if stay or not in the reading cycle
  unsigned int MandrelSpeed = 0;          //speed of the mandrel calculated
  unsigned long prev_millis = 0;          //used to store the millis alue for the speed count routine
  const unsigned int interval = 100;       //interval in milliseconds to measure the speed
  const float speed_multiplier = ((1000/(float)interval)/((float)one_turn_mandrel_steps/4))*60; //this is the multiplier used to calculate the speed
  // this is the multiplier used by to calculate the speed
  // one_turn_mandrel_steps is the total number of encoder steps per every single mandrel turn
  // the interrupt used in this routine is triggered only on one edge of one channel of the encoder, therefore it taked only 1 step every 4.
  // 1000/(float)interval: 1000msec (1sec) divided by the interval gives the ratio of the interval with respect to 1 second
  // (float)one_turn_mandrel_steps/4): the number of steps in one turn are divided by 4 (due to interrupt explained above)
  // (1000/(float)interval)/((float)one_turn_mandrel_steps/4): this therefore gives the number of turn per seconds, which are then multiplied by 60 to get the speed in RPM
    

  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuVelocitaMandrino[0])));
  lcd.setCursor(0,0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuVelocitaMandrino[1])));
  lcd.setCursor(0,2);
  lcd.print(buff);

  absolute_encoder_steps = 0;
  prev_millis = millis();
  attachInterrupt(0, InterruptEncoderSpeed, RISING);
  
  while (stay)
  {
    if (digitalRead(ESC) == PRESSED) stay = false;
    if (millis()-prev_millis >= interval)
    {
      detachInterrupt(0);
      MandrelSpeed = (unsigned int)((float)absolute_encoder_steps*speed_multiplier); //calculates the speed
      // CALCULATION OF SPEED
      // this routine calculates how many steps are done by the mandrel during "inteval" (in milliseconds)
      // the number of steps are multiplied by the "speed_multiplier" which is calculated above in the constant definition
      // the result is the exact speed in round per minute
      lcd.setCursor (13,2);
      lcd.print(MandrelSpeed);
      lcd.print("  ");
      absolute_encoder_steps = 0;
      prev_millis = millis();
      attachInterrupt(0, InterruptEncoderSpeed, RISING);
    }
  }
  detachInterrupt(0);
}

/*
void Old_MandrelSpeed()
{
  boolean stay = true;
  unsigned long prev_millis = 0;
  float MandrelSpeed = 0.00;
  long prev_absolute_encoder_steps = absolute_encoder_steps;
  unsigned int StepSlot;
  unsigned int MillisSlot;
  
  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuVelocitaMandrino[0])));
  lcd.setCursor(0,0);
  lcd.print(buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuVelocitaMandrino[1])));
  lcd.setCursor(1,2);
  lcd.print(buff);
  
  attachInterrupt(0, InterruptEncoderSpeed, RISING);
  prev_millis = millis();
  
  while (stay)
  {
    if (digitalRead(ESC) == PRESSED) stay = false;
    StepSlot = abs(absolute_encoder_steps - prev_absolute_encoder_steps);
    MillisSlot = millis() - prev_millis;
    if (StepSlot > 50)
    {
      MandrelSpeed = ((float(StepSlot)/250)*(1000/float(MillisSlot)))*60;
      prev_absolute_encoder_steps = absolute_encoder_steps;
      prev_millis = millis();
      lcd.setCursor (14,2);
      lcd.print(MandrelSpeed,1);
      lcd.print("  ");
    }
    if (MillisSlot > 1000) {lcd.setCursor (14,2); lcd.print(0,1); lcd.print("    ");}
  }
  detachInterrupt(0);
}
*/
