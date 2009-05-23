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

unsigned int Nxt_exception::error_code() const {
  return this_error_code;
}

const char* Nxt_exception::what() const throw() {
  return (error_info(this_error_code));
}

const std::string Nxt_exception::who() const {
   return ("Function \"" +function_name + "\" in class \"" + class_name + "\"");
}


const char * Nxt_exception::error_info(unsigned int error_code) const{
  switch (error_code){
    case INVALID_COM_PORT:
      return INVALID_COM_PORT_TEXT;
    break;
    case ERROR_WRITING_COM_PORT:
      return ERROR_WRITING_COM_PORT_TEXT;
    break;
    case ERROR_READING_COM_PORT:
      return ERROR_READING_COM_PORT_TEXT;
    break;
    case NO_REPLY:
      return NO_REPLY_TEXT;
    break;
    case NOT_ENOUGH_BYTES:
      return NOT_ENOUGH_BYTES_TEXT;
    break;
    case PENDING_COMMUNICATION:
      return PENDING_COMMUNICATION_TEXT;
    break;
    case MAILBOX_QUEUE_EMPTY:
      return MAILBOX_QUEUE_EMPTY_TEXT;
    break;
    case REQUEST_FAILED:
      return REQUEST_FAILED_TEXT;
    break;
    case UNKNOW_COMMAND:
      return UNKNOW_COMMAND_TEXT;
    break;
    case INSANE_PACKET:
      return INSANE_PACKET_TEXT;
    break;
    case DATA_OUT_OF_RANGE:
      return DATA_OUT_OF_RANGE_TEXT;
    break;
    case COMMUNICATION_BUS_ERROR:
      return COMMUNICATION_BUS_ERROR_TEXT;
    break;
    case BUFFER_FULL:
      return BUFFER_FULL_TEXT;
    break;
    case INVALID_CHANNEL:
      return INVALID_CHANNEL_TEXT;
    break;
    case CHANNEL_BUSY:
      return CHANNEL_BUSY_TEXT;
    break;
    case NO_ACTIVE_PROGRAM:
      return NO_ACTIVE_PROGRAM_TEXT;
    break;
    case ILLEGAL_SIZE:
      return ILLEGAL_SIZE_TEXT;
    break;
    case INVALID_MAILBOX_QUEUE:
      return INVALID_MAILBOX_QUEUE_TEXT;
    break;
    case INVALID_FIELD:
      return INVALID_FIELD_TEXT;
    break;
    case BAD_INPUT_OUTPUT:
      return BAD_INPUT_OUTPUT_TEXT;
    break;
    case OUT_OF_MEMORY:
      return OUT_OF_MEMORY_TEXT;
    break;
    case BAD_ARGUMENTS:
      return BAD_ARGUMENTS_TEXT;
    break;
    case I2C_TIMEOUT:
      return I2C_TIMEOUT_TEXT;
    break;
    case NO_MORE_HANDLES:
      return NO_MORE_HANDLES_TEXT;
    break;
    case NO_SPACE:
      return NO_SPACE_TEXT;
    break;
    case NO_MORE_FILES:
      return NO_MORE_FILES_TEXT;
    break;
    case END_OF_FILE_EXPECTED:
      return END_OF_FILE_EXPECTED_TEXT;
    break;
    case END_OF_FILE:
      return END_OF_FILE_TEXT;
    break;
    case NOT_A_LINEAR_FILE:
      return NOT_A_LINEAR_FILE_TEXT;
    break;
    case FILE_NOT_FOUND:
      return FILE_NOT_FOUND_TEXT;
    break;
    case HANDLE_ALREADY_CLOSED:
      return HANDLE_ALREADY_CLOSED_TEXT;
    break;
    case NO_LINEAR_SPACE:
      return NO_LINEAR_SPACE_TEXT;
    break;
    case UNDEFINED_FILE_ERROR:
      return UNDEFINED_FILE_ERROR_TEXT;
    break;
    case FILE_BUSY:
       return FILE_BUSY_TEXT;
    break;
    case NO_WRITE_BUFFERS:
     return NO_WRITE_BUFFERS_TEXT;
    break;
    case APPEND_NOT_POSSIBLE:
      return APPEND_NOT_POSSIBLE_TEXT;
    break;
    case FILE_IS_FULL:
      return FILE_IS_FULL_TEXT;
    break;
    case FILE_EXISTS:
     return FILE_EXISTS_TEXT;
    break;
    case MODULE_NOT_FOUND:
     return MODULE_NOT_FOUND_TEXT;
    break;
    case OUT_OF_BOUNDARY:
      return OUT_OF_BOUNDARY_TEXT;
    break;
    case ILLEGAL_FILE_NAME:
      return ILLEGAL_FILE_NAME_TEXT;
    break;
    case ILLEGAL_HANDLE:
      return ILLEGAL_HANDLE_TEXT;
    break;
    case PC_FILE_ERROR:
      return PC_FILE_ERROR_TEXT;
    break;
    case UNABLE_TO_CONNECT :
      return UNABLE_TO_CONNECT_TEXT;
    break;
    default:
      return "Unknown error";
    break;
  }
}

