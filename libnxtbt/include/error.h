#ifndef NXT_ERRORH
#define NXT_ERRORH
#include <exception>
#include <iostream>
#include <string>
#include <sstream>


/**
* Class used to throw exceptions when communication fails or the NXT reports an error
* @see Nxt_errors
*/
namespace nxt
{



  class Nxt_exception: public std::exception
  {
    public:
      /**
       * @param function_name [the name of the function that throws the exception]
       * @param class_name [the name of the Class that throws the exception]
       * @param error_code [the error code that is thrown]
       */
      Nxt_exception(std::string function_name,std::string class_name, unsigned int error_code);
      ~Nxt_exception() throw();

      /**
       * What is the error
       * @return a std::string that describes the error
       */
      const char* what() const throw();

      /**
       * Get the error code that was thrown
       * @return the error code
       * @see Nxt_errors
       */
      unsigned int error_code() const;

      /**
       * Who threw the exception
       * @return a std::string that tells the class and the function that threw the exception (used for debug)
       */
      const std::string who() const;
    protected:
      std::string function_name;
      std::string class_name;
      unsigned int this_error_code;
      const char* error_info(unsigned int error_code) const;
  };

  /**
  * Error code enumeration (error code list)
  * @see Nxt_exception
  */
  enum Nxt_errors{

    /**
     * Invalid COM port specified
     */
    INVALID_COM_PORT = 0x0001,

    /**
     *Error when writing to COM port
     */
    ERROR_WRITING_COM_PORT = 0x0002,

    /**
     *Error when reading from COM port
     */
    ERROR_READING_COM_PORT = 0x0003,

    /**
     *No reply from the NXT
     */
    NO_REPLY = 0x0004,

    /**
     *Not enough bytes recieved from the NXT
     */
    NOT_ENOUGH_BYTES = 0x0005,

    /**
     *Unsable to connect to the NXT
     */
    UNABLE_TO_CONNECT = 0x0006,

    /**
     *Bytes/answer from I2C sensor not ready which resulted in a timeout
     */
    I2C_TIMEOUT = 0x0007,

    /**
     *Unable to read or write file on PC
     */
    PC_FILE_ERROR = 0x0008,

    /**
     *No more handles
     */
    NO_MORE_HANDLES = 0x0081,

    /**
     *No more space in flash
     */
    NO_SPACE = 0x0082,

    /**
     *No more files can be created
    */
    NO_MORE_FILES = 0x0083,

    /**
     *EOF expected
    */
    END_OF_FILE_EXPECTED = 0x0084,

    /**
     *EOF
    */
    END_OF_FILE = 0x0085,

    /**
     *File specified is not linear
    */
    NOT_A_LINEAR_FILE = 0x0086,

    /**
     *File not found
    */
    FILE_NOT_FOUND = 0x0087,

    /**
     *File is already closed
    */
    HANDLE_ALREADY_CLOSED = 0x0088,

    /**
     *No linear space
    */
    NO_LINEAR_SPACE = 0x0089,

    /**
     *Undefined file error occurred
    */
    UNDEFINED_FILE_ERROR = 0x008A,

    /**
     *File is busy
    */
    FILE_BUSY = 0x008B,

    /**
     *No write buffers
    */
    NO_WRITE_BUFFERS = 0x008C,

    /**
     *Unable to append
    */
    APPEND_NOT_POSSIBLE = 0x008D,

    /**
     *File is full
    */
    FILE_IS_FULL = 0x008E,

    /**
     *File already exists
    */
    FILE_EXISTS = 0x008F,

    /**
     *Module not found
    */
    MODULE_NOT_FOUND = 0x0090,

    /**
     *Out of boundary
    */
    OUT_OF_BOUNDARY = 0x0091,

    /**
     *Filename specified is illegal
    */
    ILLEGAL_FILE_NAME = 0x0092,

    /**
     *Filehandle specified is illegal
    */
    ILLEGAL_HANDLE = 0x0093,

    /**
     *Pending communication transaction in progress
    */
    PENDING_COMMUNICATION = 0x0020,

    /**
     *Specified mailbox queue is empty
    */
    MAILBOX_QUEUE_EMPTY = 0x0040,

    /**
     *Request failed (i.e. specified file not found)
    */
    REQUEST_FAILED = 0x00BD,

    /**
     *Unknown command opcode
    */
    UNKNOW_COMMAND = 0x00BE,

    /**
     *Insane packet
    */
    INSANE_PACKET = 0x00BF,

    /**
     *Data contains out-of-range value
    */
    DATA_OUT_OF_RANGE = 0x00C0,

    /**
     *Communication bus error
    */
    COMMUNICATION_BUS_ERROR = 0x00DD,

    /**
     *No free memory in communication buffer
    */
    BUFFER_FULL = 0x00DE,

    /**
     *Specified channel/connection is not valid
    */
    INVALID_CHANNEL = 0x00DF,

    /**
     *Specified channel/connection not configured or busy
    */
    CHANNEL_BUSY = 0x00E0,

    /**
     *No active program is running on the NXT
    */
    NO_ACTIVE_PROGRAM = 0x00EC,

    /**
     *Illegal size specified
    */
    ILLEGAL_SIZE = 0x00ED,

    /**
     *Illegal mailbox queue ID specified
    */
    INVALID_MAILBOX_QUEUE = 0x00EE,

    /**
     *Attempted to access invalid field of a structure
    */
    INVALID_FIELD = 0x00EF,

    /**
     *Bad input or output specified
    */
    BAD_INPUT_OUTPUT = 0x00F0,

    /**
     *Insufficient memory available
    */
    OUT_OF_MEMORY = 0x00FB,

    /**
     *Bad arguments
    */
    BAD_ARGUMENTS  = 0x00FF
  };

  //I2C error not a part of "LEGO" errors
  #define I2C_TIMEOUT_TEXT "Bytes/answer from I2C sensor not ready which resulted in a timeout"

  //PC-file error not a part of "LEGO" errors
  #define PC_FILE_ERROR_TEXT "Unable to read or write file on PC"

  //communication error
  #define INVALID_COM_PORT_TEXT "Invalid COM port or COM port in use"
  #define ERROR_WRITING_COM_PORT_TEXT "Error when writing to COM port"
  #define ERROR_READING_COM_PORT_TEXT "Error when reading from COM port"
  #define NO_REPLY_TEXT "No reply from the NXT"
  #define NOT_ENOUGH_BYTES_TEXT "Not enough bytes recieved from the NXT"
  #define UNABLE_TO_CONNECT_TEXT "Unable to connect to the NXT"

  //added from appendix 1 (for files)
  #define NO_MORE_HANDLES_TEXT "No more handles"
  #define NO_SPACE_TEXT "No space"
  #define NO_MORE_FILES_TEXT "No more files"
  #define END_OF_FILE_EXPECTED_TEXT "End of file expected"
  #define END_OF_FILE_TEXT "End of file"
  #define NOT_A_LINEAR_FILE_TEXT "Not a linear file"
  #define FILE_NOT_FOUND_TEXT "File not found"
  #define HANDLE_ALREADY_CLOSED_TEXT "Handle already closed"
  #define NO_LINEAR_SPACE_TEXT "No linear space"
  #define UNDEFINED_FILE_ERROR_TEXT "Undefined file error"
  #define FILE_BUSY_TEXT "File is busy"
  #define NO_WRITE_BUFFERS_TEXT "No write buffers"
  #define APPEND_NOT_POSSIBLE_TEXT "Append not possible"
  #define FILE_IS_FULL_TEXT "File is full"
  #define FILE_EXISTS_TEXT "File already exists"
  #define MODULE_NOT_FOUND_TEXT "Module not found"
  #define OUT_OF_BOUNDARY_TEXT "Out of boundary"
  #define ILLEGAL_FILE_NAME_TEXT "Illegal file name"
  #define ILLEGAL_HANDLE_TEXT "Illegal handle"

  //errors from the NXT (see the LEGO BT SDK appendix 2)
  #define PENDING_COMMUNICATION_TEXT "Pending communication transaction in progress"
  #define MAILBOX_QUEUE_EMPTY_TEXT "Specified mailbox queue is empty"
  #define REQUEST_FAILED_TEXT "Request failed (i.e. specified file not found)"
  #define UNKNOW_COMMAND_TEXT "Unknown command opcode"
  #define INSANE_PACKET_TEXT "Insane packet"
  #define DATA_OUT_OF_RANGE_TEXT "Data contains out-of-range values"
  #define COMMUNICATION_BUS_ERROR_TEXT "Communication bus error"
  #define BUFFER_FULL_TEXT "No free memory in communication buffer"
  #define INVALID_CHANNEL_TEXT "Specified channel/connection is not valid"
  #define CHANNEL_BUSY_TEXT "Specified channel/connection not configured or busy"
  #define NO_ACTIVE_PROGRAM_TEXT "No active program"
  #define ILLEGAL_SIZE_TEXT "Illegal size specified"
  #define INVALID_MAILBOX_QUEUE_TEXT "Illegal mailbox queue ID specified"
  #define INVALID_FIELD_TEXT "Attempted to access invalid field of a structure"
  #define BAD_INPUT_OUTPUT_TEXT "Bad input or output specified"
  #define OUT_OF_MEMORY_TEXT "Insufficient memory available"
  #define BAD_ARGUMENTS_TEXT "Bad arguments"
}
#endif

