/*
 * usbnxt.h
 *
 *  Created on: Feb 15, 2009
 *      Author: csp
 */

#ifndef USBNXT_H_
#define USBNXT_H_

#include <iostream>
#include <string>
#include <usb.h>

#include "connection.h"


namespace nxt
{

  #define NXT_RET         0x0  /* direct command, reply required */
  #define NXT_SYS         0x1  /* system command, reply required */
  #define NXT_REPLY       0x2  /* reply command */
  #define NXT_NORET       0x80 /* Direct command, reply not required */
  #define NXT_SYSOP       0x81


  enum nxt_usb_ids {
    VENDOR_LEGO   = 0x0694,
    VENDOR_ATMEL  = 0x03EB,
    PRODUCT_NXT   = 0x0002,
    PRODUCT_SAMBA = 0x6124
  };




  /**
    * Class for UsbCon communication
  */
  class UsbCon : public Connection{
    public:
      /**
       * Constructor for UsbCon connection
       */
      UsbCon();
      ~UsbCon();
      /**
      * Send a byte string
      * @param *buffer [a pointer to a buffer that can hold the bytes to send]
      * @param num_bytes [the number of bytes to send]
      */
      void send(unsigned char *buffer, unsigned int num_bytes);

      /**
      * Connect to the NXT
      * @param comport [specify the comport that is to used for the connection between the NXT and PC]
      */

      void connect(char *btaddr);

      void nxt_open();

      /**
      * Disconnect from the NXT
      */
      void disconnect();

      /**
      * Receive a byte string
      * @param *buffer [a pointer to a buffer that can hold the received bytes]
      * @param length [the number of bytes to receive]
      */
      void receive(unsigned char *buffer, unsigned int length);

      /**
      * Flush the input and output buffer
      */
      void flush();

      /**
      * Get the connection type
      * @return BT = UsbCon (see the CONNECTION_TYPE enumeration)
      */
      Connection_type get_type();
      //unsigned int get_port();
    protected:
      char *byte;
      int number_bytes;
      usb_dev_handle * _hdl;
      struct usb_device *_dev;




      unsigned int comport;
  };
}


#endif /* USBNXT_H_ */
