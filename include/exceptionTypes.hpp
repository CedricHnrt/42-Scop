#ifndef EXCEPTIONTYPES_HPP
#define EXCEPTIONTYPES_HPP

#include <exception>
#include <string>

enum errorType {
  	NO_ERROR,				//0
  	NO_ARG_ERROR,			//1
	TOO_MANY_ARG_ERROR,		//2
  	WRONG_EXTENSION_ERROR	//3
};

extern errorType errorCode;

class BaseException : public std::exception {
  protected:
    	std::string message;
  public:
      	explicit BaseException(const std::string& message) : message(message) {}
      	const char* what() const _NOEXCEPT override { return message.c_str(); }
};

class NoArgException : public BaseException {
  public: 
    	NoArgException() : BaseException("ERROR: Insert a .obj file name to load\nUSAGE: ./scop file.obj\n") {
          		errorCode = NO_ARG_ERROR;
    	}
};

class TooManyArgException : public BaseException {
  public: 
		TooManyArgException() : BaseException("ERROR: Too many arguments\nUSAGE: ./scop file.obj\n") {
                  errorCode = TOO_MANY_ARG_ERROR;
		}
};

class WrongExtensionException : public BaseException {
  public: 
		WrongExtensionException() : BaseException("ERROR: Wrong file extension\nUSAGE: ./scop file.obj\n") {
				  errorCode = WRONG_EXTENSION_ERROR;
		}
};

#endif //EXCEPTIONTYPES_HPP
