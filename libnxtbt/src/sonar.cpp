#include "sonar.h"
#include "error.h"

using namespace std;
using namespace nxt;


Sonar::Sonar(Sensor_port port, Connection* connection, Sonar_mode mode):I2c(port, connection, LOWSPEED_9V, SONAR_ADDRESS){
  this->sonar_mode = mode;
}

Sonar::~Sonar(){}

void Sonar::init(bool reply)
{
  std::cerr << "SONAR::INIT()" << std::endl;
  I2c::init(reply);
  set_mode(SINGLE_SHOT);
  //read();
  this->has_init=true;
}

Sensor_type Sonar::get_type(){
  if(sonar_mode == METRIC){
    return SONAR_METRIC;
  }
  else{
    return SONAR_INCH;
  }
}

void Sonar::write_register(int reg_adr, int value){
  unsigned char command[3];
  command[0]=SONAR_ADDRESS;
  command[1]=reg_adr;
  command[2]=value;
  i2c_write(&command[0],3,0x00);
  wait_for_bytes(0);
}

int Sonar::read_register(int reg_adr){
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]=SONAR_ADDRESS;
  command[1]=reg_adr;
  i2c_write(&command[0], 2, 1);
  wait_for_bytes(1);
  i2c_read(&rx_buffer[0], 1);
  return 0xff & rx_buffer[1]; //returns signed value
}

void Sonar::set_mode(int mode){
  write_register(COMMAND,mode);
}

void Sonar::sonar_reset(){
  set_mode(REQUEST_WARM_RESET);
}


void Sonar::off(){
  set_mode(OFF);
}

int Sonar::read(){
  if(!this->has_init){
    init();
  }
  // clear read registers again
  if (sonar_shot_mode == SINGLE) set_mode(SINGLE_SHOT);

  if(sonar_mode == METRIC){
    return read_register(RESULT_1);
  }
  else{
    return ( (read_register(RESULT_1))  * 3937)/1000;
  }
}

std::string Sonar::print(){
  std::stringstream out;
  if(sonar_mode == METRIC){
    out << read() << " CM";
  }
  else{
    out << read() << " inch(s)";
  }
  return out.str();
}

void Sonar::set(unsigned int value){

  switch ( value )
  {
    case  INCH :
    {
      sonar_mode = INCH;
      break;
    }
    case METRIC :
    {
      sonar_mode = METRIC;
      break;
    }

    case SINGLE:
    {
      sonar_shot_mode = SINGLE;
      break;
    }

    case CONTINUOUS:
    {
      sonar_shot_mode = CONTINUOUS;
      // set it to the mode
      set_mode(CONTINUOUS_MEASUREMENT);
      break;
    }
  }
  return;
}


