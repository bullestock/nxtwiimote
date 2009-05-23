#include "bluetooth.h"
#include <stdlib.h>
#include "error.h"


using namespace std;
using namespace nxt;



Bluetooth::Bluetooth(){
#if WIN32
  this->comport = 0;
#else
  this->nxtsock = 0;
#endif
}

Bluetooth::~Bluetooth(){
  disconnect();
};

#if WIN32

void Bluetooth::send(unsigned char *buffer, unsigned int num_bytes){
  unsigned int i=0;
  if (handle == INVALID_HANDLE_VALUE){
    throw Nxt_exception::Nxt_exception("send","Bluetooth", INVALID_COM_PORT);
  }
  byte = (BYTE *) malloc(num_bytes*sizeof(BYTE));
  while(i<num_bytes){
    byte[i]=buffer[i];
    i++;
  }
  if(!WriteFile(handle, byte, num_bytes, &number_bytes, NULL)!=0){
    free(byte);
    throw Nxt_exception::Nxt_exception("send","Bluetooth", ERROR_WRITING_COM_PORT);
  }
  if(number_bytes!= num_bytes){
    free(byte);
    throw Nxt_exception::Nxt_exception("send","Bluetooth", ERROR_WRITING_COM_PORT);
  }
  free(byte);
  return;
}

void Bluetooth::connect(unsigned int comport){
  /***************************************************************************
  *** See http://www.robbayer.com/files//serial-win.pdf for dokumentation  ****
  ***************************************************************************/
  std::stringstream temp;
  temp << comport;
  string port = "\\\\.\\COM" + temp.str();
  handle = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0,0,OPEN_EXISTING,0,0);
  if (handle == INVALID_HANDLE_VALUE){
    throw Nxt_exception::Nxt_exception("connect","Bluetooth", INVALID_COM_PORT);
  }
  this->comport = comport;
  dcb.ByteSize = 8;//sets bit-size
  dcb.BaudRate = CBR_9600;//sets baudrate
  //Sets timeout for reading - 0 = deactivate

  timeout.ReadIntervalTimeout=3000;
  timeout.ReadTotalTimeoutConstant=0;
  timeout.ReadTotalTimeoutMultiplier=200;

  timeout.WriteTotalTimeoutConstant=3000;
  timeout.WriteTotalTimeoutMultiplier=200;
  SetCommTimeouts(handle, &timeout);
  dcb.Parity=0;
  SetCommState(handle, &dcb);
  flush();
  //Try to read firmware check if a NXT is at the other end
  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;

  command[2]=0x01;
  command[3]=0x88;
  try{
    this->send(&command[0],4);
    this->receive(&answer[0],9);
  }
  catch (Nxt_exception& e){
    this->disconnect();
    throw Nxt_exception::Nxt_exception("connect","Bluetooth", UNABLE_TO_CONNECT);
  }
  return;
}


void Bluetooth::disconnect(){
  if(handle!=INVALID_HANDLE_VALUE){
    CloseHandle(handle);
    handle=INVALID_HANDLE_VALUE;
  }
  this->comport=0;
  return;
}

void Bluetooth::receive(unsigned char *buffer, unsigned int length){
  unsigned int i=0;
  byte = (BYTE *) malloc(length*sizeof(BYTE));
  number_bytes=0;
  buffer[0]='\0';
  if(ReadFile(handle, byte, length, &number_bytes, NULL) == 0){
    free(byte);
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", ERROR_READING_COM_PORT);
  }
  //printf("Reveive length %d No of bytes %d\n",length, nrBytes);
  if(number_bytes==0 && length != 0){
    free(byte);
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NO_REPLY);
  }
  i=(unsigned int) number_bytes;
  //printf("No bytes received:%d\n", i);
  if(number_bytes<length){
    free(byte);
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NOT_ENOUGH_BYTES);
  }
  i=0;
  while(i<number_bytes){
    buffer[i]=byte[i];
    //printf("Receive: %d\n ",buffer[i]);
    i++;
  }
  buffer[i]='\0';
  free(byte);
  return;
}

void Bluetooth::flush(){
  PurgeComm(handle, PURGE_RXCLEAR|PURGE_TXCLEAR);
}

#else
// LINUX
bool dodebug = true;

void Bluetooth::connect(const char *btaddr)
{
  struct sockaddr_rc addr = { 0 };
  char dest[18];

  memcpy(&dest, btaddr, 18);
  // allocate a socket
  nxtsock = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  // set the connection parameters (who to connect to)
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = (uint8_t) 1;
  str2ba( dest, &addr.rc_bdaddr );

  int status = ::connect(nxtsock, (struct sockaddr *)&addr, sizeof(addr));
  if( status == 0 ) {
    if (dodebug) printf("rfcomm connection to %s good.\n", dest);
  }
  if( status < 0 ) {
    perror("rfcomm failed: ");
    throw Nxt_exception::Nxt_exception("connect", "Bluetooth", UNABLE_TO_CONNECT);
  }

  //rv = fcntl(nxtsock, F_SETFD, FD_CLOEXEC);
  int rv = fcntl(nxtsock, F_SETFL, O_NONBLOCK);
  if (rv == 0) {
    if (dodebug) { printf("nonblock set\n"); }
  } else {
    perror("nonblock failed: ");
    throw Nxt_exception::Nxt_exception("connect", "Bluetooth", UNABLE_TO_CONNECT);
  }

  unsigned char answer[NXT_BUFFER_SIZE];
  unsigned char command[4];
  command[0]=0x02;  //command length
  command[1]=0x00;

  command[2]=0x01;
  command[3]=0x88;
#if 1
  try{
    std::cerr << " CHECK CONNECTION " << std::endl;
    this->send(&command[0],4);
    this->receive(&answer[0],9);
    std::cerr << " CHECK CONNECTION DONE" << std::endl;
  }
  catch (Nxt_exception& e){
    this->disconnect();
    throw Nxt_exception::Nxt_exception("connect","Bluetooth", UNABLE_TO_CONNECT);
  }
#endif
  return;
}

void Bluetooth::send(unsigned char *buffer, unsigned int num_bytes)
{
  bool doWrite = true;
  bool doRead  = false;
  int status = readySocket(0, 1000, &doRead, &doWrite);

  number_bytes = ::write(nxtsock, buffer, num_bytes);

  if(number_bytes!= num_bytes){
      cout << "Wrote only " << number_bytes << " of " << num_bytes << endl;
    throw Nxt_exception::Nxt_exception("send","Bluetooth", ERROR_WRITING_COM_PORT);
  }
}



void Bluetooth::disconnect(){
  close(nxtsock);
}

#if 1




int Bluetooth::readySocket( const int mode, const int timeout,  bool *canRead, bool *canWrite )
{
  fd_set write_fds;
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);

  if (mode == 0) {
     FD_SET(nxtsock, &read_fds);
  }

  if (mode == 1) {
     FD_SET(nxtsock, &write_fds);
  }


#if 0
  struct timeval tv;
  tv.tv_sec = timeout;
  tv.tv_usec = 0;
#else
  struct timeval tv= {0,2};
#endif
  int status;

  while  (status = ::select(nxtsock+1, &read_fds, &write_fds, 0, &tv) < 0) {
      printf("socket select got nothing\n");
  }


  if (status <= 0)
    return status;

  if (canRead)
    *canRead = FD_ISSET(nxtsock, &read_fds);

  if (canWrite)
    *canWrite = FD_ISSET(nxtsock, &write_fds);

  return status;
}






void Bluetooth::receive(unsigned char *buffer, unsigned int length)
{

  bool doWrite = false;
  bool doRead  = true;
  int status = readySocket(0, 20 ,&doRead, &doWrite );
  //std::cerr << " SELECT got status : "<< status << std::endl;

  unsigned int i=0;
  int xr = 0;
  byte = (unsigned char *) malloc(length*sizeof(unsigned char));
  number_bytes = 0;
  buffer[0]='\0';
  //std::cerr<< " GOT SOCKET Status " << status << " READ " << doRead << std::endl;

  do {
      xr = ::read(nxtsock, byte, length);
  } while (xr == -1 );


  number_bytes = xr;
  //std::cerr << " GOT N " << xr << std::endl;

  if(number_bytes==0 && length != 0){
    free(byte);
    std::cerr << " REC ERROR " << __LINE__ << std::endl;
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NO_REPLY);
  }

  i=(unsigned int) number_bytes;
  //printf("No bytes received:%d\n", i);
  if(number_bytes<length){
    free(byte);
    std::cerr << " REC ERROR " << __LINE__ << std::endl;
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NOT_ENOUGH_BYTES);
  }
  i=0;
  while(i<number_bytes){
    buffer[i]=byte[i];
    //printf("Receive: %d\n ",buffer[i]);
    i++;
  }
  buffer[i]='\0';
  //}
  free(byte);
  return;

}


#else
void Bluetooth::receive(unsigned char *buffer, unsigned int length){
  fd_set set;
  int maxfd = nxtsock;
  FD_ZERO(&set);
  FD_SET(nxtsock, &set);
  struct timeval timeout = {0,2};
  unsigned int i=0;

  byte = (unsigned char *) malloc(length*sizeof(unsigned char));
  number_bytes = 0;
  buffer[0]='\0';

  while  (select(maxfd, &set, 0, 0, &timeout) < 0) {
    printf("got nothing\n");
  }


  int err = 0;
  unsigned int rbytes =  0;
  std::cout << " Bluetooth::receive " << length << std::endl;
  while (rbytes < length) {
    number_bytes = read(nxtsock, byte, (int)length);
    if (number_bytes == -1) throw Nxt_exception::Nxt_exception("receive","Bluetooth", NO_REPLY);
    rbytes += number_bytes;
  }




  std::cout << " receiving done " << number_bytes << std::endl;

  if(number_bytes==0 && length != 0){
    free(byte);
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NO_REPLY);
  }

  //printf("Reveive length %d No of bytes %d\n",length, nrBytes);
  if(number_bytes==0 && length != 0){
    free(byte);
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NO_REPLY);
  }

  i=(unsigned int) number_bytes;
  //printf("No bytes received:%d\n", i);
  if(number_bytes<length){
    free(byte);
    throw Nxt_exception::Nxt_exception("receive","Bluetooth", NOT_ENOUGH_BYTES);
  }
  i=0;
  while(i<number_bytes){
    buffer[i]=byte[i];
    //printf("Receive: %d\n ",buffer[i]);
    i++;
  }
  buffer[i]='\0';
  free(byte);
  std::cerr << " RECEIVE RETURN " << std::endl;
  return;
}

#endif

void Bluetooth::flush(){

}



#endif

Connection_type Bluetooth::get_type(){
  return BT;
}
