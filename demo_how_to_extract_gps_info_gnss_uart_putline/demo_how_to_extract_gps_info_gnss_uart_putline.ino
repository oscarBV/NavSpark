#include "sti_gnss_lib.h"
#include "GNSS.h"

uint8_t gpio_bit = 0;

void setup() {
  // put your setup code here, to run once:
  // GnssConf.setNavMode(STGNSS_NAV_MODE_AUTO);
  GnssConf.setNavMode(STGNSS_NAV_MODE_MARINE);
  GnssConf.setUpdateRate(STGNSS_POSITION_UPDATE_RATE_1HZ);
  GnssConf.setDopMaskMode(STGNSS_DOP_MASK_AUTO);
  GnssConf.setPdopMask(30.0);
  GnssConf.setHdopMask(30.0);
  GnssConf.setGdopMask(30.0);
  GnssConf.init(); /* do initialization for GNSS */
  gnss_gpio_set_output(gpio_bit);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
  NOTE: "task_called_after_GNSS_update()" will be called about every second
        (for 1Hz update rate), so we display the info. here.
*/
void task_called_after_GNSS_update(void)
{
  char buf[32];
  int len;
  static uint8_t val = 0;  
  
  uint16_t prnList[STGNSS_GPS_NCHAN];

  // Get info. of setallite
  GnssInfo.update();
  if (GnssInfo.isUpdated() == true) {

    // display the date from GNSS
    GnssInfo.date.formatString(buf);
    len = sprintf(buf, "%s", buf);
    gnss_uart_putline(0, (U08*) buf, len);
    
    // display the time from GNSS
    GnssInfo.time.formatString(buf);
    len = sprintf(buf, " %s UTC\r\n", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    U08 fixmode = GnssInfo.fixMode(); 
    len = sprintf(buf, "Fix Mode: %3d\r\n", fixmode);
    gnss_uart_putline(0, (U08*) buf, len);
    if (fixmode >= 1) {
      if (val == 1) gnss_gpio_high(gpio_bit);
      else gnss_gpio_low(gpio_bit);
    } else gnss_gpio_low(gpio_bit);
    val = 1 - val; 

    // display how many satellites are in the sky and how many of them are
    // used for position fix
    sprintf(buf, "NumGPSInView = %2d, NumGPSInUse = %2d\r\n",
      GnssInfo.satellites.numGPSInView(prnList),
      GnssInfo.satellites.numGPSInUse(NULL));
    len = sprintf(buf, "%s", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    // an example to show the info. of 1st satellite in prnList
    sprintf(buf, "Satellite %d: elv = %2d, azi = %3d, CNR = %d\r\n",
    	prnList[0],
      GnssInfo.satellites.elevation(CONSTELLATION_GPS, prnList[0]),
      GnssInfo.satellites.azimuth(CONSTELLATION_GPS, prnList[0]),
      GnssInfo.satellites.CNR(CONSTELLATION_GPS, prnList[0]));
    len = sprintf(buf, "%s", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    sprintf(buf, "NumGLONASSInView = %2d, NumGLONASSInUse = %2d\r\n",
      GnssInfo.satellites.numGLNInView(prnList),
      GnssInfo.satellites.numGLNInUse(NULL));
    len = sprintf(buf, "%s", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    sprintf(buf, "Satellite %d: elv = %2d, azi = %3d, CNR = %d\r\n",
    	prnList[0],
      GnssInfo.satellites.elevation(CONSTELLATION_GLONASS, prnList[0]),
      GnssInfo.satellites.azimuth(CONSTELLATION_GLONASS, prnList[0]),
      GnssInfo.satellites.CNR(CONSTELLATION_GLONASS, prnList[0]));
    len = sprintf(buf, "%s", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    // display the longitude
    GnssInfo.location.longitude_formatString(buf);
    len = sprintf(buf, "%s ,", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    GnssInfo.location.latitude_formatString(buf);
    len = sprintf(buf, "%s \r\n", buf);
    gnss_uart_putline(0, (U08*) buf, len);

    double my_longitude = GnssInfo.location.longitude();
    double my_latitude = GnssInfo.location.latitude();
    len = sprintf(buf, "%f , %f \r\n", my_latitude, my_longitude);
    gnss_uart_putline(0, (U08*) buf, len);

    // display the altitude in meters
    sprintf(buf, "height = %.2f(m), ", GnssInfo.altitude.meters());
    len = sprintf(buf, "%s ,", buf);
    gnss_uart_putline(0, (U08*) buf, len);


    // display the course in degree
    sprintf(buf, "course = %.2f (deg), ", GnssInfo.course.deg());
    len = sprintf(buf, "%s ,", buf);
    gnss_uart_putline(0, (U08*) buf, len);


    // display the speed in KM per hour
    sprintf(buf, "speed = %.2f (km/s)\r\n", GnssInfo.speed.kph());
    len = sprintf(buf, "%s\r\n", buf);
    gnss_uart_putline(0, (U08*) buf, len);

  }
}
