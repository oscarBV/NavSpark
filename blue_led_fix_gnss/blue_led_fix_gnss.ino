/*

 Blue_Led_Fix_GNSS 

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 github URL :-
 https://github.com/oscarBV/NavSpark/tree/master/blue_led_fix_gnss
 
 Després de fer el HELLO WORLD el led blau no feia blink!!!!
 
*/

// Comentar per treure els miisatges de debug
// #define DEBUG

uint8_t gpio_bit = 0;

void setup() {
GnssConf.init();
gnss_gpio_set_output(gpio_bit);
}

void loop()
{
// put your main code here, to run repeatedly:
}

void task_called_after_GNSS_update(void)
{
#ifdef DEBUG
  char buf[128];
  int len;
#endif
U08 fixmode;
static uint8_t val = 0;  
  
GnssInfo.update();
fixmode = GnssInfo.fixMode(); 

#ifdef DEBUG
  len = sprintf(buf, "fix mode: %3d\r\n", fixmode);
  gnss_uart_putline(0, (U08*) buf, len);
#endif

if (fixmode >= 1) {
  if (val == 1) gnss_gpio_high(gpio_bit);
  else gnss_gpio_low(gpio_bit);
} else gnss_gpio_low(gpio_bit);

val = 1 - val; 
}
