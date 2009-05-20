/*
 * SharpIR.h
 *
 *  Created on: Feb 21, 2009
 *      Author: csp
 */

#ifndef SHARPIR_H_
#define SHARPIR_H_

#include "i2c.h"
#if WIN32
#include "windows.h"
#endif
#include "connection.h"


namespace nxt
{
  // power
  #define POWER_ON 0x45
  #define POWER_OFF 0x44
  #define SINGLE_SHOT 0x01

  #define DIST_NXT_SHORT 0x32
  #define DIST_NXT_MED   0x33
  #define DIST_NXT_LONG  0x34

  #define ARPA_ON  0x4E
  #define ARPA_OFF 0x4F // default


  //other registers
  #define DISTNX_ADDRESS 0x02
  #define DIST_DATA_LSB  0x42 // long range
  #define DIST_DATA_MSB  0x43 // medium
  #define SENSOR_MOD_TYPE 0x50
  #define COMMAND 0x41



  using namespace std;

  /**
  * Class for the NXT sonar (ultrasonic) sensor
  */
  class DistNx: public I2c{
    public:
      /**
       * Constructor for the sonar sensor (percent mode and LED on is default)
       * @param port [which sensor port to use]
       * @param *connection [attach a connection]
       * @param mode [set the sonar mode to either METRIC or INCH (default is METRIC)]
      */
      DistNx(Sensor_port port, Connection* connection);
      ~DistNx();

      /**
       * Initiate the sensor
      * @param reply [true = require reply from NXT; false = no reply from NXT]
      */
      void init(bool reply=false);

      /**
      * Read the sensor value
      * (init method will be called if the sensor has not been initialized)
      * @return the sensor reading in either inches or centimeters
      */
      int read();

      /**
       * Set the return value to be either inch or centimeter
       * @param value [use either METRIC or INCH to indicate the return value of read()]
       * @see Sonar_mode
      */
      void set(unsigned int value);

      /**
       * Get the sensor reading as a string
       * (init method will be called if the sensor has not been initialized)
       * @return sensor reading as a string + either " CM" or " inch(s)"
      */
      string print();

      /**
       * Get the sensor type
       * @return either SONAR_METRIC or SONAR_INCH
      */
      Sensor_type get_type();


    protected:

      void off();
      void set_mode(int mode);
      void write_register(int reg_adr, int value);
      int  read_register(int reg_adr);
      void sonar_reset();
      //void set_sensor_mode(int mode);
  };
}


#endif /* SHARPIR_H_ */
