/*
 * DistNx.cpp
 *
 *  Created on: Feb 21, 2009
 *      Author: csp
 */

#include "DistNx.h"
#include "error.h"

using namespace std;
using namespace nxt;


DistNx::DistNx(Sensor_port port, Connection* connection):I2c(port, connection, LOWSPEED_9V, DISTNX_ADDRESS){

}

DistNx::~DistNx(){}

void DistNx::init(bool reply)
{
  std::cerr << "DISTNX::INIT()" << std::endl;
  I2c::init(reply);
  set_mode(DIST_NXT_LONG);
  set_mode(POWER_ON);
  //set_mode(SINGLE_SHOT);
  this->has_init=true;
}





void DistNx::write_register(int reg_adr, int value){
  unsigned char command[3];
  command[0]=DISTNX_ADDRESS;
  command[1]=reg_adr;
  command[2]=value;
  i2c_write(&command[0],3,0x00);
  wait_for_bytes(0);
}

int DistNx::read_register(int reg_adr){
  unsigned char rx_buffer[I2C_BUFFER_SIZE];
  unsigned char command[2];
  command[0]=DISTNX_ADDRESS;
  command[1]=reg_adr;
  i2c_write(&command[0], 2, 1);
  wait_for_bytes(1);
  i2c_read(&rx_buffer[0], 1);
  int val = ( 0x00FF & rx_buffer[0] );
  val    += ( (0x00FF & rx_buffer[1]) << 8 );

  std::cerr << "RAW " << val << std::endl;
  //return val;
  return 0xff & rx_buffer[1]; //returns signed value
}

void DistNx::set_mode(int mode){
  write_register(COMMAND,mode);
}

Sensor_type DistNx::get_type()
{

  int foo = read_register(SENSOR_MOD_TYPE);
  std::cout << foo << std::endl;

}

void DistNx::sonar_reset(){
  //set_mode(REQUEST_WARM_RESET);
}


void DistNx::off(){
  set_mode(POWER_OFF);
}

int DistNx::read(){
  if(!this->has_init){
    init();
  }
  set_mode(DIST_NXT_LONG);
  int dist = read_register(DIST_DATA_LSB);

  return dist ;

}

std::string DistNx::print(){
  std::stringstream out;
  out << read() << " MM";
  return out.str();
}

void DistNx::set(unsigned int value){

}

