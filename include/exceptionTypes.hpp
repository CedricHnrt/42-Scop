#ifndef EXCEPTIONTYPES_HPP
#define EXCEPTIONTYPES_HPP

#include <exception>
#include <string>
#include <utility>

enum errorType {
  	NO_ERROR,				    //0
  	NO_ARG_ERROR,			    //1
	TOO_MANY_ARG_ERROR,		    //2
  	WRONG_EXTENSION_ERROR,	    //3
	UNABLE_TO_OPEN_FILE_ERROR	//4
};

extern errorType errorCode;

class BaseException : public std::exception {
  protected:
    	std::string message;
  public:
      	explicit BaseException(std::string  message) : message(std::move(message)) {}
      	const char* what() const _NOEXCEPT override { return message.c_str(); }
};

class NoArgException : public BaseException {
  public: 
    	NoArgException() : BaseException("ERROR: Insert a .obj file name to load\nUSAGE: ./scop file.obj") {
          		errorCode = NO_ARG_ERROR;
    	}
};

class TooManyArgException : public BaseException {
  public: 
		TooManyArgException() : BaseException("ERROR: Too many arguments\nUSAGE: ./scop file.obj") {
                  errorCode = TOO_MANY_ARG_ERROR;
		}
};

class WrongExtensionException : public BaseException {
  public: 
		WrongExtensionException() : BaseException("ERROR: Wrong file extension\nUSAGE: ./scop file.obj") {
				  errorCode = WRONG_EXTENSION_ERROR;
		}
};

class UnableToOpenFileException : public BaseException {
  public: 
        UnableToOpenFileException() : BaseException("ERROR: Unable to open file") {
                  errorCode = UNABLE_TO_OPEN_FILE_ERROR;
        }
};

#endif //EXCEPTIONTYPES_HPP
