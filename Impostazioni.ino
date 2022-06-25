const char IMP0[] PROGMEM = "*** IMPOSTAZIONI ***";
const char IMP1[] PROGMEM = "passi enc. mandrino";
const char IMP2[] PROGMEM = "passi stepper vite";
const char IMP3[] PROGMEM = "passo vite madre";
const char IMP4[] PROGMEM = "rit. accelerazione";
const char IMP5[] PROGMEM = "rit. decelerazione";
const char IMP6[] PROGMEM = "rotazione standard";
const char IMP7[] PROGMEM = "avanzamento carro 1";
const char IMP8[] PROGMEM = "avanzamento trasv 1";
const char IMP9[] PROGMEM = "avanzamento carro 2";
const char IMP10[] PROGMEM = "avanzamento trasv 2";
const char IMP11[] PROGMEM = "avanzamento carro 3";
const char IMP12[] PROGMEM = "avanzamento trasv 3";
const char IMP13[] PROGMEM = "avanzamento carro 4";
const char IMP14[] PROGMEM = "avanzamento trasv 4";
const char IMP15[] PROGMEM = "offset filetto";
const char IMP16[] PROGMEM = "NORTON";
const char IMP17[] PROGMEM = "Vel max stepper";
const char IMP18[] PROGMEM = "taratura";
const char IMP19[] PROGMEM = "ripristina valori";   //<=== FINE MENU'
const char IMP20[] PROGMEM = "SEL: OK";
const char IMP21[] PROGMEM = "ORARIA";              //VALORE PER VOCE IMP[6]
const char IMP22[] PROGMEM = "ANTI-ORARIA";         //VALORE PER VOCE IMP[6]


const char* const MenuImpostazioni[] PROGMEM = {IMP0, IMP1, IMP2, IMP3, IMP4, IMP5, IMP6, IMP7, IMP8, IMP9,
                                                IMP10, IMP11, IMP12, IMP13, IMP14, IMP15, IMP16, IMP17, IMP18,
                                                IMP19, IMP20, IMP21, IMP22 };



void Impostazioni()
{
  int MenuIndex = 1;
  boolean stay = true;
  boolean SEL_pressed = false;
  boolean ESC_pressed = false;
  boolean ANALOG_X_moved = false;
  boolean ANALOG_Y_moved = false;
  int variation;
  char avanz = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  strcpy_P(buff, (char*)pgm_read_word(&(MenuImpostazioni[0])));
  lcd.print(buff);

  while (stay)
  {
    variation = 0;
    stay = true;
    
    strcpy_P(buff, (char*)pgm_read_word(&(MenuImpostazioni[MenuIndex])));
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print(buff);
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(1,2);
    
    switch (MenuIndex) {
      case 1: lcd.print(one_turn_mandrel_steps); break;
      case 2: lcd.print(one_turn_screw_steps); break;
      case 3: lcd.print(screw_pitch, 3); break;
      case 4: lcd.print(AccelerationDelay); break;
      case 5: lcd.print(DecelerationDelay); break;
      case 6: if (CCW) strcpy_P(buff, (char*)pgm_read_word(&(MenuImpostazioni[21])));
              else    strcpy_P(buff, (char*)pgm_read_word(&(MenuImpostazioni[22]))); 
              lcd.print(buff); 
              break;
      case 7: lcd.print(av_carro[0]); break;
      case 8: lcd.print(av_trasv[0]); break;
      case 9: lcd.print(av_carro[1]); break;
      case 10: lcd.print(av_trasv[1]); break;
      case 11: lcd.print(av_carro[2]); break;
      case 12: lcd.print(av_trasv[2]); break;
      case 13: lcd.print(av_carro[3]); break;
      case 14: lcd.print(av_trasv[3]); break;
      case 15: lcd.print(thread_offset_steps); break;
      case 16: lcd.print(NORTON_gearbox+1); break;
      case 17: lcd.print(MaxStepperSpeed); break;
      case 18: strcpy_P(buff, (char*)pgm_read_word(&(MenuImpostazioni[avanz+7]))); lcd.print(buff); break;
      case 19: lcd.print("SEL:ok"); break;
    }
    lcd.setCursor(0, 3);
    lcd.print("UP/DW  -  OK  -  ESC");
    SEL_pressed = false;
    ESC_pressed = false;
    while (stay)
    {
      if (digitalRead(SEL) == PRESSED)
      {
        stay = false;
        SEL_pressed = true;
        lcd.setCursor(10,3);
        lcd.blink();
        lcd.cursor();
        delay(200);
        lcd.noCursor();
        lcd.noBlink();
      }
      if (digitalRead(ESC) == PRESSED)
      {
        stay = false;
        ESC_pressed = true;
        lcd.setCursor(17,3);
        lcd.blink();
        lcd.cursor();
        delay(200);
        lcd.noCursor();
        lcd.noBlink();
      }
      if ((analogRead(ANALOG_X) < LOW_3) || (analogRead(ANALOG_X) > HIGH_3)) stay = false;
      if ((analogRead(ANALOG_Y) < LOW_TOL) || (analogRead(ANALOG_Y) > HIGH_TOL)) stay = false;
    }
    stay = true;
    if (analogRead(ANALOG_X) < LOW_3) 
    {
      MenuIndex--;
      if (MenuIndex == 0) MenuIndex = 18;
      delay(300);
    } 
    if (analogRead(ANALOG_X) > HIGH_3) 
    {
      MenuIndex++;
      if (MenuIndex == 19) MenuIndex = 1;
      delay(300);
    }
    if (SEL_pressed) 
    {
        switch(MenuIndex) {
          case 18: /*Taratura(avanz);*/lcd.setCursor(0,3); lcd.print("....COMING SOON....."); delay(1000); break;
          case 19: lcd.setCursor(0,2);
                   lcd.print("CONFERMA RIPRISTINO?");
                   lcd.setCursor(0,3);
                   lcd.print    ("SEL:ok - ESC:annulla");
                   while (stay)
                   {
                     if (digitalRead(ESC) == PRESSED) stay = false;
                     delay(300);
                     if (digitalRead(SEL) == PRESSED)
                     {
                       lcd.setCursor(0,3);
                       lcd.print(".... RIPRISTINO ....");
                       FactoryDefaultEEPROM();
                       LoadFromEEPROM();
                       stay = false;
                       delay(1000);
                     }
                   }
                   stay = true;
                   break;
          default: {WriteToEEPROM();LoadFromEEPROM();}
        }
    }
    if (ESC_pressed) stay = false;    
    if ((analogRead(ANALOG_Y) < LOW_TOL) && (analogRead(ANALOG_Y) >= LOW_2)) variation = -5;
    if ((analogRead(ANALOG_Y) < LOW_2) && (analogRead(ANALOG_Y) >= LOW_3)) variation = -50;
    if (analogRead(ANALOG_Y) < LOW_3) variation = -100;
    if ((analogRead(ANALOG_Y) > HIGH_TOL) && (analogRead(ANALOG_Y) <= HIGH_2)) variation = 5;
    if ((analogRead(ANALOG_Y) > HIGH_2) && (analogRead(ANALOG_Y) <= HIGH_3)) variation = 50;
    if (analogRead(ANALOG_Y) > HIGH_3) variation = 100;
    if (variation != 0)
    {
      switch (MenuIndex) {
        case 1: one_turn_mandrel_steps += variation; break;
        case 2: one_turn_screw_steps += variation; break;
        case 3: screw_pitch = (((screw_pitch * 1000) + (variation/5)) / 1000); break;
        case 4: AccelerationDelay += (variation * 5); break;
        case 5: DecelerationDelay += (variation * 5); break;
        case 6: CW = !(CW); CCW = !(CCW); break;
        case 7: av_carro[0] += (variation/5); break;
        case 8: av_trasv[0] += (variation/5); break;
        case 9: av_carro[1] += (variation/5); break;
        case 10: av_trasv[1] += (variation/5); break;
        case 11: av_carro[2] += (variation/5); break;
        case 12: av_trasv[2] += (variation/5); break;
        case 13: av_carro[3] += (variation/5); break;
        case 14: av_trasv[3] += (variation/5); break;
        case 15: thread_offset_steps += variation; break;
        case 16: NORTON_gearbox += (variation/50); 
                 if (NORTON_gearbox > 200 ) NORTON_gearbox = 0;
                 if (NORTON_gearbox > 3) NORTON_gearbox = 3;
                 break;  
        case 17: MaxStepperSpeed += variation; break;
        case 18: avanz += (variation/variation); if (avanz == 8) avanz = 0; if (avanz == -1) avanz = 7; break;      
      }
    }
    delay(200);
  }
}
