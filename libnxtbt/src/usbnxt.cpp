/*
 * usbnxt.cpp
 *
 *  Created on: Feb 15, 2009
 *      Author: csp
 */

#include "usbnxt.h"
#include <stdlib.h>
#include "error.h"


using namespace std;
using namespace nxt;



UsbCon::UsbCon(){
  usb_init();

}

UsbCon::~UsbCon(){
  disconnect();
};



void UsbCon::send(unsigned char *buffer, unsigned int num_bytes)
{

  unsigned int i=0;
  byte = ( char *) malloc(num_bytes*sizeof( char));
  while(i<num_bytes){
    byte[i]=buffer[i];
    //printf("send: %d\n ",buffer[i]);
    i++;
  }

  int ret = usb_bulk_write(_hdl, 0x1, byte, num_bytes, 0);

  free(byte);
  if (ret < 0)
    std::cerr << " WRITE FAILED : check permissions " << std::endl;
    return;

  return;
}


// connection helpers
void UsbCon::nxt_open()
{
  char buf[2];
  int ret;
  _hdl = usb_open(_dev);
  //ret = usb_reset(_hdl);

  ret = usb_set_configuration(_hdl,0);
  std::cerr << " SET CFG " << ret  << std::endl;
  if (ret < 0)
  {
    std::cerr << " CFG ERROR" << std::endl;
    disconnect();
    throw Nxt_exception::Nxt_exception("connect"," NXT CONFIG ERROR", UNABLE_TO_CONNECT);
    return;
  }
  ret = usb_detach_kernel_driver_np(_hdl, 0);
  std::cerr << " RESET " << ret  << std::endl;

  ret = usb_claim_interface(_hdl, 0);

#if 1
  if (ret < 0)
  {
    printf("%d\n", ret);
    disconnect();
    throw Nxt_exception::Nxt_exception("connect"," NXT BUSY", UNABLE_TO_CONNECT);
    return;
  }
#endif
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;

  command[2]=0x01;
  command[3]=0x88;
  try{
    std::cerr << " sending " << std::endl;
    this->send(&command[0],4);
    std::cerr << " rec " << std::endl;
    this->receive(&answer[0],9);
  }
  catch (Nxt_exception& e){
    std::cerr << " HANDSHAKE FAILED " << std::endl;
    this->disconnect();
    throw Nxt_exception::Nxt_exception("connect","USB", UNABLE_TO_CONNECT);
  }

  return;
}

void UsbCon::connect(char *btaddr)
{
  // scan for it


  struct usb_bus *busses, *bus;

  usb_find_busses();
  usb_find_devices();

  busses = usb_get_busses();
  bool hasNxt = false;
  for (bus = busses; bus != NULL; bus = bus->next)
  {
   struct usb_device *dev;

   for (dev = bus->devices; dev != NULL; dev = dev->next)
   {

     if (dev->descriptor.idVendor == VENDOR_ATMEL &&
         dev->descriptor.idProduct == PRODUCT_SAMBA)
     {
       _dev = dev;
       // RESET MODE FOR FLASHING
       hasNxt = true;
       break;
     }
     else if (dev->descriptor.idVendor == VENDOR_LEGO &&
              dev->descriptor.idProduct == PRODUCT_NXT)
     {
       _dev = dev;
       // happy
       hasNxt = true;
       break;
     }
   }
  }
  if (hasNxt) {

    std::cerr << " NXT COMM OK " << std::endl;
    nxt_open();


  } else {
    std::cerr<< " NO NXT USB FOUND " << std::endl;
  }

}



/**
* Disconnect from the NXT
*/
void UsbCon::disconnect()
{
  std::cerr << " CLOSING USB " << std::endl;
  usb_release_interface(_hdl, 0);
  usb_close(_hdl);

}

/**
* Receive a byte string
* @param *buffer [a pointer to a buffer that can hold the received bytes]
* @param length [the number of bytes to receive]
*/
void UsbCon::receive(unsigned char *buffer, unsigned int length)
{

  unsigned int i = 0;

  byte = (char *) malloc(length*sizeof(char));
  number_bytes = 0;
  buffer[0]='\0';
  std::cerr << " getting " << std::endl;
  int ret = usb_bulk_read(_hdl, 0x82, byte, length, 0);
  if (ret < 0)
    throw Nxt_exception::Nxt_exception("receive","usb", NO_REPLY);
  number_bytes = length;
  i=0;
  while(i<number_bytes){
    buffer[i]=byte[i];
    printf("Receive: %d\n ",buffer[i]);
    i++;
  }
  buffer[i]='\0';

}

/**
* Flush the input and output buffer
*/
void UsbCon::flush()
{}

/**
* Get the connection type
* @return BT = UsbCon (see the CONNECTION_TYPE enumeration)
*/
Connection_type UsbCon::get_type()
{}
