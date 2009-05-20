#ifndef BLUETOOTH
#define BLUETOOTH
#ifdef WIN32
  #include <windows.h>
#else
  #include <fcntl.h>
  #include <sys/param.h>
  #include <sys/ioctl.h>
  #include <sys/socket.h>

  #include <bluetooth/bluetooth.h>
  #include <bluetooth/hci.h>
  #include <bluetooth/hci_lib.h>
  #include <bluetooth/rfcomm.h>
#endif
#include <iostream>
#include <string>
#include "connection.h"


namespace nxt
{

  #define NXT_RET         0x0  /* direct command, reply required */
  #define NXT_SYS         0x1  /* system command, reply required */
  #define NXT_REPLY       0x2  /* reply command */
  #define NXT_NORET       0x80 /* Direct command, reply not required */
  #define NXT_SYSOP       0x81 /* */

  /**
    * Class for Bluetooth communication
  */
  class Bluetooth : public Connection{
    public:
      /**
       * Constructor for Bluetooth connection
       */
      Bluetooth();
      ~Bluetooth();
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
  #if WIN32
      void connect(unsigned int comport);
  #else
      int  readySocket( const int mode, const int timeout,  bool *canRead, bool *canWrite );
      void connect(const char *btaddr);
  #endif

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
      * @return BT = bluetooth (see the CONNECTION_TYPE enumeration)
      */
      Connection_type get_type();
      //unsigned int get_port();
    protected:
  #if WIN32
      /**
      * The comport handle
      */
      HANDLE handle;

      /**
      * Byte pointer used to send/recieve data
      */
      BYTE *byte;

      /**
      * nunmber of bytes recieved/send
      */
      DWORD number_bytes;

      /**
      * structure containing the time-out parameters for the comport
      */
      COMMTIMEOUTS timeout;

      /**
      * structure that defines the control setting for the comport
      */
      DCB dcb;
  #else

      unsigned char *byte;

          /**
          * nunmber of bytes recieved/send
          */
      int number_bytes;


      int devcount;
      int nxtsock;


  #endif
      unsigned int comport;
  };
}
#endif
