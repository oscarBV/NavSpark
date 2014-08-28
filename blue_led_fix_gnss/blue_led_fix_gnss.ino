#define debug 1

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
static uint8_t val = 0;  
char buf[128];
int len;
U08 fixmode;


GnssInfo.update();
fixmode = GnssInfo.fixMode(); 
len = sprintf(buf, "fix mode: %3d\r\n", fixmode);
gnss_uart_putline(0, (U08*) buf, len);

if (fixmode >= 1) {
  if (val == 1) gnss_gpio_high(gpio_bit);
  else gnss_gpio_low(gpio_bit);
} else gnss_gpio_low(gpio_bit);

val = 1 - val; 
}
