#include "error.h"

using namespace std;
using namespace nxt;

Nxt_exception::Nxt_exception(string function_name, string class_name, unsigned int error_code){
   this->function_name = function_name;
   this->class_name = class_name;
   this_error_code = error_code;
}

Nxt_exception::~Nxt_exception() throw(){

}

unsigned int Nxt_exception::error_code(){
  return this_error_code;
}

const char* Nxt_exception::what() const throw(){
  return (error_info(this_error_code));
}

const std::string Nxt_exception::who(){
   return ("Function \"" +function_name + "\" in class \"" + class_name + "\"");
}


const char * Nxt_exception::error_info(unsigned int error_code) const{
  string str;
  switch (error_code){
    case INVALID_COM_PORT:
      str = INVALID_COM_PORT_TEXT;
    break;
    case ERROR_WRITING_COM_PORT:
      str = ERROR_WRITING_COM_PORT_TEXT;
    break;
    case ERROR_READING_COM_PORT:
      str = ERROR_READING_COM_PORT_TEXT;
    break;
    case NO_REPLY:
      str = NO_REPLY_TEXT;
    break;
    case NOT_ENOUGH_BYTES:
      str = NOT_ENOUGH_BYTES_TEXT;
    break;
    case PENDING_COMMUNICATION:
      str = PENDING_COMMUNICATION_TEXT;
    break;
    case MAILBOX_QUEUE_EMPTY:
      str = MAILBOX_QUEUE_EMPTY_TEXT;
    break;
    case REQUEST_FAILED:
      str = REQUEST_FAILED_TEXT;
    break;
    case UNKNOW_COMMAND:
      str = UNKNOW_COMMAND_TEXT;
    break;
    case INSANE_PACKET:
      str = INSANE_PACKET_TEXT;
    break;
    case DATA_OUT_OF_RANGE:
      str = DATA_OUT_OF_RANGE_TEXT;
    break;
    case COMMUNICATION_BUS_ERROR:
      str = COMMUNICATION_BUS_ERROR_TEXT;
    break;
    case BUFFER_FULL:
      str = BUFFER_FULL_TEXT;
    break;
    case INVALID_CHANNEL:
      str = INVALID_CHANNEL_TEXT;
    break;
    case CHANNEL_BUSY:
      str = CHANNEL_BUSY_TEXT;
    break;
    case NO_ACTIVE_PROGRAM:
      str = NO_ACTIVE_PROGRAM_TEXT;
    break;
    case ILLEGAL_SIZE:
      str = ILLEGAL_SIZE_TEXT;
    break;
    case INVALID_MAILBOX_QUEUE:
      str = INVALID_MAILBOX_QUEUE_TEXT;
    break;
    case INVALID_FIELD:
      str = INVALID_FIELD_TEXT;
    break;
    case BAD_INPUT_OUTPUT:
      str = BAD_INPUT_OUTPUT_TEXT;
    break;
    case OUT_OF_MEMORY:
      str = OUT_OF_MEMORY_TEXT;
    break;
    case BAD_ARGUMENTS:
      str = BAD_ARGUMENTS_TEXT;
    break;
    case I2C_TIMEOUT:
      str = I2C_TIMEOUT_TEXT;
    break;
    case NO_MORE_HANDLES:
      str = NO_MORE_HANDLES_TEXT;
    break;
    case NO_SPACE:
      str = NO_SPACE_TEXT;
    break;
    case NO_MORE_FILES:
      str = NO_MORE_FILES_TEXT;
    break;
    case END_OF_FILE_EXPECTED:
      str = END_OF_FILE_EXPECTED_TEXT;
    break;
    case END_OF_FILE:
      str = END_OF_FILE_TEXT;
    break;
    case NOT_A_LINEAR_FILE:
      str = NOT_A_LINEAR_FILE_TEXT;
    break;
    case FILE_NOT_FOUND:
      str = FILE_NOT_FOUND_TEXT;
    break;
    case HANDLE_ALREADY_CLOSED:
      str = HANDLE_ALREADY_CLOSED_TEXT;
    break;
    case NO_LINEAR_SPACE:
      str = NO_LINEAR_SPACE_TEXT;
    break;
    case UNDEFINED_FILE_ERROR:
      str = UNDEFINED_FILE_ERROR_TEXT;
    break;
    case FILE_BUSY:
       str = FILE_BUSY_TEXT;
    break;
    case NO_WRITE_BUFFERS:
     str = NO_WRITE_BUFFERS_TEXT;
    break;
    case APPEND_NOT_POSSIBLE:
      str = APPEND_NOT_POSSIBLE_TEXT;
    break;
    case FILE_IS_FULL:
      str = FILE_IS_FULL_TEXT;
    break;
    case FILE_EXISTS:
     str = FILE_EXISTS_TEXT;
    break;
    case MODULE_NOT_FOUND:
     str = MODULE_NOT_FOUND_TEXT;
    break;
    case OUT_OF_BOUNDARY:
      str = OUT_OF_BOUNDARY_TEXT;
    break;
    case ILLEGAL_FILE_NAME:
      str = ILLEGAL_FILE_NAME_TEXT;
    break;
    case ILLEGAL_HANDLE:
      str = ILLEGAL_HANDLE_TEXT;
    break;
    case PC_FILE_ERROR:
      str = PC_FILE_ERROR_TEXT;
    break;
    case UNABLE_TO_CONNECT :
      str = UNABLE_TO_CONNECT_TEXT;
    break;
    default:
       str = "Unknown error";
    break;
  }
  return str.c_str();
}

