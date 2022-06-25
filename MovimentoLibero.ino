const char ML0[] PROGMEM = "* MOVIMENTO LIBERO *";
const char ML1[] PROGMEM = "<< - < ------ > - >>";
const char ML2[] PROGMEM = "posizione";
const char ML3[] PROGMEM = "RESET            ESC";
const char* const MenuMovimentoLibero[] PROGMEM = {ML0, ML1, ML2, ML3};



void MovimentoLibero()        //moves the stepper motor freely using the joystick. It returns the carriage position calculated on the actual NORTON gearbox ratio
                              
{
  boolean stay = true;
  unsigned long stepperSpeed;
  float Posizione = 0.00;
  unsigned int joy;
  byte NORTON_feed;
    
  ClearPWM();
  Speed = 0;
  lcd.clear();
  strcpy_P(buff, (char*)pgm_read_word(&(MenuMovimentoLibero[0])));
  lcd.setCursor(0,0);
  lcd.print (buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuMovimentoLibero[1])));
  lcd.setCursor(0,1);
  lcd.print (buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuMovimentoLibero[2])));
  lcd.setCursor(0,2);
  lcd.print (buff);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuMovimentoLibero[3])));
  lcd.setCursor(0,3);
  lcd.print (buff);
  SetPWM();
  delay(200);
  if (CarroTrasv) NORTON_feed = av_carro[NORTON_gearbox]; else NORTON_feed = av_trasv[NORTON_gearbox];
  
  while (stay)
  {
    joy = analogRead(ANALOG_X);
    if (joy >= HIGH_TOL) { stepperSpeed = joy - HIGH_TOL; Direction = CCW;}
    if (joy <= LOW_TOL) { stepperSpeed = LOW_TOL - joy; Direction = CW;}
    if ((joy >= LOW_TOL) && (joy <=HIGH_TOL)) stepperSpeed = 0;
    if (digitalRead(DIR) != Direction) {if (Speed == 0) digitalWrite(DIR, Direction); delayMicroseconds(10);}
    if (digitalRead(ESC) == PRESSED) stay = false;
    if (digitalRead(DIR) == Direction) StepperRunToSpeed(stepperSpeed); else StepperRunToSpeed(0);
    if (digitalRead(RESET) == PRESSED) absolute_steps = 0;
    delay (10);
    Posizione = ((float(absolute_steps)/one_turn_screw_steps)*float(NORTON_feed))/100;
    lcd.setCursor(10,2);
    lcd.print(Posizione,2);
    lcd.print(" mm ");
  }
  ClearPWM();
}
