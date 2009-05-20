#ifndef CONNECTIONH
#define CONNECTIONH
#include <string>


namespace nxt
{

  #define NXT_BUFFER_SIZE 64


  /**
    * Connection type enumeration
    @see Connection#get_type
  */
  enum Connection_type {
    /**
    * Bluetooth connection
    */
    BT = 0x00

  };

  /**
    * Abstract class for connections
  */
  class Connection {
    public:
      virtual ~Connection(){}

      /**
      * Send a byte string
      * (must be implemented in sub class)
      * @param *buffer [a pointer to a buffer that can hold the bytes to send]
      * @param no_bytes [the number of bytes to send]
      */
      virtual void send(unsigned char *buffer,unsigned int no_bytes)=0;

      /**
      * Connect to the NXT
      * (Dummy method does nothing - is implemented in some sub class)
      * @param comport [specify the comport that is to used for the connection between the NXT and PC]
      */
  #if WIN32
      virtual void connect(unsigned int comport){return;}
  #else
      virtual void connect(char *){return;}
  #endif
      /**
      * Disconnect from the NXT
      * (must be implemented in sub class)
      */
      virtual void disconnect()=0;

      /**
      * Receive a byte string
      * (must be implemented in sub class)
      * @param *buffer [a pointer to a buffer that can hold the received bytes]
      * @param length [the number of bytes to receive]
      */
      virtual void receive(unsigned char *buffer, unsigned int length)=0;

      /**
      * Flush the input and output buffer
      * (must be implemented in sub class)
      */
      virtual void flush()=0;

      /**
      * Get the connection type
      * (must be implemented in sub class)
      * @return the connection type
      */
      virtual Connection_type get_type()=0;
    protected:
  };
}
#endif
