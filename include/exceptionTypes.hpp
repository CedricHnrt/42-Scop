#ifndef EXCEPTIONTYPES_HPP
#define EXCEPTIONTYPES_HPP

#include <exception>
#include <string>

enum errorType {
  	NO_ERROR,				    //0
	UNDEFINED_ERROR,			//1
  	NO_ARG_ERROR,			    //2
	TOO_MANY_ARG_ERROR,		    //3
  	WRONG_EXTENSION_ERROR,	    //4
	UNABLE_TO_OPEN_OBJ_ERROR,	//5
	UNABLE_TO_OPEN_PPM_ERROR,	//6
	WRONG_PPM_ERROR,			//7
	RUNTIME_ERROR				//8
};

extern errorType errorCode;

class BaseException : public std::exception {
	protected:
   		std::string message;
	public:
     		explicit BaseException(std::string message) : message(std::move(message)) {}
     		[[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class NoArgException final : public BaseException {
	public: 
   		NoArgException() : BaseException("ERROR: Insert a .obj file name to load\nUSAGE: ./scop file.obj") {
          		errorCode = NO_ARG_ERROR;
   		}
};

class TooManyArgException final : public BaseException {
	public: 
		TooManyArgException() : BaseException("ERROR: Too many arguments\nUSAGE: ./scop file.obj") {
                		errorCode = TOO_MANY_ARG_ERROR;
		}
};

class WrongExtensionException final : public BaseException {
	public: 
		WrongExtensionException() : BaseException("ERROR: Wrong file extension\nUSAGE: ./scop file.obj") {
						errorCode = WRONG_EXTENSION_ERROR;
		}
};

class UnableToOpenOBJException final : public BaseException {
	public: 
	     UnableToOpenOBJException() : BaseException("ERROR: Unable to open OBJ file") {
	               errorCode = UNABLE_TO_OPEN_OBJ_ERROR;
	     }
};

class UnableToOpenPPMException final : public BaseException {
	public:
		explicit UnableToOpenPPMException(const std::string &file) : BaseException(std::string("ERROR: Unable to open \"") + file + "\"") {
			errorCode = UNABLE_TO_OPEN_PPM_ERROR;
		}
};

class WrongPPMFormatException final : public BaseException
{
	public:
		explicit WrongPPMFormatException(const std::string &file) : BaseException("ERROR: Wrong PPM format in file \"" + file + "\"") {
			errorCode = UNABLE_TO_OPEN_PPM_ERROR;
		}
};

class RuntimeException final : public BaseException {
	public:
		explicit RuntimeException(const std::string &message) : BaseException(message) {
			errorCode = RUNTIME_ERROR;
		}
};

#endif //EXCEPTIONTYPES_HPP
