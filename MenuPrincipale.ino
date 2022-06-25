const char MP0[] PROGMEM = "* MENU  PRINCIPALE *";
const char MP1[] PROGMEM = "Impostazioni";
const char MP2[] PROGMEM = "Avanzamento";
const char MP3[] PROGMEM = "Filettatura";
const char MP4[] PROGMEM = "Movimento Libero";
const char MP5[] PROGMEM = "Velocita' mandrino";
const char MP6[] PROGMEM = "Posizione angolare";
const char* const MenuPrincipale[] PROGMEM = {MP0, MP1, MP2, MP3, MP4, MP5, MP6};


char Principale()
{
  static char current;
  char previous = 6;
  char next = 2;
  boolean stay = true;
  boolean innerstay = true;
  
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print(">");
  lcd.setCursor(19,2);
  lcd.print("<");
  strcpy_P(buff, (char*)pgm_read_word(&(MenuPrincipale[0])));
  lcd.setCursor(0,0);
  lcd.print(buff);
  
  while (stay)
  {
    switch (current) {
      case 0: current = 6; break;
      case 7: current = 1; break;
    }  
    previous = current - 1;
    next = current +1;
    if (previous == 0)
      previous = 6;
    if (next == 7)
      next = 1;
      
    strcpy_P(buff, (char*)pgm_read_word(&(MenuPrincipale[previous])));
    lcd.setCursor(1,1);
    lcd.print("                  ");
    lcd.setCursor(1,1);
    lcd.print(buff);
    
    strcpy_P(buff, (char*)pgm_read_word(&(MenuPrincipale[current])));
    lcd.setCursor(1,2);
    lcd.print("                  ");
    lcd.setCursor(1,2);
    lcd.print(buff);
    
    strcpy_P(buff, (char*)pgm_read_word(&(MenuPrincipale[next])));
    lcd.setCursor(1,3);
    lcd.print("                  ");
    lcd.setCursor(1,3);
    lcd.print(buff);
    
    innerstay = true;
    while (innerstay)
    {      
      if (digitalRead(SEL) == PRESSED) { stay = false; innerstay = false; }
      if (analogRead(ANALOG_Y) < LOW_1) { current -= 1; innerstay = false; }
      if (analogRead(ANALOG_Y) > HIGH_1) { current += 1; innerstay = false; }
    } 
    
    delay (300);
  }
  
  return current;  
}
