#ifndef OBJECTDATA_HPP
#define OBJECTDATA_HPP

#include <iostream>
#include <ostream>
#include "exceptionTypes.hpp"

class ObjectData {
  	public:
    	static ObjectData& getInstance();
		ObjectData(const ObjectData&) = delete;
		ObjectData& operator=(const ObjectData&) = delete;
		void* operator new(size_t) = delete;
		void operator delete(void*) = delete;
		void load(const char* filename);
		void printInfo() const;
    
   	private:
		ObjectData() = default;
		~ObjectData() = default;
		std::string filename;
};

#endif //OBJECTDATA_HPP
