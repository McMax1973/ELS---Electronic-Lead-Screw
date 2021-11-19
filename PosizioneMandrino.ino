const char PA0[] PROGMEM = "POSIZIONE ANGOLARE";
const char PA1[] PROGMEM = "gradi:";
const char PA2[] PROGMEM = "RESET            ESC";
const char* const MenuPosizioneAngolare[] PROGMEM = {PA0, PA1, PA2};


void InterruptEncoder()              //this is the Interrupt Service Routine which manage the encoder inputs
{
  static byte prev_encoder;          // this variable stores the value of the input port register of the previous encoder state
  byte port;                         // this variable stores the value of the input port register of the current encoder state

  port = PIND & B00001100;           //reads input pins 2 and 3 of the Arduino uno PORTD D and puts the binary image on the "port" byte variable
  port |= prev_encoder;              //makes the logic "OR" between the current "port" byte and the previous
  steps = steps + encoder[port];     //increment/decrement the encoder couter according to the previous and current encoder values
  port >>= 2;                        //shift sthe port image by 2 bits on right
  prev_encoder = port;               //assign the current port image to the "prev_encoder" byte by keeping just the bits n. 2 and 3
}



void PosizioneAngolare() // this function displays the angular position of the mandrel
{
  float Degrees = 0.00;                                          //variable used to store degrees
  float old_Degrees =0.01;                                       //variable to check if the position is changed. Set different from Degrees to print LCD output on the first cycle 
  const float AngularRatio = 360/float(one_turn_mandrel_steps);  //calculates the degrees per single mandrel encoder step
  
  
  lcd.clear();
  
  strcpy_P(buff, (char*)pgm_read_word(&(MenuPosizioneAngolare[0])));
  lcd.setCursor(1,0);
  lcd.print(buff);
  
  strcpy_P(buff, (char*)pgm_read_word(&(MenuPosizioneAngolare[1])));
  lcd.setCursor(1,2);
  lcd.print(buff);
  
  strcpy_P(buff, (char*)pgm_read_word(&(MenuPosizioneAngolare[2])));
  lcd.setCursor(0,3);
  lcd.print(buff);
  
  lcd.setCursor(8,2);
  
  attachInterrupt(0, InterruptEncoder, CHANGE);
  attachInterrupt(1, InterruptEncoder, CHANGE);
  
  while (digitalRead(ESC) != PRESSED)
  {
    if (digitalRead(RESET) == PRESSED)
      steps = 0;
    Degrees = ((float)((abs(steps)%one_turn_mandrel_steps))*AngularRatio);  
    if (steps < 0) Degrees = 360 - Degrees;
    if (Degrees == 360) Degrees = 0;  
    if (Degrees != old_Degrees)
      {
        lcd.setCursor(8,2);
        lcd.print(Degrees);
        lcd.print("   ");
        old_Degrees = Degrees;
      } 
  }  
  detachInterrupt(0);
  detachInterrupt(1);  
}
